/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32f4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "includes.h"

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration    
    PB12     ------> SPI2_NSS
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }

}
//
///**
//* @brief SPI MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hspi: SPI handle pointer
//* @retval None
//*/
//void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
//{
//  if(hspi->Instance==SPI2)
//  {
//  /* USER CODE BEGIN SPI2_MspDeInit 0 */
//
//  /* USER CODE END SPI2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_SPI2_CLK_DISABLE();
//
//    /**SPI2 GPIO Configuration
//    PB12     ------> SPI2_NSS
//    PB13     ------> SPI2_SCK
//    PB14     ------> SPI2_MISO
//    PB15     ------> SPI2_MOSI
//    */
//    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
//
//  /* USER CODE BEGIN SPI2_MspDeInit 1 */
//
//  /* USER CODE END SPI2_MspDeInit 1 */
//  }
//
//}
//
///**
//* @brief UART MSP Initialization
//* This function configures the hardware resources used in this example
//* @param huart: UART handle pointer
//* @retval None
//*/
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(huart->Instance==USART2)
//  {
//  /* USER CODE BEGIN USART2_MspInit 0 */
//
//  /* USER CODE END USART2_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_USART2_CLK_ENABLE();
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**USART2 GPIO Configuration
//    PA2     ------> USART2_TX
//    PA3     ------> USART2_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN USART2_MspInit 1 */
//
//  /* USER CODE END USART2_MspInit 1 */
//  }
//
//}
//
///**
//* @brief UART MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param huart: UART handle pointer
//* @retval None
//*/
//void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
//{
//  if(huart->Instance==USART2)
//  {
//  /* USER CODE BEGIN USART2_MspDeInit 0 */
//
//  /* USER CODE END USART2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART2_CLK_DISABLE();
//
//    /**USART2 GPIO Configuration
//    PA2     ------> USART2_TX
//    PA3     ------> USART2_RX
//    */
//    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
//
//  /* USER CODE BEGIN USART2_MspDeInit 1 */
//
//  /* USER CODE END USART2_MspDeInit 1 */
//  }
//
//}

IRQn_Type MSP_GetIRQn( uint16_t GPIO_Pin)
{
  switch( GPIO_Pin )
  {
    case GPIO_PIN_0:  return EXTI0_IRQn;
    case GPIO_PIN_1:  return EXTI1_IRQn;
    case GPIO_PIN_2:  return EXTI2_IRQn;
    case GPIO_PIN_3:  return EXTI3_IRQn;
    case GPIO_PIN_4:  return EXTI4_IRQn;
    case GPIO_PIN_5:
    case GPIO_PIN_6:
    case GPIO_PIN_7:
    case GPIO_PIN_8:
    case GPIO_PIN_9:  return EXTI9_5_IRQn;
    case GPIO_PIN_10:
    case GPIO_PIN_11:
    case GPIO_PIN_12:
    case GPIO_PIN_13:
    case GPIO_PIN_14:
    case GPIO_PIN_15:
	default: return EXTI15_10_IRQn;
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
