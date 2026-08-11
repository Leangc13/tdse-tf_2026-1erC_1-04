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
#include "task_system.h"
#include "task_system_interface.h"
#include "eeprom.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
static task_system_ev_t task_system_event;
static bool b_task_system_event = false;

/* System Config values */
static uint32_t wind_critical_th = WIND_CRITICAL_TH;
static uint32_t wind_moderate_th = WIND_MODERATE_TH;
static uint32_t irrig_duration_s = 60;
static uint32_t enable_night_irrig = 0; /* Default OFF */
static bool night_irrigation_inhibited = true;
static uint8_t web_inhibits = 0;
static uint32_t op_mode = 1; /* Default Mode 1 */

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_event_task_system(void)
{
	task_system_event = EV_SYS_IDLE;
	b_task_system_event = false;

	/* Try to load saved settings from EEPROM */
	sys_settings_t saved;
	if (eeprom_read_settings(&saved))
	{
		/* Checksum valid: use saved values */
		wind_critical_th = saved.wind_crit_th;
		wind_moderate_th = saved.wind_mod_th;
		irrig_duration_s = saved.irrig_duration_s;
		enable_night_irrig = saved.enable_night_irrig;
	}
	/* else: keep hardcoded defaults */
}

void put_event_task_system(task_system_ev_t event)
{
	task_system_event = event;
	b_task_system_event = true;
}

task_system_ev_t get_event_task_system(void)
{
	b_task_system_event = false;
	return task_system_event;
}

bool any_event_task_system(void)
{
	return b_task_system_event;
}

uint32_t get_sys_wind_critical_th(void)
{
	return wind_critical_th;
}

uint32_t get_sys_wind_moderate_th(void)
{
	return wind_moderate_th;
}

uint32_t get_sys_irrig_duration_s(void)
{
	return irrig_duration_s;
}

uint32_t get_sys_enable_night_irrig(void)
{
	return enable_night_irrig;
}

bool is_night_irrigation_inhibited(void)
{
	/* Night irrigation is inhibited if light is low AND user disabled it */
	return (enable_night_irrig == 0);
}

uint8_t get_sys_web_inhibits(void)
{
	return web_inhibits;
}

uint32_t get_sys_op_mode(void)
{
	return op_mode;
}

task_system_st_t get_system_mode(void)
{
    /* To access the actual state we might need to grab it from task_system_dta 
       but we can just store a copy here or make it visible. Let's declare it in task_system.c and read it. */
    extern task_system_st_t current_sys_state;
    return current_sys_state;
}

void set_sys_wind_critical_th(uint32_t val)
{
    wind_critical_th = val;
}

void set_sys_wind_moderate_th(uint32_t val)
{
    wind_moderate_th = val;
}

void set_sys_irrig_duration_s(uint32_t val)
{
	irrig_duration_s = val;
}

void set_sys_enable_night_irrig(uint32_t val)
{
	enable_night_irrig = val;
}

void set_sys_web_inhibits(uint8_t mask)
{
	web_inhibits = mask;
}

void set_sys_op_mode(uint32_t val)
{
	op_mode = val;
}

/********************** end of file ******************************************/
