#ifndef DHT11_H_
#define DHT11_H_

#include "stm32f4xx_hal.h"

/**
 * @file    dht11.h
 * @author  Ted Wang
 * @date    2025-08-07
 * @brief   DHT11 sensor driver API header for STM32
 *
 * @details
 * This header provides the API for reading temperature and humidity from a DHT11 sensor
 * using GPIO bit-banging. Timing is based on TIM2. All functions are blocking.
 */

/**
 * @brief Read temperature and humidity from DHT11 sensor (float version)
 *
 * This function reads the DHT11 sensor and returns the latest humidity and temperature values.
 *
 * @param[out] humidity Pointer to float for humidity (%RH)
 * @param[out] temperature Pointer to float for temperature (°C)
 * @retval 0 on success, -1 on failure (checksum or no response)
 *
 * @note This function blocks for ~25ms. Data is valid only if return value is 0.
 */
int8_t DHT11_ReadData(float *humidity, float *temperature);

#endif // DHT11_H_