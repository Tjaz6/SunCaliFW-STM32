/*
 * col_control.h
 *
 *  Created on: Dec 2, 2024
 *      Author: tjaz
 */

#ifndef INC_COL_CONTROL_H_
#define INC_COL_CONTROL_H_


#include "stm32g4xx_hal.h" // Adjust according to your STM32 series
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Function Declarations
void StartColumn(GPIO_TypeDef* colPort, uint16_t colPin, GPIO_TypeDef* ledPort, uint16_t ledPin,
                 ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, ADC_HandleTypeDef* hadc3,
                 uint16_t* adcBuf1, uint16_t* adcBuf2, uint16_t* adcBuf3);

void OutputColumnData(const char* columnName, uint16_t* adcBuf1, uint16_t* adcBuf2, uint16_t* adcBuf3,
                      GPIO_TypeDef* colPort, uint16_t colPin, GPIO_TypeDef* ledPort, uint16_t ledPin,
					  volatile uint8_t* flag1, volatile uint8_t* flag2, volatile uint8_t* flag3);


#endif /* INC_COL_CONTROL_H_ */
