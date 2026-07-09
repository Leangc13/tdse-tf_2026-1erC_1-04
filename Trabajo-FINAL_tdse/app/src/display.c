/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Application & Tasks includes */
#include "display.h"
#include "board.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/
static void lcd_enable_pulse(void);
static void lcd_send_nibble(char data);

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/

static void lcd_enable_pulse(void)
{
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
}

static void lcd_send_nibble(char data)
{
	HAL_GPIO_WritePin(LCD_D4_PORT, LCD_D4_PIN, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D5_PORT, LCD_D5_PIN, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D6_PORT, LCD_D6_PIN, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D7_PORT, LCD_D7_PIN, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void lcd_send_cmd(char cmd)
{
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET); /* RS=0 for command */
	lcd_send_nibble(cmd >> 4);   /* Upper nibble */
	lcd_enable_pulse();
	lcd_send_nibble(cmd & 0x0F); /* Lower nibble */
	lcd_enable_pulse();
}

void lcd_send_data(char data)
{
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET); /* RS=1 for data */
	lcd_send_nibble(data >> 4);   /* Upper nibble */
	lcd_enable_pulse();
	lcd_send_nibble(data & 0x0F); /* Lower nibble */
	lcd_enable_pulse();
}

void lcd_init(void)
{
	/* 4 bit initialisation sequence */
	HAL_Delay(50);  /* Wait for >40ms after VDD rises to 2.7V */
	HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
	
	lcd_send_nibble(0x03);
	lcd_enable_pulse();
	HAL_Delay(5);   /* Wait for >4.1ms */
	
	lcd_send_nibble(0x03);
	lcd_enable_pulse();
	HAL_Delay(1);   /* Wait for >100us */
	
	lcd_send_nibble(0x03);
	lcd_enable_pulse();
	HAL_Delay(10);
	
	lcd_send_nibble(0x02); /* Set 4-bit mode */
	lcd_enable_pulse();
	HAL_Delay(10);

	/* Display initialisation */
	lcd_send_cmd(0x28); /* DL=0 (4 bit mode), N = 1 (2 line display), F = 0 (5x8 characters) */
	HAL_Delay(1);
	lcd_send_cmd(0x08); /* Display off */
	HAL_Delay(1);
	lcd_send_cmd(0x01); /* Clear display */
	HAL_Delay(2);
	lcd_send_cmd(0x06); /* Entry mode set: increment cursor, no shift */
	HAL_Delay(1);
	lcd_send_cmd(0x0C); /* Display on, cursor off */
}

void lcd_send_string(char *str)
{
	while (*str)
		lcd_send_data(*str++);
}

void lcd_set_cursor(int row, int col)
{
	/* Select Row and Column */
	switch (row)
	{
		case 0:
			col |= 0x80;
			break;
		case 1:
			col |= 0xC0;
			break;
	}
	lcd_send_cmd(col);
}

void lcd_clear(void)
{
	lcd_send_cmd(0x01); /* Clear display */
	HAL_Delay(2);       /* Command takes a bit longer */
}

/********************** internal functions definition ************************/

/********************** end of file ******************************************/
