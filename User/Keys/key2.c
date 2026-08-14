/*
 * key2.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"
#include "buzzer_task.h"

static KeyHandle_TypeDef hKey2;
static void key2_ClickedCallback(void);

void key2_Init(void)
{
    hKey2.GPIO_Port = KEY2_GPIO_Port;
    hKey2.GPIO_Pin = KEY2_Pin;
    hKey2.ClickedCallback = key2_ClickedCallback;
    key_init(&hKey2);
}

void key2_Scan(void)
{
    key_scan(&hKey2);
}

static void key2_ClickedCallback(void)
{
    Buzzer_Beep();
}

