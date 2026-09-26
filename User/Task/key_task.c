/*
 * key_task.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

 #include "FreeRTOS.h"
 #include "task.h"
#include "main.h"
#include "key1.h"
#include "key2.h"
#include "key3.h"
#include "key4.h"
#include "key_up.h"
#include "key_down.h"

void keyTask(void *pvParameters)
{
    key1_Init();
    key2_Init();
    key3_Init();
    key4_Init();
    keyUp_Init();
    keyDown_Init();

    for(;;)
    {
        key1_Scan();
        key2_Scan();
        key3_Scan();
        key4_Scan();
        keyUp_Scan();
        keyDown_Scan();
        vTaskDelay(pdMS_TO_TICKS(10)); // Debounce delay
    }
}

