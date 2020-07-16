/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "includes.h"
#include "radio.h"
#include "timeServer.h"
#include "delay.h"
#include "low_power.h"
#include "lora.h"

#include "vcom.h"

#if defined( USE_BAND_868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( USE_BAND_915 )

#define RF_FREQUENCY                                915000000 // Hz

#else
    #error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             14        // dBm

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

#define LEDS_OFF   do{ \
                   LED_Off( LED_BLUE ) ;   \
                   LED_Off( LED_RED ) ;    \
                   LED_Off( LED_GREEN1 ) ; \
                   LED_Off( LED_GREEN2 ) ; \
                   } while(0) ;

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

uint8_t BufferSPI[10];
/* Buffer used for reception */
uint8_t aRxBuffer[BUFFER_SIZE];

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

extern SPI_HandleTypeDef hspi1;

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

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;
uint8_t ReadyMsg[] = "READY";
uint8_t OKMsg[] = "OK";
int recibidoReady = 0;
int recibidoOK = 0;
char info[10];
int i = 1;

int main(void) {
	bool isMaster = true;

	HAL_Init();

	SystemClock_Config();

	//DBG_Init();
	Radio.IoInit();
	HW_SPI_Init();
	vcom_Init();
	UART_Init();
	USART2_UART_Init();

    SPI_Init(&hspi2);
    SPI2_Init();

	PRINTF("------------- PROGRAMA F411 -------------\r\n");

//   Radio initialization
	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;

	Radio.Init(&RadioEvents);

	Radio.SetChannel( RF_FREQUENCY);

	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
	LORA_SPREADING_FACTOR, LORA_CODINGRATE,
	LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
	true, 0, 0, LORA_IQ_INVERSION_ON, 3000000);

	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
	LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
	LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, true, 0, 0,
	LORA_IQ_INVERSION_ON, true);
	//Establece la radio en modo de recepción durante un tiempo
	Radio.Rx( RX_TIMEOUT_VALUE);
	DelayMs(1);


	//

	while (1) {

		HAL_SPI_TransmitReceive(&hspi2, "HOLA", (uint8_t *) BufferSPI, 8, 4000);
//		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
//		}

//		HAL_SPI_Receive(&hspi2, (uint8_t *) BufferSPI, 8, 1000);
		Radio.Send(BufferSPI, BufferSize);
		DelayMs(3);
//		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
//			}
//		PRINTF("%s\r\n", BufferSPI);
	//	HAL_Delay(1000);
//		//sprintf(info, "%s", BufferSPI);
//		strcpy(misDat[i].datos, BufferSPI);
//		PRINTF("%s\r\n", misDat[i] .datos);
//
//
//		//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//		HAL_SPI_Transmit(&hspi2, (uint8_t *) "ADIOS", 5, 1000);
//		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
//			}


//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		switch (State) {
		case RX:
			PRINTF("RX\n");
			if (isMaster == true) {
				if (BufferSize > 0) {
					PRINTF("MASTER: %s\r\n", Buffer);
//					if (strncmp((const char*) Buffer, (const char*) ReadyMsg, 5)== 0) {
//						PRINTF("RECIBIDO READY\r\n");
//						Radio.Send("PREST", BufferSize);
//						recibidoReady = 1;
//						//isMaster = true;
//					}
					if (strncmp((const char*) Buffer, (const char*) OKMsg, 2)
							== 0) {
						PRINTF("RECIBIDO OK\r\n");
						//PRINTF("DATOS: %s\r\n", misDat[i].datos);
						Radio.Send(BufferSPI, BufferSize);
						//Radio.Send(misDat[i].datos, BufferSize);
						recibidoOK = 1;
					}
//					else {
//						PRINTF("NI READY NI OK\r\n");
//					}
					Radio.Send("READER_MASTER_RX", BufferSize);
					DelayMs(1);
				}
			} else {
				if (BufferSize > 0) {
					PRINTF("SLAVE: %s\r\n", Buffer);
					Radio.Send("READER_SLAVE_RX", BufferSize);
				}
			}
			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case TX:
			PRINTF("TX\n");
			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case RX_TIMEOUT:
		case RX_ERROR:
			PRINTF("RX_ERROR\n");
//			if (recibidoReady == 0) {
//				PRINTF("RX_ERROR\n");
//				Radio.Send("PREST", BufferSize);
//				DelayMs(1);
//				Radio.Rx( RX_TIMEOUT_VALUE);
//			} else {
//				PRINTF("DATOS: %s\r\n", misDat[i].datos);
//				Radio.Send(misDat[i].datos, BufferSize);
//				DelayMs(1);
//				Radio.Rx( RX_TIMEOUT_VALUE);
//			}
			Radio.Send("READER_MASTER_RX_ERROR", BufferSize);
			State = LOWPOWER;
			break;
		case TX_TIMEOUT:
			Radio.Rx( RX_TIMEOUT_VALUE);
			State = LOWPOWER;
			break;
		case LOWPOWER:
		default:
			Radio.Rx( RX_TIMEOUT_VALUE);
			break;
		}
//		memset(BufferSPI, 0, sizeof(BufferSPI));
//		PRINTF("i: %d\r\n", i);
		i++;

		if (i == 14) {
			i = 0;
		}
		DISABLE_IRQ( );
		ENABLE_IRQ( );
		DelayMs(1);

	}
}

void OnTxDone(void) {
	Radio.Sleep();
	State = TX;
	PRINTF("OnTxDone\n");
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
	Radio.Sleep();
	BufferSize = size;
	memcpy(Buffer, payload, BufferSize);
	RssiValue = rssi;
	SnrValue = snr;
	State = RX;

	PRINTF("OnRxDone\n");
//	PRINTF("RssiValue=%d dBm, SnrValue=%d\n", rssi, snr);
}

void OnTxTimeout(void) {
	Radio.Sleep();
	State = TX_TIMEOUT;

	//PRINTF("OnTxTimeout\n");
}

void OnRxTimeout(void) {
	Radio.Sleep();
	State = RX_TIMEOUT;
	//PRINTF("OnRxTimeout\n");
}

void OnRxError(void) {
	Radio.Sleep();
	State = RX_ERROR;
	PRINTF("OnRxError\n");
}

static void OnledEvent(void) {
//  LED_Toggle( LED_BLUE ) ;
//  LED_Toggle( LED_RED1 ) ;
//  LED_Toggle( LED_RED2 ) ;
//  LED_Toggle( LED_GREEN ) ;
//
//  TimerStart(&timerLed );
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
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	  /** Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	  RCC_OscInitStruct.PLL.PLLQ = 4;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /** Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
