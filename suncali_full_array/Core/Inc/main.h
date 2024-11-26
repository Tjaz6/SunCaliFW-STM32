/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

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
#define COL12_Pin GPIO_PIN_13
#define COL12_GPIO_Port GPIOC
#define COL11_Pin GPIO_PIN_14
#define COL11_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOC
#define TAMP_OUT0_Pin GPIO_PIN_0
#define TAMP_OUT0_GPIO_Port GPIOA
#define TAMP_OUT1_Pin GPIO_PIN_1
#define TAMP_OUT1_GPIO_Port GPIOA
#define TAMP_OUT2_Pin GPIO_PIN_2
#define TAMP_OUT2_GPIO_Port GPIOA
#define TAMP_OUT3_Pin GPIO_PIN_3
#define TAMP_OUT3_GPIO_Port GPIOA
#define TAMP_OUT4_Pin GPIO_PIN_4
#define TAMP_OUT4_GPIO_Port GPIOA
#define TAMP_OUT5_Pin GPIO_PIN_5
#define TAMP_OUT5_GPIO_Port GPIOA
#define TAMP_OUT6_Pin GPIO_PIN_6
#define TAMP_OUT6_GPIO_Port GPIOA
#define TAMP_OUT7_Pin GPIO_PIN_7
#define TAMP_OUT7_GPIO_Port GPIOA
#define TAMP_OUT8_Pin GPIO_PIN_0
#define TAMP_OUT8_GPIO_Port GPIOB
#define TAMP_OUT9_Pin GPIO_PIN_1
#define TAMP_OUT9_GPIO_Port GPIOB
#define TAMP_OUT10_Pin GPIO_PIN_2
#define TAMP_OUT10_GPIO_Port GPIOB
#define COL7_Pin GPIO_PIN_10
#define COL7_GPIO_Port GPIOB
#define TAMP_OUT11_Pin GPIO_PIN_11
#define TAMP_OUT11_GPIO_Port GPIOB
#define COL6_Pin GPIO_PIN_12
#define COL6_GPIO_Port GPIOB
#define COL5_Pin GPIO_PIN_13
#define COL5_GPIO_Port GPIOB
#define COL4_Pin GPIO_PIN_14
#define COL4_GPIO_Port GPIOB
#define COL2_Pin GPIO_PIN_15
#define COL2_GPIO_Port GPIOB
#define COL3_Pin GPIO_PIN_8
#define COL3_GPIO_Port GPIOA
#define COL0_Pin GPIO_PIN_9
#define COL0_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_10
#define COL1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOA
#define COL10_Pin GPIO_PIN_4
#define COL10_GPIO_Port GPIOB
#define COL9_Pin GPIO_PIN_5
#define COL9_GPIO_Port GPIOB
#define COL8_Pin GPIO_PIN_9
#define COL8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
