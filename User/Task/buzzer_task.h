/*
 * buzzer_test.h
 *
 *  Created on: Aug 14, 2026
 *      Author: lqtghm
 */

#ifndef TASK_BUZZER_TASK_H_
#define TASK_BUZZER_TASK_H_

void Buzzer_Init();
void BuzzerTask(void *pvParameters);
void Buzzer_Beep();

#endif /* TASK_BUZZER_TASK_H_ */
