/*
 * key_down.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#include "key.h"
#include "string.h"
#include "buzzer_task.h"

static KeyHandle_TypeDef hKeyDown;
static void keyDown_ClickedCallback(void);

void keyDown_Init(void)
{
    hKeyDown.GPIO_Port = KEY_DOWN_GPIO_Port;
    hKeyDown.GPIO_Pin = KEY_DOWN_Pin;
    hKeyDown.ClickedCallback = keyDown_ClickedCallback;
    key_init(&hKeyDown);
}

void keyDown_Scan(void)
{
    key_scan(&hKeyDown);
}

static void keyDown_ClickedCallback(void)
{
    Buzzer_Beep();
}

