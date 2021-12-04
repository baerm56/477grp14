/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HEARTBEAT_LED_Pin GPIO_PIN_2
#define HEARTBEAT_LED_GPIO_Port GPIOE
#define HALLSEL0_Pin GPIO_PIN_0
#define HALLSEL0_GPIO_Port GPIOA
#define HALLSEL1_Pin GPIO_PIN_1
#define HALLSEL1_GPIO_Port GPIOA
#define HALLSEL2_Pin GPIO_PIN_2
#define HALLSEL2_GPIO_Port GPIOA
#define HALLOUT0_Pin GPIO_PIN_13
#define HALLOUT0_GPIO_Port GPIOF
#define HALLOUT1_Pin GPIO_PIN_14
#define HALLOUT1_GPIO_Port GPIOF
#define HALLOUT2_Pin GPIO_PIN_15
#define HALLOUT2_GPIO_Port GPIOF
#define HALLOUT3_Pin GPIO_PIN_0
#define HALLOUT3_GPIO_Port GPIOG
#define HALLOUT4_Pin GPIO_PIN_1
#define HALLOUT4_GPIO_Port GPIOG
#define HALLOUT5_Pin GPIO_PIN_7
#define HALLOUT5_GPIO_Port GPIOE
#define HALLOUT6_Pin GPIO_PIN_8
#define HALLOUT6_GPIO_Port GPIOE
#define HALLOUT7_Pin GPIO_PIN_9
#define HALLOUT7_GPIO_Port GPIOE
#define BUTTON1_Pin GPIO_PIN_12
#define BUTTON1_GPIO_Port GPIOD
#define BUTTON1_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON2_Pin GPIO_PIN_13
#define BUTTON2_GPIO_Port GPIOD
#define BUTTON2_EXTI_IRQn EXTI15_10_IRQn
#define SWITCH1_1_Pin GPIO_PIN_3
#define SWITCH1_1_GPIO_Port GPIOG
#define SWITCH1_2_Pin GPIO_PIN_4
#define SWITCH1_2_GPIO_Port GPIOG
#define SWITCH1_3_Pin GPIO_PIN_5
#define SWITCH1_3_GPIO_Port GPIOG
#define SWITCH2_1_Pin GPIO_PIN_6
#define SWITCH2_1_GPIO_Port GPIOG
#define SWITCH2_1_EXTI_IRQn EXTI9_5_IRQn
#define SWITCH2_2_Pin GPIO_PIN_7
#define SWITCH2_2_GPIO_Port GPIOG
#define SWITCH2_2_EXTI_IRQn EXTI9_5_IRQn
#define SWITCH2_3_Pin GPIO_PIN_8
#define SWITCH2_3_GPIO_Port GPIOG
#define SWITCH2_3_EXTI_IRQn EXTI9_5_IRQn
#define SD_CS_Pin GPIO_PIN_0
#define SD_CS_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi3
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
