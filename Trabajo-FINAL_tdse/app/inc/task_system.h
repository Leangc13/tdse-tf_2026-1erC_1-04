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
#include <stdbool.h>
#ifndef TASK_SYSTEM_H_
#define TASK_SYSTEM_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
/* Wind Thresholds mapping */
#define WIND_CRITICAL_TH     90
#define WIND_MODERATE_TH     40

/* Direction constants */
#define DIR_NORTH            0
#define DIR_SOUTH            1
#define DIR_EAST             2
#define DIR_WEST             3

/* Light thresholds */
#define LIGHT_NIGHT_TH       10
#define LIGHT_DAY_TH         40

/********************** typedef **********************************************/

/* Events for the System State Machine */
typedef enum {
    EV_SYS_IDLE,
    EV_SYS_BTN_ENTER,
    EV_SYS_CRITICAL_ERROR,
    EV_SYS_ERROR_CLEARED
} task_system_ev_t;

/* System States */
typedef enum {
    MODO_NORMAL,
    MODO_SET_UP,
    MODO_FALLA
} task_system_st_t;

/* Data struct for task system */
typedef struct {
    task_system_st_t state;
    task_system_ev_t event;
    bool flag;
} task_system_dta_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
void task_system_init(void *parameters);
void task_system_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_SYSTEM_H_ */

/********************** end of file ******************************************/
