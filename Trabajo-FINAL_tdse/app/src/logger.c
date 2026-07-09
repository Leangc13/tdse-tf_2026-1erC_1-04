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

/********************** macros and definitions *******************************/
#define RX_BUFFER_SIZE  32
#define TX_BUFFER_SIZE  64
#define TELEMETRY_PERIOD_MS 2000

/********************** internal data declaration ****************************/
extern UART_HandleTypeDef huart2;

static uint8_t rx_char;
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_index = 0;
static bool msg_received = false;

static uint8_t tx_buffer[TX_BUFFER_SIZE];
static uint32_t telemetry_tick_stamp = 0;

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
	HAL_UART_Receive_IT(&huart2, &rx_char, 1);
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
	HAL_UART_Receive_IT(&huart2, &rx_char, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		logger_uart_rx_callback();
	}
}

/********************** internal functions definition ************************/
static void process_bt_command(void)
{
	if (strncmp((char*)rx_buffer, "GET:STATE", 9) == 0)
	{
		send_telemetry();
	}
	else if (strncmp((char*)rx_buffer, "SET:WIND_THR:", 13) == 0)
	{
		/* Very basic parsing, would use setter to update FSM threshold 
		   E.g., int val; sscanf((char*)rx_buffer, "SET:WIND_THR:%d", &val);
		   set_sys_wind_critical_th(val);
		   Not implemented here as setter wasn't requested in FSM interface yet, 
		   but this is where it maps.
		*/
		int val;
		if (sscanf((char*)rx_buffer, "SET:WIND_THR:%d", &val) == 1) {
			/* Success placeholder */
			sprintf((char*)tx_buffer, "OK:WIND_THR=%d\r\n", val);
			HAL_UART_Transmit_IT(&huart2, tx_buffer, strlen((char*)tx_buffer));
		}
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

	/* Format JSON payload for the Web App */
	sprintf((char*)tx_buffer, "{\"m\":\"%s\", \"v\":%lu, \"d\":\"%c\", \"s\":[%d,%d,%d,%d]}\n", 
		    mode_str, w_spd, dir_char, sN, sS, sE, sW);
			
	HAL_UART_Transmit_IT(&huart2, tx_buffer, strlen((char*)tx_buffer));
}

/********************** end of file ******************************************/
