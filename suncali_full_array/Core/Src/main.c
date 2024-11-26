/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "usbd_cdc_if.h"

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */


uint16_t adc_buffer[5];
uint16_t adc_buffer2[5];
uint16_t adc_buffer3[2];

volatile uint8_t flag = 0;
volatile uint8_t flag2 = 0;
volatile uint8_t flag3 = 0;

uint8_t state = 0;

char uart_buffer[93];

volatile uint8_t run_main_loop = 0; // 0 = stop, 1 = start
volatile uint8_t run_offset = 0;





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_USB_Device_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */

  // Calibrate ADC
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);

  HAL_TIM_Base_Start_IT(&htim1);


  // Set all COL to 0
  HAL_GPIO_WritePin(COL0_GPIO_Port, COL0_Pin, 0);
  HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, 0);
  HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, 0);
  HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, 0);
  HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, 0);
  HAL_GPIO_WritePin(COL5_GPIO_Port, COL5_Pin, 0);
  HAL_GPIO_WritePin(COL6_GPIO_Port, COL6_Pin, 0);
  HAL_GPIO_WritePin(COL7_GPIO_Port, COL7_Pin, 0);
  HAL_GPIO_WritePin(COL8_GPIO_Port, COL8_Pin, 0);
  HAL_GPIO_WritePin(COL9_GPIO_Port, COL9_Pin, 0);
  HAL_GPIO_WritePin(COL10_GPIO_Port, COL10_Pin, 0);
  HAL_GPIO_WritePin(COL11_GPIO_Port, COL11_Pin, 0);
  HAL_GPIO_WritePin(COL12_GPIO_Port, COL12_Pin, 0);

  HAL_Delay(10);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

	  switch (state){
	  case 0:
		  if (run_main_loop == 1){
			  state = 1;
		  }

		  if(run_offset == 1 ){
			  HAL_GPIO_WritePin(COL0_GPIO_Port, COL0_Pin, 0);
			  HAL_GPIO_WritePin(COL12_GPIO_Port, COL12_Pin, 0);
			  HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, 0);
			  HAL_Delay(2);
			  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
			  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
			  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
			  state = 50;
		  }

		  break;

	  case 1: //COL0 start
		  HAL_GPIO_WritePin(COL0_GPIO_Port, COL0_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 2;
		  break;

	  case 2: //COL0 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL0: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL0_GPIO_Port, COL0_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 3;
		  }
		  break;

	  case 3: //COL1 start
		  HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 4;
		  break;

	  case 4: //COL1 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL1: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 5;
		  }
		  break;

	  case 5: //COL2 start
		  HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 6;
		  break;

	  case 6: //COL2 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL2: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 7;
		  }
		  break;

	  case 7: //COL3 start
		  HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 8;
		  break;

	  case 8: //COL3 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL3: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 9;
		  }
		  break;

	  case 9: //COL4 start
		  HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 10;
		  break;

	  case 10: //COL4 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL4: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 11;
		  }
		  break;

	  case 11: //COL5 start
		  HAL_GPIO_WritePin(COL5_GPIO_Port, COL5_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 12;
		  break;

	  case 12: //COL5 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL5: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL5_GPIO_Port, COL5_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 13;
		  }
		  break;

	  case 13: //COL6 start
		  HAL_GPIO_WritePin(COL6_GPIO_Port, COL6_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 14;
		  break;

	  case 14: //COL6 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL6: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL6_GPIO_Port, COL6_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 15;
		  }
		  break;

	  case 15: //COL7 start
		  HAL_GPIO_WritePin(COL7_GPIO_Port, COL7_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 16;
		  break;

	  case 16: //COL7 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL7: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL7_GPIO_Port, COL7_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 17;
		  }
		  break;

	  case 17: //COL8 start
		  HAL_GPIO_WritePin(COL8_GPIO_Port, COL8_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 18;
		  break;

	  case 18: //COL8 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL8: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL8_GPIO_Port, COL8_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 19;
		  }
		  break;

	  case 19: //COL9 start
		  HAL_GPIO_WritePin(COL9_GPIO_Port, COL9_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state =20;
		  break;

	  case 20: //COL9 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL9: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL9_GPIO_Port, COL9_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 21;
		  }
		  break;

	  case 21: //COL10 start
		  HAL_GPIO_WritePin(COL10_GPIO_Port, COL10_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 22;
		  break;

	  case 22: //COL10 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL10: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL10_GPIO_Port, COL10_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 23;
		  }
		  break;

	  case 23: //COL11 start
		  HAL_GPIO_WritePin(COL11_GPIO_Port, COL11_Pin, 1);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		  HAL_Delay(2);

		  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 5);
		  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer2, 5);
		  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_buffer3, 2);
		  state = 24;
		  break;

	  case 24: //COL11 output data
		  if(flag == 1 && flag2 == 1 && flag3 == 1){
			  snprintf(uart_buffer, sizeof(uart_buffer), "COL11: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer[3],
					  adc_buffer2[0], adc_buffer2[1], adc_buffer2[2], adc_buffer2[3], adc_buffer3[0], adc_buffer3[1], adc_buffer2[4], adc_buffer[4]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  flag3 = 0;

			  HAL_GPIO_WritePin(COL11_GPIO_Port, COL11_Pin, 0);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			  HAL_Delay(2);
			  state = 25;
		  }
		  break;

	  case 25:
		  if(run_main_loop == 0){
			  CDC_Transmit_FS((uint8_t*)"Main loop stopped\r\n", 19);
		  }
		  state = 0;
		  break;

	  case 50:
		  if(flag == 1 && flag2 == 1){

			  snprintf(uart_buffer, sizeof(uart_buffer), "Offsets: %u, %u, %u, %u, %u\r\n", adc_buffer[0], adc_buffer[1], adc_buffer[2], adc_buffer2[0], adc_buffer2[1]);
			  CDC_Transmit_FS((uint8_t *)uart_buffer, strlen(uart_buffer));

			  flag = 0;
			  flag2 = 0;
			  state = 0;
		  }

		  break;

	  default:
		  state = 0;

	  }

	  HAL_Delay(10);


  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 18;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 5;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = ENABLE;
  hadc1.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_16;
  hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
  hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.GainCompensation = 0;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 5;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.OversamplingMode = ENABLE;
  hadc2.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_16;
  hadc2.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
  hadc2.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc2.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_17;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.GainCompensation = 0;
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 2;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = ENABLE;
  hadc3.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_16;
  hadc3.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
  hadc3.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc3.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 14400-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
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
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, COL12_Pin|COL11_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, COL7_Pin|COL6_Pin|COL5_Pin|COL4_Pin
                          |COL2_Pin|COL10_Pin|COL9_Pin|COL8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, COL3_Pin|COL0_Pin|COL1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : COL12_Pin COL11_Pin LED1_Pin */
  GPIO_InitStruct.Pin = COL12_Pin|COL11_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : COL7_Pin COL6_Pin COL5_Pin COL4_Pin
                           COL2_Pin COL10_Pin COL9_Pin COL8_Pin */
  GPIO_InitStruct.Pin = COL7_Pin|COL6_Pin|COL5_Pin|COL4_Pin
                          |COL2_Pin|COL10_Pin|COL9_Pin|COL8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : COL3_Pin COL0_Pin COL1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = COL3_Pin|COL0_Pin|COL1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM1){
    	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1){
    	flag = 1;
    }

    if (hadc->Instance == ADC2){
		flag2 = 1;
	}

    if (hadc->Instance == ADC3){
    		flag3 = 1;
    	}
}


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
