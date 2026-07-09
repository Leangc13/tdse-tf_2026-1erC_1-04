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
#include "eeprom.h"

/********************** macros and definitions *******************************/
#define EEPROM_MEM_ADDR 0x00 /* Start address for settings in EEPROM */

/********************** internal data declaration ****************************/
extern I2C_HandleTypeDef hi2c1;

/********************** internal functions declaration ***********************/
static uint8_t calculate_checksum(uint8_t *data, uint16_t length);

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void eeprom_init(void)
{
	/* I2C already initialized in main.c */
}

bool eeprom_read_settings(sys_settings_t *settings)
{
	uint8_t buffer[sizeof(sys_settings_t)];
	
	/* Read data from EEPROM */
	if (HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDR, EEPROM_MEM_ADDR, I2C_MEMADD_SIZE_8BIT, buffer, sizeof(buffer), 100) != HAL_OK)
	{
		return false;
	}
	
	/* Calculate expected checksum based on data bytes (excluding the last byte which is the checksum) */
	uint8_t calc_checksum = calculate_checksum(buffer, sizeof(sys_settings_t) - 1);
	
	/* The last byte is the stored checksum */
	uint8_t stored_checksum = buffer[sizeof(sys_settings_t) - 1];
	
	if (calc_checksum != stored_checksum)
	{
		return false; /* Memory corrupted or empty */
	}
	
	/* Copy data to structure */
	settings->wind_mod_th = *((uint32_t*)&buffer[0]);
	settings->wind_crit_th = *((uint32_t*)&buffer[4]);
	settings->irrig_duration_s = *((uint32_t*)&buffer[8]);
	settings->checksum = stored_checksum;
	
	return true;
}

bool eeprom_write_settings(sys_settings_t *settings)
{
	uint8_t buffer[sizeof(sys_settings_t)];
	
	/* Serialize data */
	*((uint32_t*)&buffer[0]) = settings->wind_mod_th;
	*((uint32_t*)&buffer[4]) = settings->wind_crit_th;
	*((uint32_t*)&buffer[8]) = settings->irrig_duration_s;
	
	/* Calculate checksum and append it */
	settings->checksum = calculate_checksum(buffer, sizeof(sys_settings_t) - 1);
	buffer[sizeof(sys_settings_t) - 1] = settings->checksum;
	
	/* AT24C02 has 8-byte pages. Writing more than 8 bytes sequentially without page handling 
	   might wrap around within the page. For simplicity, we write byte by byte to avoid page boundaries. */
	for (uint16_t i = 0; i < sizeof(buffer); i++)
	{
		if (HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDR, EEPROM_MEM_ADDR + i, I2C_MEMADD_SIZE_8BIT, &buffer[i], 1, 100) != HAL_OK)
		{
			return false;
		}
		HAL_Delay(5); /* Write cycle time for EEPROM */
	}
	
	return true;
}

/********************** internal functions definition ************************/
static uint8_t calculate_checksum(uint8_t *data, uint16_t length)
{
	uint8_t sum = 0;
	for (uint16_t i = 0; i < length; i++)
	{
		sum += data[i];
	}
	return sum;
}

/********************** end of file ******************************************/
