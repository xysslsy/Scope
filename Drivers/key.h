/*
 * key.h
 *
 *  Created on: Aug 7, 2026
 *      Author: lqtghm
 */

#ifndef KEY_H_
#define KEY_H_

#include "main.h"

typedef struct
{
    GPIO_PinState Previous;
    GPIO_PinState Current;

    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;

    void(*ClickedCallback)(void);

} KeyHandle_TypeDef;

void key_init(KeyHandle_TypeDef *Handle);
void key_scan(KeyHandle_TypeDef *Handle);

#endif /* KEY_H_ */
