/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  UART_HandleTypeDef *huarts[] = {&huart1};
  buffered_serial_init(huarts);
  buffered_serial_serial_descriptor *serial1 = buffered_serial_get_huart_serial_descriptor(&huart1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //test 1

  uint8_t test1a[40] = "2A6V7W5NL5ZZC6AYE84NKZ6MVFMZ5DZSYD9TM3\r\n";
  static_strings_string_descriptor *string_descriptor = static_strings_save(test1a);
  buffered_serial_print_string(serial1,string_descriptor);
  static_strings_deallocate(string_descriptor);

  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
  uint8_t test1b[800] = "AENQ6TFME9R6JC47T94HXXK7758KKG5FN4A7PG47YKNMLUXR54UYJNTHZ9T3DAF6B9ELHBPS97NWZZTCVNDGMKV3WFAZ75Z6N7QE6WKHQ8DJR9HX56LLDMQX39NTA8TR89FS8794QSX9RNMEZ639NZVPPPCK2MXXGULX653GJVVT3SX3BV3NA687ZF9PE9ZVKN45373YSFTTS7G4HPV7CBECAYR24SEX4N7XTN8CUEXFYZB9WK3TC9Y3NR49DVWHHXJHZBSN5D2665GCNAJSDK4S4LTY42SNXZVAHSHECWRL7QFFC8ARET2HBDACUJJVS3X8ELKMZ98VEC9FN6VLBGP92NG7B8VH2LZQQ457NHP88RHVP5NNMY6CQWMS4HSX838NSLMJAVCYDPP2Y5F6MBFRUY4X5B894K7KZNQHRG5QHX35SE8A6JXCUF5999WNQJFXXDJR65CE3AWGUR3XD498E6WG3ZG6YTN6ATM7YBQ9LWWE7853PWA74QG6UMYQD7FFDA7NWGSCVNXX92ETRZEWKKJARV65Q7AM5SZ5E26U4D9CJU2YDJWXYX9WTPMMUM7L82FTME784EBYHGKAD7E8XT66SANF3394QEEK4JPJPFYFBDLD7RLF379THW6WBWV4CQRBVLXDAPDKM9ATY6D5U4QXC9FB7HRBZTKUSDKPJ5B4U2KED3EL4GAG57KB232G7HFR5CQ25KWV4LKZAQCDZGLNC2X8Y9FCVGZNPLQKZSKYP8GRYMQMH3266FHPMA4MSX27R5KY2JDBGGPFCHQFC2W65R\r\n";
  string_descriptor = static_strings_save(test1b);

  uint16_t length = string_descriptor->length;
  uint8_t length_string[10];
  sprintf((char *)length_string,"%08d",length);
  length_string[8] = '\r';
  length_string[9] = '\n';
  while(serial1->huart->gState == HAL_UART_STATE_BUSY_TX);
  HAL_UART_Transmit_DMA(serial1->huart,length_string,10);
  while(serial1->huart->gState == HAL_UART_STATE_BUSY_TX);

  buffered_serial_print_string(serial1,string_descriptor);
  static_strings_deallocate(string_descriptor);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

  // test 2

  uint8_t character = 'A';
  buffered_serial_print_character(serial1,character);

  while (1)
  {

	  // test 3-7 Echo

	  if(buffered_serial_available(serial1) > 0){
		  uint16_t available = buffered_serial_available(serial1);
		  uint8_t available_string[10];
		  sprintf((char *)available_string,"%08d",available);
		  available_string[8] = '\r';
		  available_string[9] = '\n';
		  HAL_UART_Transmit_DMA(serial1->huart,available_string,10);
		  string_descriptor = buffered_serial_read_line(serial1);
		  if(string_descriptor != NULL){
			  buffered_serial_print_string(serial1,string_descriptor);
			  static_strings_deallocate(string_descriptor);
		  }
		  else{
			  HAL_UART_Transmit_DMA(serial1->huart,(uint8_t *)"fail\r\n",7);
			  while(serial1->huart->gState == HAL_UART_STATE_BUSY_TX);
		  }
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
