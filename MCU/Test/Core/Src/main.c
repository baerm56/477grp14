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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tracker.h"
#include "speaker.h"
#include "leds.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
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
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac_ch1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint32_t sine[] = {1500, 1514, 1529, 1544, 1559, 1574, 1589, 1604,
		1619, 1634, 1649, 1664, 1679, 1694, 1709, 1724, 1738, 1753,
		1768, 1783, 1798, 1812, 1827, 1841, 1856, 1871, 1885, 1900,
		1914, 1928, 1943, 1957, 1971, 1986, 2000, 2014, 2028, 2042,
		2056, 2070, 2084, 2097, 2111, 2125, 2138, 2152, 2165, 2179,
		2192, 2205, 2219, 2232, 2245, 2258, 2271, 2284, 2296, 2309,
		2322, 2334, 2346, 2359, 2371, 2383, 2395, 2407, 2419, 2431,
		2443, 2454, 2466, 2477, 2489, 2500, 2511, 2522, 2533, 2544,
		2554, 2565, 2576, 2586, 2596, 2606, 2616, 2626, 2636, 2646,
		2656, 2665, 2674, 2684, 2693, 2702, 2711, 2720, 2728, 2737,
		2745, 2754, 2762, 2770, 2778, 2785, 2793, 2801, 2808, 2815,
		2822, 2829, 2836, 2843, 2850, 2856, 2862, 2869, 2875, 2881,
		2886, 2892, 2898, 2903, 2908, 2913, 2918, 2923, 2928, 2932,
		2937, 2941, 2945, 2949, 2953, 2956, 2960, 2963, 2966, 2969,
		2972, 2975, 2978, 2980, 2982, 2985, 2987, 2989, 2990, 2992,
		2993, 2995, 2996, 2997, 2998, 2998, 2999, 2999, 2999, 2999,
		2999, 2999, 2999, 2998, 2998, 2997, 2996, 2995, 2994, 2992,
		2991, 2989, 2987, 2985, 2983, 2981, 2978, 2975, 2973, 2970,
		2967, 2964, 2960, 2957, 2953, 2949, 2945, 2941, 2937, 2933,
		2928, 2924, 2919, 2914, 2909, 2904, 2898, 2893, 2887, 2882,
		2876, 2870, 2863, 2857, 2851, 2844, 2837, 2831, 2824, 2816,
		2809, 2802, 2794, 2787, 2779, 2771, 2763, 2755, 2747, 2738,
		2730, 2721, 2712, 2703, 2694, 2685, 2676, 2667, 2657, 2648,
		2638, 2628, 2618, 2608, 2598, 2588, 2577, 2567, 2556, 2545,
		2535, 2524, 2513, 2502, 2490, 2479, 2468, 2456, 2445, 2433,
		2421, 2409, 2397, 2385, 2373, 2361, 2348, 2336, 2324, 2311,
		2298, 2286, 2273, 2260, 2247, 2234, 2221, 2208, 2194, 2181,
		2168, 2154, 2141, 2127, 2113, 2100, 2086, 2072, 2058, 2044,
		2030, 2016, 2002, 1988, 1974, 1959, 1945, 1931, 1916, 1902,
		1887, 1873, 1858, 1844, 1829, 1815, 1800, 1785, 1770, 1756,
		1741, 1726, 1711, 1696, 1681, 1667, 1652, 1637, 1622, 1607,
		1592, 1577, 1562, 1547, 1532, 1517, 1502, 1487, 1472, 1457,
		1442, 1427, 1412, 1397, 1382, 1367, 1352, 1337, 1322, 1307,
		1293, 1278, 1263, 1248, 1233, 1219, 1204, 1189, 1174, 1160,
		1145, 1131, 1116, 1102, 1087, 1073, 1059, 1044, 1030, 1016,
		1002, 987, 973, 959, 945, 931, 918, 904, 890, 876, 863, 849,
		836, 822, 809, 796, 782, 769, 756, 743, 730, 718, 705, 692,
		679, 667, 655, 642, 630, 618, 606, 594, 582, 570, 558, 547,
		535, 524, 512, 501, 490, 479, 468, 457, 446, 436, 425, 415,
		404, 394, 384, 374, 364, 355, 345, 335, 326, 317, 308, 298,
		290, 281, 272, 264, 255, 247, 239, 231, 223, 215, 207, 200,
		192, 185, 178, 171, 164, 157, 150, 144, 138, 131, 125, 119,
		114, 108, 102, 97, 92, 87, 82, 77, 72, 68, 63, 59, 55, 51,
		47, 43, 40, 36, 33, 30, 27, 24, 22, 19, 17, 15, 13, 11, 9,
		7, 6, 5, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5,
		7, 8, 10, 12, 14, 16, 18, 21, 23, 26, 29, 32, 35, 38, 42, 45,
		49, 53, 57, 61, 65, 70, 75, 79, 84, 89, 94, 100, 105, 111, 117,
		122, 128, 135, 141, 147, 154, 161, 167, 174, 181, 189, 196, 203,
		211, 219, 227, 235, 243, 251, 259, 268, 277, 285, 294, 303, 312,
		322, 331, 340, 350, 360, 369, 379, 389, 400, 410, 420, 431, 441,
		452, 463, 474, 485, 496, 507, 518, 530, 541, 553, 564, 576, 588,
		600, 612, 624, 636, 649, 661, 673, 686, 699, 711, 724, 737, 750,
		763, 776, 789, 803, 816, 829, 843, 856, 870, 883, 897, 911, 925,
		939, 953, 967, 981, 995, 1009, 1023, 1037, 1052, 1066, 1080, 1095,
		1109, 1124, 1138, 1153, 1167, 1182, 1197, 1212, 1226, 1241, 1256,
		1271, 1285, 1300, 1315, 1330, 1345, 1360, 1375, 1390, 1405, 1420,
		1435, 1450, 1465, 1480, 1495};

const uint16_t sine_wave_array[32] = {2047, 1648, 1264, 910, 600,  345,
                   156, 39,  0,  39,  156,  345,
                   600, 910, 1264, 1648, 2048, 2447,
                   2831, 3185, 3495, 3750, 3939, 4056,
                   4095, 4056, 3939, 3750, 3495, 3185,
                   2831, 2447};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM6_Init(void);
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
  MX_USART1_UART_Init();
  MX_DAC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_DMA_Init();
  MX_FATFS_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  /*TrackerInit();

  SPI_HandleTypeDef hspi1 = LEDSInit();
  writeHexVal(hspi1, LED_CONFIGURATION, 0x01);
  writeHexVal(hspi1, LED_SCAN_LIMIT, 0x07);
  writeHexVal(hspi1, LED_INTENSITY, 0x07);
  writeHexVal(hspi1, LED_DECODE_MODE, 0xFF);
  writeHexVal(hspi1, LED_1, 0x01);
  writeHexVal(hspi1, LED_2, 0x02);
  writeHexVal(hspi1, LED_3, 0x03);*/


  /*HAL_Delay(1000); //a short delay is important to let the SD card settle

  //some variables for FatFs
  FATFS FatFs; 	//Fatfs handle
  FIL fil; 		//File handle
  FRESULT fres; //Result after operations

  fres = f_mount(&FatFs, "", 1);

  fres = f_open(&fil, "tomshere.txt", FA_CREATE_NEW);
  fres = f_close(&fil);

  //We're done, so de-mount the drive
  f_mount(NULL, "", 0);*/

  HAL_TIM_Base_Start(&htim6);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_wave_array, 32, DAC_ALIGN_12B_R);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim6);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  huart1.Init.BaudRate = 9600;
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
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
