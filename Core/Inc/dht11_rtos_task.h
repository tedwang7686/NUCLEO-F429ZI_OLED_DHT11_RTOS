

/**
 * @file    dht11_rtos_task.h
 * @author  Ted Wang
 * @date    2025-08-01
 * @brief   RTOS task and data structure definitions for DHT11 sensor (NUCLEO-F429ZI).
 *
 * @details
 * Provides type definitions, configuration macros, and API prototypes for the DHT11 RTOS task
 * using CMSIS-RTOS v2. This header enables modular, maintainable, and well-documented code for
 * periodic DHT11 sensor acquisition and inter-task communication.
 */


#ifndef DHT11_RTOS_TASK_H
#define DHT11_RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os2.h"

/* Exported constants --------------------------------------------------------*/
/**
 * @def DHT11_TASK_STACK_SIZE_BYTES
 * @brief Stack size (in bytes) for the DHT11 RTOS task.
 */
#define DHT11_TASK_STACK_SIZE_BYTES      (512 * 4)

/**
 * @def DHT11_TASK_THREAD_NAME
 * @brief Name of the DHT11 RTOS task (for debugging/RTOS awareness).
 */
#define DHT11_TASK_THREAD_NAME           "DHT11_Task"

/**
 * @def DHT11_TASK_THREAD_PRIORITY
 * @brief Priority of the DHT11 RTOS task.
 */
#define DHT11_TASK_THREAD_PRIORITY       osPriorityAboveNormal

/**
 * @def DHT11_QUEUE_SIZE
 * @brief Message queue size for DHT11 data updates to display task.
 */
#define DHT11_QUEUE_SIZE                 3

/* Exported types ------------------------------------------------------------*/
/**
 * @brief DHT11 sensor data structure (temperature and humidity).
 *
 * Holds the latest temperature (Celsius) and humidity (%) readings from the DHT11 sensor.
 */
typedef struct {
    float temperature; /**< Temperature in Celsius */
    float humidity;    /**< Relative humidity in percent */
} DHT11_Data_t;

/* Exported variables --------------------------------------------------------*/
/**
 * @brief Global instance for latest DHT11 sensor data.
 */
extern DHT11_Data_t g_dht11_data;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Initialize the DHT11 RTOS task and its message queue.
 *
 * Configures and creates the DHT11 acquisition task and its associated message queue
 * using CMSIS-RTOS v2 API. The task periodically samples the DHT11 sensor and posts
 * results to the queue for other tasks (e.g., display). Call once during system initialization.
 */
void DHT11_Task_Init(void);

#ifdef __cplusplus
}
#endif

#endif // DHT11_RTOS_TASK_H
