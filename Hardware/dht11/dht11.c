

/**
 * @file    dht11.c
 * @author  Ted Wang
 * @date    2025-08-07
 * @brief   DHT11 sensor low-level driver for STM32 (bare-metal or RTOS)
 *
 * @details
 * This file provides GPIO-based routines for DHT11 temperature/humidity sensor communication.
 * It includes initialization, start signal, data read, and CRC check. Timing is based on TIM2.
 */

#include "gpio.h"
#include "main.h"
#include "tim.h"

/**
 * @brief Delay for a specified number of microseconds using TIM2.
 * @param us Number of microseconds to delay.
 */
static void DHT11_DelayUs(uint32_t us) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	__HAL_TIM_ENABLE(&htim2);
	while(__HAL_TIM_GET_COUNTER(&htim2) < us);
	__HAL_TIM_DISABLE(&htim2);
}

/**
 * @brief Configure the DHT11 data pin as open-drain output.
 * @param GPIOx GPIO port
 * @param GPIO_Pin GPIO pin
 */
static void DHT11_SetPinOutput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief Configure the DHT11 data pin as input with pull-up.
 * @param GPIOx GPIO port
 * @param GPIO_Pin GPIO pin
 */
static void DHT11_SetPinInput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/**
 * @brief Send start signal and check DHT11 response.
 * @retval 1 if DHT11 responds, 0 otherwise
 *
 * @note This function implements the DHT11 start sequence and waits for the sensor's response.
 */
static uint8_t DHT11_CheckResponse(void) {
	uint8_t timer = 0;
	DHT11_SetPinOutput(DHT11_GPIO_Port, DHT11_Pin);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
	DHT11_DelayUs(20000);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
	DHT11_DelayUs(30);
	DHT11_SetPinInput(DHT11_GPIO_Port, DHT11_Pin);

	while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
		timer++;
		DHT11_DelayUs(1);
		if(timer > 100) return 0;
	}
	timer = 0;
	while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
		timer++;
		DHT11_DelayUs(1);
		if(timer > 100) return 0;
	}
	return 1;
}

/**
 * @brief Read one byte from DHT11 sensor.
 * @retval 8-bit data
 *
 * @note This function reads a single 8-bit value from the DHT11 data line.
 */
static uint8_t DHT11_ReadByte(void) {
	uint8_t byte = 0;
	for(int i = 0; i < 8; i++) {
		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin));
		while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin));
		DHT11_DelayUs(40);
		byte <<= 1;
		if(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) {
			byte |= 0x01;
		}
	}
	return byte;
}

/**
 * @brief Read temperature and humidity from DHT11 sensor.
 *
 * @param[out] humidity Pointer to float for humidity (%RH)
 * @param[out] temperature Pointer to float for temperature (°C)
 * @retval 0 on success, -1 on failure (checksum or no response)
 *
 * @note This function blocks for ~25ms. Data is valid only if return value is 0.
 */
int8_t DHT11_ReadData(float *humidity, float *temperature) {
	int8_t status = -1;
	uint8_t buffer[5] = {0};
	if(DHT11_CheckResponse()) {
		for(int i = 0; i < 5; i++) {
			buffer[i] = DHT11_ReadByte();
		}
		if(buffer[0] + buffer[1] + buffer[2] + buffer[3] == buffer[4]) {
			*humidity = buffer[0] + buffer[1] * 0.1f;
			*temperature = buffer[2] + buffer[3] * 0.1f;
			status = 0;
		} else {
			*humidity = 99;
			*temperature = 99;
			status = -1;
		}
	} else {
		*humidity = 99;
		*temperature = 99;
		status = -1;
	}
	return status;
}
