/*
 * spi.c
 *
 *  Created on: 27 ago. 2019
 *      Author: Larraitz
 */

#include "includes.h"
#include "gpio.h"
#include "config.h"

void GPIO_Ports_Init(void)
{

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

}
