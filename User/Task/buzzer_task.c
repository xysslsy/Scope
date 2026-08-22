/*
 * buzzer_test.c
 *
 *  Created on: Aug 14, 2026
 *      Author: lqtghm
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"

static SemaphoreHandle_t xBuzzerSemphr;

void Buzzer_Init()
{
    xBuzzerSemphr = xSemaphoreCreateBinary();
}

void vBuzzerTask(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreTake(xBuzzerSemphr, portMAX_DELAY);

        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(100));
        HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
    }
}

void Buzzer_Beep()
{
    xSemaphoreGive(xBuzzerSemphr);
}
