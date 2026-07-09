/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_sensor.h"
#include "task_sensor_interface.h"
#include "task_system.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define DEBOUNCE_TIME_MS    50 /* 50ms debounce window */

/********************** internal data declaration ****************************/
extern ADC_HandleTypeDef hadc1; /* From main.c */

/* Raw ADC buffer populated by DMA */
/* [0]: Wind Speed (Joy X)
   [1]: Wind Direction (Joy Y)
   [2]: LDR (Light) */
static volatile uint16_t adc_raw_buffer[ADC_CHANNELS_QTY];

/* Moving Average filter variables for Wind Speed */
static uint32_t wind_filter_buffer[FILTER_WINDOW_SIZE];
static uint32_t wind_filter_index = 0;
static uint32_t wind_filter_sum = 0;
static bool filter_initialized = false;

/* Hardware variables defined in task_sensor_interface.c */
extern volatile uint32_t sensor_wind_speed;
extern volatile uint32_t sensor_wind_dir;
extern volatile uint32_t sensor_light;
extern volatile bool sensor_adc_error;

extern volatile uint8_t sensor_dip_switches;

extern volatile bool flag_btn_enter;
extern volatile bool flag_btn_up;
extern volatile bool flag_btn_down;

/* Button Debounce tracking */
typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	btn_state_t state;
	uint32_t tick_stamp;
	volatile bool *flag_ptr;
} button_dta_t;

static button_dta_t buttons[3];

/********************** internal functions declaration ***********************/
static void process_adc_data(void);
static void button_debounce_update(void);
static void update_dip_switches(void);

/********************** internal data definition *****************************/
const char *p_task_sensor = "Task Sensor (Escrutar FSM)";

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void task_sensor_init(void *parameters)
{
	/* Print out: Task Initialized */
	/* LOGGER_INFO("  %s is running", GET_NAME(task_sensor_init)); */

	/* Initialize Moving Average buffer */
	for (uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) {
		wind_filter_buffer[i] = 0;
	}
	wind_filter_sum = 0;
	wind_filter_index = 0;
	filter_initialized = false;

	/* Start ADC + DMA in circular mode (Assuming configured in CubeMX as continuous/circular) */
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_raw_buffer, ADC_CHANNELS_QTY) != HAL_OK)
	{
		sensor_adc_error = true;
	}
	/* Deshabilitar la interrupción del DMA para evitar la tormenta de interrupciones (CPU Lockup) */
	HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);


	/* Initialize Buttons data */
	buttons[0].port = BTN_ENTER_PORT;
	buttons[0].pin  = BTN_ENTER_PIN;
	buttons[0].state = BTN_ST_UP;
	buttons[0].flag_ptr = &flag_btn_enter;

	buttons[1].port = BTN_UP_PORT;
	buttons[1].pin  = BTN_UP_PIN;
	buttons[1].state = BTN_ST_UP;
	buttons[1].flag_ptr = &flag_btn_up;

	buttons[2].port = BTN_DOWN_PORT;
	buttons[2].pin  = BTN_DOWN_PIN;
	buttons[2].state = BTN_ST_UP;
	buttons[2].flag_ptr = &flag_btn_down;
}

void task_sensor_update(void *parameters)
{
	/* 1. Escrutar: Update Inputs */
	button_debounce_update();
	update_dip_switches();

	/* Push ENTER event to system queue directly to respect specific instruction: 
	   task_system waits for EV_SYS_BTN_ENTER. */
	if (get_sensor_btn_enter_pressed()) {
		put_event_task_system(EV_SYS_BTN_ENTER);
	}
}

/* 
 * This callback is meant to be called from the HAL_ADC_ConvCpltCallback 
 * which is implemented in app_it.c or right below if we define it here. 
 */
void task_sensor_adc_cplt_callback(void)
{
	/* Called safely from interrupt context or deferred queue */
	process_adc_data();
}

/* Override the weak HAL callback. In the Cátedra style, usually placed in app_it.c, 
   but placed here for encapsulation if app_it.c only calls it. */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
	{
		task_sensor_adc_cplt_callback();
	}
}

