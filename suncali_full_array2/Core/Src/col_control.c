/*
 * col_control.c
 *
 *  Created on: Dec 2, 2024
 *      Author: tjaz
 */
#include "col_control.h"
#include "usbd_cdc_if.h"


void OutputColumnData(const char* columnName, uint16_t* adcBuf1, uint16_t* adcBuf2, uint16_t* adcBuf3, GPIO_TypeDef* colPort, uint16_t colPin, GPIO_TypeDef* ledPort, uint16_t ledPin, volatile uint8_t* flag1, volatile uint8_t* flag2, volatile uint8_t* flag3) {
    char uart_buffer[128]; // Adjust size as needed

    // Transmit the ADC data
    snprintf(uart_buffer, sizeof(uart_buffer), "%s: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u \r\n",
             columnName,
             adcBuf1[0], adcBuf1[1], adcBuf1[2], adcBuf1[3], adcBuf2[0], adcBuf2[1], adcBuf2[2],
             adcBuf2[3], adcBuf3[0], adcBuf3[1], adcBuf2[4], adcBuf1[4]);
    CDC_Transmit_FS((uint8_t*)uart_buffer, strlen(uart_buffer));

    // Reset the GPIO pins
    HAL_GPIO_WritePin(colPort, colPin, GPIO_PIN_RESET); // Turn off column
    HAL_GPIO_WritePin(ledPort, ledPin, GPIO_PIN_RESET); // Turn off LED
    HAL_Delay(2);

    // Reset the flags
    *flag1 = 0;
    *flag2 = 0;
    *flag3 = 0;
}

void StartColumn(GPIO_TypeDef* colPort, uint16_t colPin, GPIO_TypeDef* ledPort, uint16_t ledPin, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, ADC_HandleTypeDef* hadc3, uint16_t* adcBuf1, uint16_t* adcBuf2, uint16_t* adcBuf3) {
    HAL_GPIO_WritePin(colPort, colPin, GPIO_PIN_SET); // Turn on column
    HAL_GPIO_WritePin(ledPort, ledPin, GPIO_PIN_SET); // Turn on LED
    HAL_Delay(2);

    HAL_ADC_Start_DMA(hadc1, (uint32_t *)adcBuf1, 5);
    HAL_ADC_Start_DMA(hadc2, (uint32_t *)adcBuf2, 5);
    HAL_ADC_Start_DMA(hadc3, (uint32_t *)adcBuf3, 2);
}

void StartColumnOffset(GPIO_TypeDef* colPort, uint16_t colPin, GPIO_TypeDef* ledPort, uint16_t ledPin, ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, ADC_HandleTypeDef* hadc3, uint16_t* adcBuf1, uint16_t* adcBuf2, uint16_t* adcBuf3) {
    HAL_GPIO_WritePin(colPort, colPin, GPIO_PIN_RESET); // Turn on column
    HAL_GPIO_WritePin(ledPort, ledPin, GPIO_PIN_SET); // Turn on LED
    HAL_Delay(2);

    HAL_ADC_Start_DMA(hadc1, (uint32_t *)adcBuf1, 5);
    HAL_ADC_Start_DMA(hadc2, (uint32_t *)adcBuf2, 5);
    HAL_ADC_Start_DMA(hadc3, (uint32_t *)adcBuf3, 2);
}

