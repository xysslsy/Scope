/*
 * key.c
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

 #include "main.h"
 #include "string.h"
 #include "key.h"

void key_init(KeyHandle_TypeDef *Handle)
{
    Handle->Previous = GPIO_PIN_SET;
    Handle->Current = GPIO_PIN_SET;
}

void key_scan(KeyHandle_TypeDef *Handle)
{
    Handle->Current = HAL_GPIO_ReadPin(Handle->GPIO_Port, Handle->GPIO_Pin);
        
        if (Handle->Previous == GPIO_PIN_RESET && Handle->Current == GPIO_PIN_SET)
        {
            Handle->ClickedCallback();
        }
        Handle->Previous = Handle->Current;
}
