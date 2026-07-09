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
#include "task_system.h"
#include "task_system_interface.h"
/* To be created: */
/* #include "task_sensor_interface.h" */
/* #include "task_actuator_interface.h" */

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
static task_system_dta_t task_system_dta;
task_system_st_t current_sys_state = MODO_NORMAL;

/* Dummy prototypes until the actual task interfaces are implemented */
extern uint32_t get_sensor_wind_speed(void);
extern uint32_t get_sensor_wind_dir(void);
extern uint32_t get_sensor_light(void);
extern bool get_sensor_adc_error(void);
extern void put_actuator_sector(uint8_t sector, bool state);
extern void put_actuator_status_led(uint8_t led_id, bool state);
extern void put_actuator_buzzer(bool state);

/********************** internal functions declaration ***********************/
static void evaluate_irrigation_logic(void);
static void suspend_irrigation(void);

/********************** internal data definition *****************************/
const char *p_task_system = "Task System (System FSM)";

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	/* Print out: Task Initialized */
	/* LOGGER_INFO("  %s is running", GET_NAME(task_system_init)); */

	init_event_task_system();

	task_system_dta.state = MODO_NORMAL;
	task_system_dta.event = EV_SYS_IDLE;
	task_system_dta.flag = false;
}

void task_system_update(void *parameters)
{
	/* Fetch new events */
	if (true == any_event_task_system())
	{
		task_system_dta.flag = true;
		task_system_dta.event = get_event_task_system();
	}

	/* Force MODO_FALLA if sensor errors exist or critical wind */
	if (task_system_dta.state != MODO_FALLA)
	{
		if (get_sensor_adc_error() == true || get_sensor_wind_speed() >= get_sys_wind_critical_th())
		{
			task_system_dta.state = MODO_FALLA;
			task_system_dta.flag = false;
		}
	}

	current_sys_state = task_system_dta.state;

	/* 2. Procesar: Execute state logic and evaluate transitions */
	switch (task_system_dta.state)
	{
		case MODO_NORMAL:
			
			/* Turn RUN led ON, others OFF */
			put_actuator_status_led(1, true); /* RUN LED */
			put_actuator_status_led(2, false); /* ALERTA LED */
			put_actuator_buzzer(false);

			if (task_system_dta.flag && task_system_dta.event == EV_SYS_BTN_ENTER)
			{
				task_system_dta.flag = false;
				task_system_dta.state = MODO_SET_UP;
			}
			else
			{
				evaluate_irrigation_logic();
			}

			break;

		case MODO_SET_UP:

			/* Suspend irrigation */
			suspend_irrigation();
			put_actuator_status_led(1, false); /* RUN LED off */

			if (task_system_dta.flag && task_system_dta.event == EV_SYS_BTN_ENTER)
			{
				task_system_dta.flag = false;
				task_system_dta.state = MODO_NORMAL;
			}

			break;

		case MODO_FALLA:

			/* Inhibits sectors, blinks ALERTA LED, sounds buzzer */
			suspend_irrigation();
			put_actuator_status_led(1, false); /* RUN LED off */
			
			/* The blinking and buzzing will be handled via events in actuator, 
			   but we signal the fall state. */
			put_actuator_status_led(2, true); /* ALERTA */
			put_actuator_buzzer(true);

			if (task_system_dta.flag && task_system_dta.event == EV_SYS_ERROR_CLEARED)
			{
				/* Only exit if the hardware/critical condition cleared */
				if (get_sensor_adc_error() == false && get_sensor_wind_speed() < get_sys_wind_critical_th())
				{
					task_system_dta.flag = false;
					put_actuator_status_led(2, false);
					put_actuator_buzzer(false);
					task_system_dta.state = MODO_NORMAL;
				}
			}

			break;

		default:
			task_system_dta.state = MODO_NORMAL;
			break;
	}
}

static void evaluate_irrigation_logic(void)
{
	uint32_t wind_speed = get_sensor_wind_speed();
	uint32_t wind_dir = get_sensor_wind_dir();
	uint32_t light = get_sensor_light();

	bool inhibit_all = false;
	bool inhibit_N = false, inhibit_S = false, inhibit_E = false, inhibit_W = false;

	/* Night Inhibition */
	if (is_night_irrigation_inhibited() && light < LIGHT_NIGHT_TH)
	{
		inhibit_all = true;
	}

	/* Moderate Wind Inhibition (Counter-sectors) */
	if (wind_speed >= get_sys_wind_moderate_th() && wind_speed < get_sys_wind_critical_th())
	{
		/* If wind blows from North, inhibit South sector, etc. */
		if (wind_dir == DIR_NORTH) inhibit_S = true;
		if (wind_dir == DIR_SOUTH) inhibit_N = true;
		if (wind_dir == DIR_EAST)  inhibit_W = true;
		if (wind_dir == DIR_WEST)  inhibit_E = true;
	}

	/* Update actuator sectors based on inhibition */
	if (inhibit_all)
	{
		suspend_irrigation();
	}
	else
	{
		put_actuator_sector(DIR_NORTH, !inhibit_N);
		put_actuator_sector(DIR_SOUTH, !inhibit_S);
		put_actuator_sector(DIR_EAST,  !inhibit_E);
		put_actuator_sector(DIR_WEST,  !inhibit_W);
	}
}

static void suspend_irrigation(void)
{
	put_actuator_sector(DIR_NORTH, false);
	put_actuator_sector(DIR_SOUTH, false);
	put_actuator_sector(DIR_EAST, false);
	put_actuator_sector(DIR_WEST, false);
}

/********************** end of file ******************************************/
