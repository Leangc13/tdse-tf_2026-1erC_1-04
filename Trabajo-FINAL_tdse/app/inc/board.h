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

#ifndef BOARD_H_
#define BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "main.h"

/********************** macros ***********************************************/
#define NUCLEO_F103RB		(0)
#define BOARD (NUCLEO_F103RB)

/* SRAGV Board Definitions mapped to standard NUCLEO-F103RB */
#if (BOARD == NUCLEO_F103RB)


/* Inputs: Buttons (Active Low - internal pull-up assumed) */
#define BTN_UP_PIN          BTN1_Pin
#define BTN_UP_PORT         BTN1_GPIO_Port
#define BTN_UP_PRESSED      GPIO_PIN_RESET
#define BTN_UP_HOVER        GPIO_PIN_SET

#define BTN_DOWN_PIN        BTN2_Pin
#define BTN_DOWN_PORT       BTN2_GPIO_Port
#define BTN_DOWN_PRESSED    GPIO_PIN_RESET
#define BTN_DOWN_HOVER      GPIO_PIN_SET

#define BTN_ENTER_PIN       BTN3_Pin
#define BTN_ENTER_PORT      BTN3_GPIO_Port
#define BTN_ENTER_PRESSED   GPIO_PIN_RESET
#define BTN_ENTER_HOVER     GPIO_PIN_SET

/* Inputs: DIP Switches */
#define DIP1_PIN            BTN1_DIP_Pin
#define DIP1_PORT           BTN1_DIP_GPIO_Port
#define DIP2_PIN            BTN2_DIP_Pin
#define DIP2_PORT           BTN2_DIP_GPIO_Port
#define DIP3_PIN            BTN3_DIP_Pin
#define DIP3_PORT           BTN3_DIP_GPIO_Port
#define DIP4_PIN            BTN4_DIP_Pin
#define DIP4_PORT           BTN4_DIP_GPIO_Port
#define DIP_ON              GPIO_PIN_RESET
#define DIP_OFF             GPIO_PIN_SET

/* Outputs: Status LEDs (Active High) */
#define LED_PWR_PIN         LED1_Pin
#define LED_PWR_PORT        LED1_GPIO_Port
#define LED_RUN_PIN         LED2_Pin
#define LED_RUN_PORT        LED2_GPIO_Port
#define LED_ALERTA_PIN      LED3_Pin
#define LED_ALERTA_PORT     LED3_GPIO_Port
#define LED_ON              GPIO_PIN_SET
#define LED_OFF             GPIO_PIN_RESET

/* Outputs: Sector LEDs (Active High) */
#define SECT_N_PIN          LEDN_Pin
#define SECT_N_PORT         LEDN_GPIO_Port
#define SECT_S_PIN          LEDS_Pin
#define SECT_S_PORT         LEDS_GPIO_Port
#define SECT_E_PIN          LEDE_Pin
#define SECT_E_PORT         LEDE_GPIO_Port
#define SECT_W_PIN          LEDW_Pin
#define SECT_W_PORT         LEDW_GPIO_Port
#define SECT_ON             GPIO_PIN_SET
#define SECT_OFF            GPIO_PIN_RESET

/* Outputs: Buzzer (Active High) */
#define BUZZER_PIN          BUZZ_Pin
#define BUZZER_PORT         BUZZ_GPIO_Port
#define BUZZER_ON           GPIO_PIN_SET
#define BUZZER_OFF          GPIO_PIN_RESET

/* LCD 16x2 Parallel Pins (4-bit mode) */
#define LCD_RS_PIN          LCDRS_Pin
#define LCD_RS_PORT         LCDRS_GPIO_Port
#define LCD_EN_PIN          LCDEN_Pin
#define LCD_EN_PORT         LCDEN_GPIO_Port
#define LCD_D4_PIN          LCDD4_Pin
#define LCD_D4_PORT         LCDD4_GPIO_Port
#define LCD_D5_PIN          LCDD5_Pin
#define LCD_D5_PORT         LCDD5_GPIO_Port
#define LCD_D6_PIN          LCDD6_Pin
#define LCD_D6_PORT         LCDD6_GPIO_Port
#define LCD_D7_PIN          LCDD7_Pin
#define LCD_D7_PORT         LCDD7_GPIO_Port

/* I2C1 (EEPROM AT24C02) mapped */
/* PB8 -> I2C1_SCL (or PB6 depending on remap) */
/* PB9 -> I2C1_SDA (or PB7 depending on remap) */

/* USART2 (Bluetooth HM-10 / Logger) mapped */
/* PA2 -> USART2_TX */
/* PA3 -> USART2_RX */

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_H_ */

/********************** end of file ******************************************/
