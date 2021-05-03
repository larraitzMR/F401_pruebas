/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <string.h>
#include "includes.h"
#include "../Lora/Phy/radio.h"
#include "../Lora/Utilities/timeServer.h"
#include "../Lora/Utilities/delay.h"
#include "../Lora/Utilities/low_power.h"
#include "../Lora/Core/lora.h"
#include "stm32f4xx_hal.h"
#include "hw_rtc.h"

#include "vcom.h"

#define USE_BAND_868
#if defined( USE_BAND_868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( USE_BAND_915 )

#define RF_FREQUENCY                                915000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             14        // dBm

#define USE_MODEM_LORA

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
//  1: 250 kHz,
//  2: 500 kHz,
//  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
//  2: 4/6,
//  3: 4/7,
//  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#elif defined( USE_MODEM_FSK )

#define FSK_FDEV                                    25e3      // Hz
#define FSK_DATARATE                                50e3      // bps
#define FSK_BANDWIDTH                               50e3      // Hz
#define FSK_AFC_BANDWIDTH                           83.333e3  // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#else
    #error "Please define a modem in the compiler options."
#endif

typedef enum {
	LOWPOWER, RX, RX_TIMEOUT, RX_ERROR, TX, TX_TIMEOUT,
} States_t;

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 64 // Define the payload size here
#define LED_PERIOD_MS               200
extern RTC_HandleTypeDef RtcHandle;
uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

uint8_t BufferSPI[10];
/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE];

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/* Led Timers objects*/
static TimerEvent_t timerLed;

/* Private function prototypes -----------------------------------------------*/
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone(void);

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout(void);

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout(void);

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError(void);

/*!
 * \brief Function executed on when led timer elapses
 */
static void OnledEvent(void);
/**
 * Main application entry point.
 */

struct datosVariscite {
	char datos[10];
};

struct datosVariscite misDat[15];

//uint8_t ReadyMsg[] = "READY";
uint8_t OKMsg[] = "OK";
int recibidoReady = 0;
int recibidoOK = 0;
char info[10];
int i = 1;

uint8_t ReadyMsg[] = "PREST";
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";
char buff[5];

int esclavo = 0;
int prest = 0;

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi4;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI4_Init(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	HAL_Init();

	SystemClock_Config();

	//DBG_Init();

	MX_GPIO_Init();
	HW_SPI_Init();
	HW_RTC_Init();
	Radio.IoInit();

	//Radio initialization
	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;

	uint32_t response = Radio.Init(&RadioEvents);
	Radio.SetChannel( RF_FREQUENCY);

	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	true, 0, 0, LORA_IQ_INVERSION_ON, 3000000);

	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
	LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
	LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, true, 0, 0,
	LORA_IQ_INVERSION_ON, true);

	bool isMaster = true;
	if (isMaster) {
		//PRINTF("\r\n------------- SOY MASTER2  -------------\r\n");
	} else {
		//PRINTF("\r\n------------- SOY ESCLAVO -------------\r\n");
	}

	//Establece la radio en modo de recepción durante un tiempo
	Radio.Rx( RX_TIMEOUT_VALUE);
	DelayMs(1);

	while (1) {

		switch (State) {

		case RX:

			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); // //D14 LEDGREEN  D15 LEDRED

			if (isMaster == true) {
				if (BufferSize > 0) {
					Radio.Send((uint8_t*) "HOLA, ESCLAVO", 13); //ESCLAVO
					Radio.Rx( RX_TIMEOUT_VALUE);
				}
			} else {
				if (BufferSize > 0) {
					Radio.Send((uint8_t*) "HOLA, MAESTRO", 13);
					Radio.Rx( RX_TIMEOUT_VALUE);
				}
			}
			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case TX:

			/*SET RF-SWITCH OFF FOR RECEPTION*/
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);

			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case RX_TIMEOUT:
		case RX_ERROR:

			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15,
					(State == RX_ERROR ? GPIO_PIN_SET : GPIO_PIN_RESET)); // //D14 LEDGREEN  D15 LEDRED
			if (isMaster == true) {

				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15,
						(State == RX_TIMEOUT ? GPIO_PIN_SET : GPIO_PIN_RESET)); // //D14 LEDGREEN  D15 LEDRED

				/*SET RF-SWITCH ON FOR TRANSMISSION*/
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);

				Radio.Send((uint8_t*) "HOLA, ESCLAVO", 13);
			}
			Radio.Rx( RX_TIMEOUT_VALUE);

			/*SET RF-SWITCH OFF FOR RECEPTION*/
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);

			State = LOWPOWER;
			break;

		case TX_TIMEOUT:

			/*SET RF-SWITCH OFF FOR RECEPTION*/
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);

			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case LOWPOWER:
		default:
			//Radio.Rx( RX_TIMEOUT_VALUE);
			break;
		}
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSI)
 *            SYSCLK(Hz)                     = 84000000
 *            HCLK(Hz)                       = 84000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSI Frequency(Hz)              = 16000000
 *            PLL_M                          = 16
 *            PLL_N                          = 336
 *            PLL_P                          = 4
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale2 mode
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		Error_Handler();
	}

}

void OnTxDone(void) {
	Radio.Sleep();
	State = TX;
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
	Radio.Sleep();
	BufferSize = size;
	memcpy(Buffer, payload, BufferSize);
	RssiValue = rssi;
	SnrValue = snr;
	State = RX;
}

void OnTxTimeout(void) {
	Radio.Sleep();
	State = TX_TIMEOUT;
}

void OnRxTimeout(void) {
	Radio.Sleep();
	State = RX_TIMEOUT;
}

void OnRxError(void) {
	Radio.Sleep();
	State = RX_ERROR;
}

//void OnledEvent(void) {
//	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14|GPIO_PIN_15);
//	TimerStart(&timerLed );
//}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
void MX_SPI1_Init(void) {

	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
void MX_SPI2_Init(void) {

	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler();
	}

}

/**
 * @brief SPI4 Initialization Function
 * @param None
 * @retval None
 */
void MX_SPI4_Init(void) {

	/* SPI4 parameter configuration*/
	hspi4.Instance = SPI4;
	hspi4.Init.Mode = SPI_MODE_MASTER;
	hspi4.Init.Direction = SPI_DIRECTION_2LINES;
	hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi4.Init.NSS = SPI_NSS_SOFT;
	hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi4.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi4) != HAL_OK) {
		Error_Handler();
	}

}

void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET); //E4 SA_NSS (CS SPI4) E14 SA_FEMCTX (ENABLE SWITCH RF)

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); //A3 SX_FEMCTX (ENABLE SWITCH RF) A4 SA_NSS (ENABLE SWITCH RF)

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET); //D14 LEDGREEN  D15 LEDRED

	/*Configure GPIO pins : PE4 PE14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : PA3 PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_3; //|GPIO_PIN_4
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PD14 PD15 */
	GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
}
