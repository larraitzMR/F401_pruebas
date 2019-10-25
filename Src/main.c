
#include "includes.h"
#include "spi.h"
#include "uart.h"

uint8_t buffTX[] = "JELOU";
uint8_t buffRX[10];

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();


	/* Initialize all configured peripherals */
	//GPIO_Init();
	SPI_Init(&hspi2);
	SPI2_Init();
	USART2_UART_Init();

	char *msg = "\n\rPROGRAMA SPI\n\r";
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), 0xFFFF);

	while (1) {
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		//HAL_SPI_TransmitReceive(&hspi2, (uint8_t*) buffTX, (uint8_t*) buffRX, 8, 1000);
		HAL_SPI_Receive(&hspi2, buffRX, 9, 1000);
		//HAL_SPI_Transmit(&hspi2, buffTX, 5, 1000);
		while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
		}
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffRX, strlen(buffRX), 1000);
		//bzero(buffRX, sizeof(buffRX));
		//HAL_Delay(1000);
		//fflush(buffRX);
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

