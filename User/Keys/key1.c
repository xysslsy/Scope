/*
 * key1.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

 #include "key.h"
 #include "string.h"

 static KeyHandle_TypeDef hKey1;
 static void key1_ClickedCallback(void);

void key1_Init(void)
{
    hKey1.GPIO_Port = KEY1_GPIO_Port;
    hKey1.GPIO_Pin = KEY1_Pin;
    hKey1.ClickedCallback = key1_ClickedCallback;
    key_init(&hKey1);
}

void key1_Scan(void)
{
    key_scan(&hKey1);
}

 static void key1_ClickedCallback(void)
{
    extern UART_HandleTypeDef huart1;

    HAL_UART_Transmit(&huart1, (uint8_t *)"Key Pressed!\r\n", strlen("Key1 Pressed!\r\n"), HAL_MAX_DELAY);
}