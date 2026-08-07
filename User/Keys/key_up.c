/*
 * key_up.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"

static KeyHandle_TypeDef hKeyUp;
static void keyUp_ClickedCallback(void);

void keyUp_Init(void)
{
    hKeyUp.GPIO_Port = KEY_UP_GPIO_Port;
    hKeyUp.GPIO_Pin = KEY_UP_Pin;
    hKeyUp.ClickedCallback = keyUp_ClickedCallback;
    key_init(&hKeyUp);
}

void keyUp_Scan(void)
{
    key_scan(&hKeyUp);
}

static void keyUp_ClickedCallback(void)
{
    extern UART_HandleTypeDef huart1;

    HAL_UART_Transmit(&huart1, (uint8_t *)"Key Up Pressed!\r\n", strlen("Key Up Pressed!\r\n"), HAL_MAX_DELAY);
}
