/*
 * key_up.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"
#include "buzzer_task.h"

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
    Buzzer_Beep();
}

