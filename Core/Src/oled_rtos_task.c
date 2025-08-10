

/**
 * @file    oled_rtos_task.c
 * @author  Ted Wang
 * @date    2025-08-01
 * @brief   RTOS task for OLED display control (CMSIS-RTOS v2)
 *
 * @details
 * Implements a FreeRTOS/CMSIS-RTOS v2 task for OLED display update using u8g2 library.
 * The task receives DHT11 sensor data from a message queue and updates the display accordingly.
 */

/* Includes ------------------------------------------------------------------*/

#include "oled_rtos_task.h"
#include "dht11_rtos_task.h"
#include "main.h"
#include "oled_driver.h"
#include <string.h>
#include <stdio.h>



/**
 * @brief OLED RTOS task handle.
 */
static osThreadId_t oled_task_handle;

/**
 * @brief Queue handle for DHT11 info updates.
 */
osMessageQueueId_t display_dht11_info_queue;

/**
 * @brief UART3 handle for debug/error output (defined elsewhere).
 */
extern UART_HandleTypeDef huart3;



/**
 * @brief OLED RTOS display task function (thread entry point).
 * @param argument Unused (required by CMSIS-RTOS API)
 */
static void OLED_Display_Task(void *argument);





/**
 * @brief  Initialize the OLED display RTOS task and message queue.
 *
 * This function creates the message queue for DHT11 info updates and starts the OLED display task.
 * Call once during system initialization before the RTOS kernel starts.
 *
 * @note If queue or task creation fails, outputs error via UART3 and calls Error_Handler().
 */
void OLED_Task_Init(void)
{
    display_dht11_info_queue = osMessageQueueNew(DHT11_QUEUE_SIZE, sizeof(DHT11_Data_t), NULL);
    if (display_dht11_info_queue == NULL)
    {
        char msg[] = "Failed to create display DHT11 info queue\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }

    const osThreadAttr_t oled_task_attributes = {
        .name = OLED_TASK_THREAD_NAME,
        .priority = OLED_TASK_THREAD_PRIORITY,
        .stack_size = OLED_TASK_STACK_SIZE_BYTES
    };
    oled_task_handle = osThreadNew(OLED_Display_Task, NULL, &oled_task_attributes);
    if (oled_task_handle == NULL)
    {
        char msg[] = "Failed to create OLED display task\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }
}




/**
 * @brief OLED RTOS display task main loop.
 *
 * Waits for DHT11 data from the queue, then updates the OLED display and LED accordingly.
 *
 * @param argument Unused (required by CMSIS-RTOS API)
 */
static void OLED_Display_Task(void *argument)
{
    char displayTemp[20];
    char displayHumi[20];
    DHT11_Data_t dht11_data;
    OLED_Init();
    u8g2_t *u8g2 = OLED_GetDisplay();
    if (u8g2 == NULL)
    {
        char msg[] = "Failed to initialize OLED display\r\n";
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), 100);
        Error_Handler();
    }

    u8g2_ClearBuffer(u8g2);
    u8g2_ClearDisplay(u8g2);
    u8g2_SendBuffer(u8g2);
    u8g2_SetFont(u8g2, u8g2_font_ncenB08_tr);

    while (1) {
        // Block until new data arrives
        osStatus_t dht11Receive = osMessageQueueGet(display_dht11_info_queue, &dht11_data, NULL, osWaitForever);
        u8g2_ClearBuffer(u8g2);
        snprintf(displayTemp, sizeof(displayTemp), "Temp: %.1f C", dht11_data.temperature);
        snprintf(displayHumi, sizeof(displayHumi), "Humi: %.1f %%", dht11_data.humidity);
        u8g2_DrawStr(u8g2, 0, 20, displayTemp);
        u8g2_DrawStr(u8g2, 0, 40, displayHumi);
        // Show project name at the bottom line
        u8g2_DrawStr(u8g2, 0, 62, OLED_SHOW_PROJECT_NAME);
        if (dht11_data.humidity >= 60) {
            HAL_GPIO_WritePin(LED_PB7_GPIO_Port, LED_PB7_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LED_PB7_GPIO_Port, LED_PB7_Pin, GPIO_PIN_RESET);
        }
        u8g2_SendBuffer(u8g2);
        osDelay(100);
    }
}
