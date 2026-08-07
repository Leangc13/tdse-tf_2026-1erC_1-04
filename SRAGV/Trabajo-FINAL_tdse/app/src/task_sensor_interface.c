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
#include "task_sensor_interface.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
/* These variables store the latest processed values */
volatile uint32_t sensor_wind_speed = 0;
volatile uint32_t sensor_wind_dir = 0;
volatile uint32_t sensor_light = 0;
volatile bool sensor_adc_error = false;

volatile uint8_t sensor_dip_switches = 0;

volatile bool flag_btn_enter = false;
volatile bool flag_btn_up = false;
volatile bool flag_btn_down = false;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/

uint32_t get_sensor_wind_speed(void)
{
	return sensor_wind_speed;
}

uint32_t get_sensor_wind_dir(void)
{
	return sensor_wind_dir;
}

uint32_t get_sensor_light(void)
{
	return sensor_light;
}

bool get_sensor_adc_error(void)
{
	return sensor_adc_error;
}

uint8_t get_sensor_dip_switches(void)
{
	return sensor_dip_switches;
}

bool get_sensor_btn_enter_pressed(void)
{
	bool current_flag = flag_btn_enter;
	flag_btn_enter = false; /* Clear on read */
	return current_flag;
}

bool get_sensor_btn_up_pressed(void)
{
	bool current_flag = flag_btn_up;
	flag_btn_up = false; /* Clear on read */
	return current_flag;
}

bool get_sensor_btn_down_pressed(void)
{
	bool current_flag = flag_btn_down;
	flag_btn_down = false; /* Clear on read */
	return current_flag;
}

/********************** end of file ******************************************/
