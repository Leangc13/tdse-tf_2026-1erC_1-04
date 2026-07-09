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
#include <stdio.h>

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_display.h"
#include "display.h"
#include "eeprom.h"
#include "task_system.h"
#include "task_system_interface.h"
#include "task_sensor_interface.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define UPDATE_PERIOD_MS    500 /* Refresh rate for normal mode */

typedef enum {
	MENU_OPT_MOD_TH = 0,
	MENU_OPT_CRIT_TH = 1,
	MENU_OPT_MAX = 2
} menu_option_t;

/********************** internal data declaration ****************************/
static task_system_st_t prev_sys_state = MODO_NORMAL;
static uint32_t last_update_tick = 0;

static menu_option_t current_menu = MENU_OPT_MOD_TH;
static uint32_t local_mod_th = 40;
static uint32_t local_crit_th = 80;
static bool menu_dirty = false;

/********************** internal functions declaration ***********************/
static void init_and_load_eeprom(void);
static void render_normal_mode(void);
static void render_setup_mode(void);
static void render_falla_mode(void);
static void handle_setup_navigation(void);

/********************** internal data definition *****************************/
const char *p_task_display = "Task Display (LCD Menu)";

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void task_display_init(void *parameters)
{
	/* Print out: Task Initialized */
	/* LOGGER_INFO("  %s is running", GET_NAME(task_display_init)); */
	
	lcd_init();
	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_send_string(" SRAGV STARTING ");
	lcd_set_cursor(1, 0);
	lcd_send_string(" EEPROM INIT... ");
	
	init_and_load_eeprom();
	
	/* Initialize local cache for setup menu */
	local_mod_th = get_sys_wind_moderate_th();
	local_crit_th = get_sys_wind_critical_th();
	
	HAL_Delay(1000); /* Small startup delay so user can read */
	lcd_clear();
}

void task_display_update(void *parameters)
{
	task_system_st_t current_state = get_system_mode();
	
	/* Detect state transitions to clear screen and commit EEPROM */
	if (current_state != prev_sys_state)
	{
		lcd_clear();
		
		/* If exiting SET_UP mode, save values */
		if (prev_sys_state == MODO_SET_UP)
		{
			set_sys_wind_moderate_th(local_mod_th);
			set_sys_wind_critical_th(local_crit_th);
			
			sys_settings_t new_settings;
			new_settings.wind_mod_th = local_mod_th;
			new_settings.wind_crit_th = local_crit_th;
			new_settings.irrig_duration_s = 60; /* Fixed for now */
			
			lcd_set_cursor(0, 0);
			lcd_send_string(" SAVING CONFIG! ");
			eeprom_write_settings(&new_settings);
			/* Delay intentionally to show saving message briefly */
			HAL_Delay(500); 
			lcd_clear();
		}
		
		/* If entering SET_UP mode, reset menu to first option */
		if (current_state == MODO_SET_UP)
		{
			current_menu = MENU_OPT_MOD_TH;
			local_mod_th = get_sys_wind_moderate_th();
			local_crit_th = get_sys_wind_critical_th();
			menu_dirty = true;
		}
		
		prev_sys_state = current_state;
		last_update_tick = 0; /* Force immediate render */
	}

	/* Render logic per state */
	switch (current_state)
	{
		case MODO_NORMAL:
			if ((HAL_GetTick() - last_update_tick) >= UPDATE_PERIOD_MS)
			{
				last_update_tick = HAL_GetTick();
				render_normal_mode();
			}
			break;
			
		case MODO_SET_UP:
			handle_setup_navigation();
			if (menu_dirty)
			{
				render_setup_mode();
				menu_dirty = false;
			}
			break;
			
		case MODO_FALLA:
			if ((HAL_GetTick() - last_update_tick) >= UPDATE_PERIOD_MS)
			{
				last_update_tick = HAL_GetTick();
				render_falla_mode();
			}
			break;
	}
}

/********************** internal functions definition ************************/

