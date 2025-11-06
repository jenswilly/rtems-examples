/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>
#include <stdio.h>

#include "../led.h"

rtems_task Init(rtems_task_argument argument) {
  rtems_interval one_second;

  puts("\n\n*** LED BLINKER -- task wake after ***");

  one_second = 1 * rtems_clock_get_ticks_per_second();

  LED_INIT();
#if defined(STM32H743xx)
  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure GPIO pin for LED (PB0 - LD1/Green LED) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; /* Push-pull output */
  GPIO_InitStruct.Pull = GPIO_NOPULL;         /* No pull-up or pull-down */
  GPIO_InitStruct.Speed =
      GPIO_SPEED_FREQ_LOW; /* Low speed is sufficient for LED */

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

  while (1) {

    (void)rtems_task_wake_after(one_second);
    LED_OFF();
    (void)rtems_task_wake_after(one_second);
    LED_ON();
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
