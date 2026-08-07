/*
 * key_task.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

 #include "FreeRTOS.h"
 #include "task.h"
 #include "key.h"
 #include "main.h"
 #include "string.h"

 static KeyHandle_TypeDef hKey1;

 static void key1_ClickedCallback(void);

void keyTask(void *pvParameters)
{
    hKey1.GPIO_Port = KEY1_GPIO_Port;
    hKey1.GPIO_Pin = KEY1_Pin;
    hKey1.ClickedCallback = key1_ClickedCallback;

   key_init(&hKey1);

    for(;;)
    {
        key_scan(&hKey1);

        vTaskDelay(pdMS_TO_TICKS(10)); // Debounce delay
    }
}

static void key1_ClickedCallback(void)
{
    extern UART_HandleTypeDef huart1;

    HAL_UART_Transmit(&huart1, (uint8_t *)"Key Pressed!\r\n", strlen("Key Pressed!\r\n"), HAL_MAX_DELAY);
}
