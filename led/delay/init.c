/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>
#include <bsp/gpio.h>
#include <stdio.h>
#include <stm32h7xx_hal.h>

#include "../led.h"

#if defined(STM32H753xx)
#warning !! Using STM32H753xx
#elif defined(STM32H743xx)
#warning !! Using STM32H743xx
#endif

rtems_task Init(rtems_task_argument argument) {
    rtems_interval interval;

    puts("\n\n*** LED BLINKER (STM32H753zi) -- task wake after ***");

    interval = 0.25 * rtems_clock_get_ticks_per_second();

    LED_INIT();
#if defined(STM32H743xx_NOT)
    /* Enable GPIOB clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Configure GPIO pin for LED (PB0 - LD1/Green LED) */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  /* Push-pull output */
    GPIO_InitStruct.Pull = GPIO_NOPULL;          /* No pull-up or pull-down */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; /* Low speed is sufficient for LED */

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
    __HAL_RCC_GPIOB_CLK_ENABLE();

    rtems_gpio_bsp_select_output(1, 0, NULL);  // For STM32H7, LED on PB0 = bank 1, pin 0

    while (1) {
        (void)rtems_task_wake_after(interval);
        rtems_gpio_bsp_set(1, 0);  // Turn on LED PB0
        (void)rtems_task_wake_after(interval);
        rtems_gpio_bsp_clear(1, 0);  // Turn off LED PB0
    }

    (void)rtems_task_delete(RTEMS_SELF);
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
