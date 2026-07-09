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

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
static task_system_ev_t task_system_event;
static bool b_task_system_event = false;

/* System Config values */
static uint32_t wind_critical_th = WIND_CRITICAL_TH;
static uint32_t wind_moderate_th = WIND_MODERATE_TH;
static bool night_irrigation_inhibited = true;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_event_task_system(void)
{
	task_system_event = EV_SYS_IDLE;
	b_task_system_event = false;
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

bool is_night_irrigation_inhibited(void)
{
	return night_irrigation_inhibited;
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

/********************** end of file ******************************************/
