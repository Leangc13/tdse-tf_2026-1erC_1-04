#include "main.h"
#include <stdbool.h>

#include "app.h"
#include "logger.h"
#include "task_sensor.h"
#include "task_system.h"
#include "task_display.h"
#include "task_actuator.h"

typedef struct {
	void (*task_init)(void *);
	void (*task_update)(void *);
	void *parameters;
} task_cfg_t;

/* Array defining the execution order of our tasks */
const task_cfg_t task_cfg_list[] = {
    {logger_init,        logger_update,        NULL},
    {task_sensor_init,   task_sensor_update,   NULL},
    {task_system_init,   task_system_update,   NULL},
    {task_display_init,  task_display_update,  NULL},
    {task_actuator_init, task_actuator_update, NULL}
};

#define TASK_QTY (sizeof(task_cfg_list)/sizeof(task_cfg_t))

volatile uint32_t g_app_tick_cnt;

void app_init(void)
{
    uint32_t index;
    
    /* Initialize all tasks */
    for (index = 0; index < TASK_QTY; index++)
    {
        (*task_cfg_list[index].task_init)(task_cfg_list[index].parameters);
    }
    
    /* Safely initialize Tick Counter */
    __asm("CPSID i"); /* Disable interrupts */
    g_app_tick_cnt = 0;
    __asm("CPSIE i"); /* Enable interrupts */
}

void app_update(void)
{
    uint32_t index;
    bool b_time_update_required = false;

    /* Check if 1ms SysTick has elapsed */
    __asm("CPSID i");
    if (0 < g_app_tick_cnt)
    {
        g_app_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    /* Run the Super Loop execution */
    while (b_time_update_required)
    {
        for (index = 0; index < TASK_QTY; index++)
        {
            (*task_cfg_list[index].task_update)(task_cfg_list[index].parameters);
        }
        
        /* Low Power Sleep Mode: Wait until the next SysTick interrupt */
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
        
        /* Re-evaluate tick counter upon wake-up */
        __asm("CPSID i");
        if (0 < g_app_tick_cnt)
        {
            g_app_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");
    }
}

/* This overrides the WEAK HAL Systick callback to drive our super loop */
void HAL_SYSTICK_Callback(void)
{
    g_app_tick_cnt++;
}