/********************** internal functions definition ************************/
static void process_adc_data(void)
{
	uint16_t raw_wind = adc_raw_buffer[0];
	uint16_t raw_dir  = adc_raw_buffer[1];
	uint16_t raw_lght = adc_raw_buffer[2];

	/* Moving average filter for Wind Speed */
	wind_filter_sum -= wind_filter_buffer[wind_filter_index];
	wind_filter_buffer[wind_filter_index] = raw_wind;
	wind_filter_sum += wind_filter_buffer[wind_filter_index];
	
	wind_filter_index++;
	if (wind_filter_index >= FILTER_WINDOW_SIZE) {
		wind_filter_index = 0;
		filter_initialized = true;
	}

	uint32_t avg_wind;
	if (filter_initialized) {
		avg_wind = wind_filter_sum / FILTER_WINDOW_SIZE;
	} else {
		/* Avoid dividing by zero or smaller window size logic, just use raw until filled */
		avg_wind = raw_wind;
	}

	/* Store processed variables */
	
	/* Convert raw wind (0-4095) to percentage (0-100) for easier thresholds */
	sensor_wind_speed = (avg_wind * 100) / 4095;

	/* Convert Light (0-4095) to percentage (0-100) */
	sensor_light = (raw_lght * 100) / 4095;

	/* Resolve Direction Zone from Joystick Y (0-4095) into 4 quadrants */
	if (raw_dir < 1000) {
		sensor_wind_dir = DIR_SOUTH;
	} else if (raw_dir < 2000) {
		sensor_wind_dir = DIR_WEST;
	} else if (raw_dir < 3000) {
		sensor_wind_dir = DIR_EAST;
	} else {
		sensor_wind_dir = DIR_NORTH;
	}
	
	/* Basic saturation/null check (Fault condition) */
	if (raw_wind == 0xFFFF || raw_wind == 0) { /* Actually 12 bits max is 4095 */
		/* Just an example of basic check. In real hardware 0 and 4095 might be valid */
		/* sensor_adc_error = true; */
	}
}

static void button_debounce_update(void)
{
	uint8_t i;
	for (i = 0; i < 3; i++)
	{
		GPIO_PinState pin_state = HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin);
		bool is_pressed = (pin_state == GPIO_PIN_RESET); /* Active low */

		switch (buttons[i].state)
		{
			case BTN_ST_UP:
				if (is_pressed)
				{
					buttons[i].state = BTN_ST_FALLING;
					buttons[i].tick_stamp = HAL_GetTick();
				}
				break;

			case BTN_ST_FALLING:
				if (is_pressed)
				{
					if ((HAL_GetTick() - buttons[i].tick_stamp) >= DEBOUNCE_TIME_MS)
					{
						buttons[i].state = BTN_ST_DOWN;
						*(buttons[i].flag_ptr) = true; /* Trigger event */
					}
				}
				else
				{
					buttons[i].state = BTN_ST_UP;
				}
				break;

			case BTN_ST_DOWN:
				if (!is_pressed)
				{
					buttons[i].state = BTN_ST_RISING;
					buttons[i].tick_stamp = HAL_GetTick();
				}
				break;

			case BTN_ST_RISING:
				if (!is_pressed)
				{
					if ((HAL_GetTick() - buttons[i].tick_stamp) >= DEBOUNCE_TIME_MS)
					{
						buttons[i].state = BTN_ST_UP;
					}
				}
				else
				{
					buttons[i].state = BTN_ST_DOWN;
				}
				break;
		}
	}
}

static void update_dip_switches(void)
{
	uint8_t val = 0;
	if (HAL_GPIO_ReadPin(DIP1_PORT, DIP1_PIN) == DIP_ON) val |= (1 << 0);
	if (HAL_GPIO_ReadPin(DIP2_PORT, DIP2_PIN) == DIP_ON) val |= (1 << 1);
	if (HAL_GPIO_ReadPin(DIP3_PORT, DIP3_PIN) == DIP_ON) val |= (1 << 2);
	if (HAL_GPIO_ReadPin(DIP4_PORT, DIP4_PIN) == DIP_ON) val |= (1 << 3);

	sensor_dip_switches = val;
}

/********************** end of file ******************************************/
