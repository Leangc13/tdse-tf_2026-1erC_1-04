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
#include "task_actuator.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define BLINK_PERIOD_MS    500  /* 500ms ON, 500ms OFF = 1Hz */
#define BUZZER_PERIOD_MS   500

/********************** internal data declaration ****************************/
/* Import shared states from interface module */
extern volatile bool actuator_sectors[4];
extern volatile bool actuator_status_leds[3];
extern volatile bool actuator_buzzer;

static uint32_t blink_tick_stamp = 0;
static bool blink_state = false;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_actuator = "Task Actuator (Actuar FSM)";

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void task_actuator_init(void *parameters)
{
	/* Print out: Task Initialized */
	/* LOGGER_INFO("  %s is running", GET_NAME(task_actuator_init)); */
	
	blink_tick_stamp = HAL_GetTick();
	blink_state = false;
	
	/* Default OFF states */
	HAL_GPIO_WritePin(SECT_N_PORT, SECT_N_PIN, SECT_OFF);
	HAL_GPIO_WritePin(SECT_S_PORT, SECT_S_PIN, SECT_OFF);
	HAL_GPIO_WritePin(SECT_E_PORT, SECT_E_PIN, SECT_OFF);
	HAL_GPIO_WritePin(SECT_W_PORT, SECT_W_PIN, SECT_OFF);
	
	HAL_GPIO_WritePin(LED_PWR_PORT, LED_PWR_PIN, LED_ON); /* PWR always ON */
	HAL_GPIO_WritePin(LED_RUN_PORT, LED_RUN_PIN, LED_OFF);
	HAL_GPIO_WritePin(LED_ALERTA_PORT, LED_ALERTA_PIN, LED_OFF);
	
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_OFF);
}

void task_actuator_update(void *parameters)
{
	/* 1. Actuar: Update Outputs based on internal state vars */
	
	/* Sectors */
	HAL_GPIO_WritePin(SECT_N_PORT, SECT_N_PIN, actuator_sectors[0] ? SECT_ON : SECT_OFF);
	HAL_GPIO_WritePin(SECT_S_PORT, SECT_S_PIN, actuator_sectors[1] ? SECT_ON : SECT_OFF);
	HAL_GPIO_WritePin(SECT_E_PORT, SECT_E_PIN, actuator_sectors[2] ? SECT_ON : SECT_OFF);
	HAL_GPIO_WritePin(SECT_W_PORT, SECT_W_PIN, actuator_sectors[3] ? SECT_ON : SECT_OFF);
	
	/* Status LEDs */
	/* LED PWR is hardcoded ON in init */
	HAL_GPIO_WritePin(LED_RUN_PORT, LED_RUN_PIN, actuator_status_leds[ID_LED_RUN] ? LED_ON : LED_OFF);
	
	/* ALERTA and BUZZER blinking logic (non-blocking) */
	if (actuator_status_leds[ID_LED_ALERTA] || actuator_buzzer)
	{
		/* Handle blinking using SysTick */
		if ((HAL_GetTick() - blink_tick_stamp) >= BLINK_PERIOD_MS)
		{
			blink_tick_stamp = HAL_GetTick();
			blink_state = !blink_state;
		}
		
		/* Apply blink state */
		if (actuator_status_leds[ID_LED_ALERTA]) {
			HAL_GPIO_WritePin(LED_ALERTA_PORT, LED_ALERTA_PIN, blink_state ? LED_ON : LED_OFF);
		} else {
			HAL_GPIO_WritePin(LED_ALERTA_PORT, LED_ALERTA_PIN, LED_OFF);
		}
		
		if (actuator_buzzer) {
			HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, blink_state ? BUZZER_ON : BUZZER_OFF);
		} else {
			HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_OFF);
		}
	}
	else
	{
		/* Turn off if false and reset blink tracker so it starts ON when triggered */
		HAL_GPIO_WritePin(LED_ALERTA_PORT, LED_ALERTA_PIN, LED_OFF);
		HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_OFF);
		blink_state = true; 
		blink_tick_stamp = HAL_GetTick();
	}
}

/********************** end of file ******************************************/
