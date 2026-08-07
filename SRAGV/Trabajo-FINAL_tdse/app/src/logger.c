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
#include <string.h>

/* Application & Tasks includes */
#include "logger.h"
#include "task_sensor_interface.h"
#include "task_actuator_interface.h"
#include "task_system_interface.h"
#include "eeprom.h"

/********************** macros and definitions *******************************/
#define RX_BUFFER_SIZE  32
#define TX_BUFFER_SIZE  256
#define TELEMETRY_PERIOD_MS 2000

/********************** internal data declaration ****************************/
extern UART_HandleTypeDef huart1;

static uint8_t rx_char;
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_index = 0;
static bool msg_received = false;

static uint8_t tx_buffer[TX_BUFFER_SIZE];
static uint32_t telemetry_tick_stamp = 0;
static volatile bool tx_busy = false;

/********************** internal functions declaration ***********************/
static void process_bt_command(void);
static void send_telemetry(void);

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void logger_init(void *parameters)
{
	rx_index = 0;
	msg_received = false;
	telemetry_tick_stamp = HAL_GetTick();
	
	/* Start listening for the first character non-blocking */
	HAL_UART_Receive_IT(&huart1, &rx_char, 1);
}

void logger_update(void *parameters)
{
	/* Parse incoming message if any */
	if (msg_received)
	{
		process_bt_command();
		msg_received = false;
	}

	/* Send telemetry periodically non-blocking */
	if ((HAL_GetTick() - telemetry_tick_stamp) >= TELEMETRY_PERIOD_MS)
	{
		telemetry_tick_stamp = HAL_GetTick();
		send_telemetry();
	}
}

/* Call this from HAL_UART_RxCpltCallback */
void logger_uart_rx_callback(void)
{
	if (rx_char == '\n' || rx_char == '\r')
	{
		if (rx_index > 0) {
			rx_buffer[rx_index] = '\0';
			msg_received = true;
			rx_index = 0;
		}
	}
	else
	{
		rx_buffer[rx_index++] = rx_char;
		if (rx_index >= RX_BUFFER_SIZE) {
			rx_index = 0; /* Overflow protection */
		}
	}
	
	/* Re-arm interrupt */
	HAL_UART_Receive_IT(&huart1, &rx_char, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		logger_uart_rx_callback();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		tx_busy = false;
	}
}

/********************** internal functions definition ************************/
static void process_bt_command(void)
{
	if (strncmp((char*)rx_buffer, "GET:STATE", 9) == 0)
	{
		send_telemetry();
	}
	else if (strncmp((char*)rx_buffer, "SET:", 4) == 0)
	{
		int val;
		if (sscanf((char*)rx_buffer, "SET:MOD:%d", &val) == 1) {
			set_sys_wind_moderate_th(val);
		} else if (sscanf((char*)rx_buffer, "SET:CRI:%d", &val) == 1) {
			set_sys_wind_critical_th(val);
		} else if (sscanf((char*)rx_buffer, "SET:TIM:%d", &val) == 1) {
			set_sys_irrig_duration_s(val);
		} else if (sscanf((char*)rx_buffer, "SET:NIG:%d", &val) == 1) {
			set_sys_enable_night_irrig(val);
		} else if (sscanf((char*)rx_buffer, "SET:INH:%d", &val) == 1) {
			set_sys_web_inhibits(val);
		} else if (sscanf((char*)rx_buffer, "SET:OPM:%d", &val) == 1) {
			set_sys_op_mode(val);
		}
		
		/* Persist settings to EEPROM (except web_inhibits which are runtime) */
		sys_settings_t current;
		current.wind_mod_th = get_sys_wind_moderate_th();
		current.wind_crit_th = get_sys_wind_critical_th();
		current.irrig_duration_s = get_sys_irrig_duration_s();
		current.enable_night_irrig = get_sys_enable_night_irrig();
		current.op_mode = get_sys_op_mode();
		eeprom_write_settings(&current);

		/* Force telemetry update to reflect changes immediately */
		send_telemetry();
	}
}

static void send_telemetry(void)
{
	uint32_t w_spd = get_sensor_wind_speed();
	uint32_t w_dir = get_sensor_wind_dir();
	
	bool sN = get_actuator_sector_state(0);
	bool sS = get_actuator_sector_state(1);
	bool sE = get_actuator_sector_state(2);
	bool sW = get_actuator_sector_state(3);
	
	/* Convert System Mode */
	char* mode_str = "OFFLINE";
	switch(get_system_mode()) {
		case MODO_NORMAL: mode_str = "NORMAL"; break;
		case MODO_SET_UP: mode_str = "SETUP"; break;
		case MODO_FALLA:  mode_str = "FALLA"; break;
	}

	/* Convert Wind Direction */
	char dir_char = '-';
	switch(w_dir) {
		case DIR_NORTH: dir_char = 'N'; break;
		case DIR_SOUTH: dir_char = 'S'; break;
		case DIR_EAST:  dir_char = 'E'; break;
		case DIR_WEST:  dir_char = 'O'; break;
	}

	/* Fetch Current Configs */
	uint32_t c_mod = get_sys_wind_moderate_th();
	uint32_t c_cri = get_sys_wind_critical_th();
	uint32_t c_tim = get_sys_irrig_duration_s();
	uint32_t c_nig = get_sys_enable_night_irrig();
	uint8_t  c_inh = get_sys_web_inhibits();
	uint32_t c_opm = get_sys_op_mode();
	uint32_t c_lux = get_sensor_light(); /* LDR raw % value for debugging */
	
	/* Send JSON payload */
	sprintf((char*)tx_buffer, "{\"m\":\"%s\",\"v\":%lu,\"d\":\"%c\",\"s\":[%d,%d,%d,%d],\"c_mod\":%lu,\"c_cri\":%lu,\"c_tim\":%lu,\"c_nig\":%lu,\"c_inh\":%d,\"c_opm\":%lu,\"c_lux\":%lu}\r\n", 
		    mode_str, w_spd, dir_char, sN, sS, sE, sW, c_mod, c_cri, c_tim, c_nig, c_inh, c_opm, c_lux);
			
	/* Use non-blocking interrupt transmit to avoid stalling the super-loop */
	if (!tx_busy)
	{
		tx_busy = true;
		HAL_UART_Transmit_IT(&huart1, tx_buffer, strlen((char*)tx_buffer));
	}
}

/********************** end of file ******************************************/
