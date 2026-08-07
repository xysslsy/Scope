/*
 * key3.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"

static KeyHandle_TypeDef hKey3;
static void key3_ClickedCallback(void);
void key3_Init(void)
{
    hKey3.GPIO_Port = KEY3_GPIO_Port;
    hKey3.GPIO_Pin = KEY3_Pin;
    hKey3.ClickedCallback = key3_ClickedCallback;
    key_init(&hKey3);
}
void key3_Scan(void)
{
    key_scan(&hKey3);
}

static void key3_ClickedCallback(void)
{
    extern UART_HandleTypeDef huart1;

    HAL_UART_Transmit(&huart1, (uint8_t *)"Key3 Pressed!\r\n", strlen("Key3 Pressed!\r\n"), HAL_MAX_DELAY);
}

