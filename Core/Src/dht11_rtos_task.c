

/**
 * @file    dht11_rtos_task.c
 * @author  Ted Wang
 * @date    2025-08-01
 * @brief   RTOS task for DHT11 sensor data acquisition (CMSIS-RTOS v2)
 *
 * @details
 * Implements a FreeRTOS/CMSIS-RTOS v2 task for periodic DHT11 temperature/humidity acquisition.
 * The task reads the DHT11 sensor, sends the result to a message queue for display, and outputs debug info via UART.
 */


#include "dht11_rtos_task.h"
#include "oled_rtos_task.h"
#include "dht11.h"
#include "main.h"
#include "oled_driver.h"
#include <string.h>
#include <stdio.h>


/**
 * @brief Global instance for latest DHT11 sensor data.
 */
DHT11_Data_t g_dht11_data;

/**
 * @brief DHT11 RTOS task handle.
 */
static osThreadId_t dht11_task_handle;

/**
 * @brief UART3 handle for debug/error output (defined elsewhere).
 */
extern UART_HandleTypeDef huart3;

/**
 * @brief DHT11 RTOS task main loop (thread entry point).
 * @param argument Unused (required by CMSIS-RTOS API)
 */
static void DHT11_Task(void *argument);





/**
 * @brief  Initialize the DHT11 RTOS task.
 *
 * This function creates the DHT11 acquisition task. Call once during system initialization before the RTOS kernel starts.
 *
 * @note If task creation fails, outputs error via UART3 and calls Error_Handler().
 */
void DHT11_Task_Init(void)
{
    const osThreadAttr_t dht11_task_attributes = {
        .name = DHT11_TASK_THREAD_NAME,
        .priority = DHT11_TASK_THREAD_PRIORITY,
        .stack_size = DHT11_TASK_STACK_SIZE_BYTES
    };
    dht11_task_handle = osThreadNew(DHT11_Task, NULL, &dht11_task_attributes);
    if (dht11_task_handle == NULL)
    {
        char msg[] = "Failed to create DHT11 task\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }
}



/**
 * @brief DHT11 RTOS task main loop.
 *
 * Periodically reads DHT11 sensor, sends data to display queue, and outputs debug info via UART.
 *
 * @param argument Unused (required by CMSIS-RTOS API)
 */
static void DHT11_Task(void *argument)
{
    while (1)
    {
        HAL_GPIO_WritePin(LED_PB14_GPIO_Port, LED_PB14_Pin, GPIO_PIN_SET);
        if (DHT11_ReadData(&g_dht11_data.humidity, &g_dht11_data.temperature) == 0) {
            osMessageQueuePut(display_dht11_info_queue, &g_dht11_data, 0, 0);
            char msg[40];
            snprintf(msg, sizeof(msg), "Temp:%.1f C\r\nHumi:%.1f %%\r\n", g_dht11_data.temperature, g_dht11_data.humidity);
            HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        } else {
            char msg[] = "DHT11_ReadData failed\r\n";
            HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        }
        HAL_GPIO_WritePin(LED_PB14_GPIO_Port, LED_PB14_Pin, GPIO_PIN_RESET);
        osDelay(3000);
    }
}