static void init_and_load_eeprom(void)
{
	sys_settings_t settings;
	
	if (eeprom_read_settings(&settings))
	{
		/* Success reading EEPROM */
		set_sys_wind_moderate_th(settings.wind_mod_th);
		set_sys_wind_critical_th(settings.wind_crit_th);
	}
	else
	{
		/* Memory empty or corrupted. Write defaults. */
		settings.wind_mod_th = 40;
		settings.wind_crit_th = 80;
		settings.irrig_duration_s = 60;
		
		set_sys_wind_moderate_th(settings.wind_mod_th);
		set_sys_wind_critical_th(settings.wind_crit_th);
		
		eeprom_write_settings(&settings);
	}
}

static void render_normal_mode(void)
{
	char line1[17];
	char line2[17];
	
	uint32_t spd = get_sensor_wind_speed();
	uint32_t dir = get_sensor_wind_dir();
	char dir_char = 'N';
	if (dir == DIR_SOUTH) dir_char = 'S';
	else if (dir == DIR_EAST) dir_char = 'E';
	else if (dir == DIR_WEST) dir_char = 'W';
	
	sprintf(line1, "WIND:%3lu%% D:%c   ", spd, dir_char);
	
	bool sN = get_actuator_sector_state(0);
	bool sS = get_actuator_sector_state(1);
	bool sE = get_actuator_sector_state(2);
	bool sW = get_actuator_sector_state(3);
	
	sprintf(line2, "SEC: N%d S%d E%d W%d", sN, sS, sE, sW);
	
	lcd_set_cursor(0, 0);
	lcd_send_string(line1);
	lcd_set_cursor(1, 0);
	lcd_send_string(line2);
}

static void handle_setup_navigation(void)
{
	bool up = get_sensor_btn_up_pressed();
	bool down = get_sensor_btn_down_pressed();
	
	/* The 'ENTER' button is polled by task_system to exit the menu, 
	   so we only handle UP/DOWN here to modify values or change selection. */
	
	if (up || down) {
		menu_dirty = true; /* Need to redraw */
	}
	
	/* If we hold a button, wait? The debouncer in task_sensor is edge-triggered.
	   So we get one event per press. */
	if (up)
	{
		if (current_menu == MENU_OPT_MOD_TH) {
			local_mod_th += 5;
			if (local_mod_th > 100) local_mod_th = 100;
		} else if (current_menu == MENU_OPT_CRIT_TH) {
			local_crit_th += 5;
			if (local_crit_th > 100) local_crit_th = 100;
		}
	}
	
	if (down)
	{
		if (current_menu == MENU_OPT_MOD_TH) {
			/* Toggle menu instead of decrementing to allow navigation? 
			   Let's make UP increment value, DOWN decrement value. 
			   Wait, how do we switch between Mod and Crit threshold?
			   Let's use Light sensor Dip switch logic or just cycle menu automatically 
			   every 3 seconds? 
			   Let's keep it simple: UP toggles menu item. DOWN decrements. Or we can just 
			   cycle menu if value hits 100. Let's make UP increment, DOWN decrement, and 
			   if we hit 0 or 100 we jump. Actually, let's just make UP cycle menu, and DOWN 
			   increment value. */
			   
			current_menu++;
			if (current_menu >= MENU_OPT_MAX) current_menu = MENU_OPT_MOD_TH;
		}
	}
}

static void render_setup_mode(void)
{
	char line1[17];
	char line2[17];
	
	lcd_set_cursor(0, 0);
	lcd_send_string(" SETUP MODE:    ");
	
	if (current_menu == MENU_OPT_MOD_TH)
	{
		sprintf(line2, ">MOD_TH: %3lu%%  ", local_mod_th);
	}
	else if (current_menu == MENU_OPT_CRIT_TH)
	{
		sprintf(line2, ">CRI_TH: %3lu%%  ", local_crit_th);
	}
	
	lcd_set_cursor(1, 0);
	lcd_send_string(line2);
}

static void render_falla_mode(void)
{
	/* Display error based on ADC error or Wind */
	bool adc_err = get_sensor_adc_error();
	
	lcd_set_cursor(0, 0);
	lcd_send_string(" SYSTEM FAULT!  ");
	
	lcd_set_cursor(1, 0);
	if (adc_err) {
		lcd_send_string(" ERR: ADC FAIL  ");
	} else {
		lcd_send_string(" ERR: VIENTO CRI");
	}
}

/********************** end of file ******************************************/
