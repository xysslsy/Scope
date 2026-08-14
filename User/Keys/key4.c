/*
 * key4.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"
#include "buzzer_task.h"

static KeyHandle_TypeDef hKey4;
static void key4_ClickedCallback(void);

void key4_Init(void)
{
    hKey4.GPIO_Port = KEY4_GPIO_Port;
    hKey4.GPIO_Pin = KEY4_Pin;
    hKey4.ClickedCallback = key4_ClickedCallback;
    key_init(&hKey4);
}

void key4_Scan(void)
{
    key_scan(&hKey4);
}

static void key4_ClickedCallback(void)
{
    Buzzer_Beep();
}

