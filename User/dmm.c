#include "dmm.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"

static QueueHandle_t xMailboxForDMM; //QueueHandle_t是一个指向QueueDefinition结构体的指针类型，xMailboxForDMM是一个队列句柄，用于在FreeRTOS中管理队列。
extern ADC_HandleTypeDef hadc1; //extern关键字用于声明一个在其他文件中定义的全局变量。这里声明了一个名为hadc1的ADC_HandleTypeDef类型的变量，表示ADC1的句柄。
extern TIM_HandleTypeDef htim2; //extern关键字用于声明一个在其他文件中定义的全局变量。这里声明了一个名为htim2的TIM_HandleTypeDef类型的变量，表示定时器2的句柄。

//
// @作用: 对DMM模块进行初始化
//
void DMM_Init(void)
{
    xMailboxForDMM = xQueueCreate(1, sizeof(DMMData_t));

    HAL_ADCEx_Calibration_Start(&hadc1); //HAL_ADCEx_Calibration_Start函数用于启动ADC的校准过程。它的参数是一个指向ADC_HandleTypeDef结构体的指针，这里传入了hadc1，表示对ADC1进行校准。
    HAL_ADCEx_InjectedStart_IT(&hadc1); //HAL_ADCEx_InjectedStart_IT函数用于启动ADC的注入通道转换，并启用中断。它的参数是一个指向ADC_HandleTypeDef结构体的指针，这里传入了hadc1，表示对ADC1的注入通道进行转换。
    HAL_TIM_Base_Start(&htim2); //HAL_TIM_Base_Start函数用于启动定时器的基本计数功能。它的参数是一个指向TIM_HandleTypeDef结构体的指针，这里传入了htim2，表示启动定时器2。

}

//
// @作用: 获取DMM模块的测量结果
//
DMMData_t DMM_GetData(void)
{
    DMMData_t data;
    xQueuePeek(xMailboxForDMM, &data, 0); //xQueuePeek函数用于从队列中获取数据，但不会将其从队列中移除。它的参数包括队列句柄、指向接收数据的缓冲区的指针以及等待时间（以滴答计数为单位）。在这里，等待时间为0，表示如果队列为空，则立即返回而不阻塞。
    return data;
}

void  HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        DMMData_t data;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE; //BaseType_t是一个长整型类型，xHigherPriorityTaskWoken是一个变量，用于指示在中断服务例程中是否需要进行上下文切换。它的初始值为pdFALSE，表示不需要进行上下文切换。

        float Vadc = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1)/4095.0f * 3.3f; //获取ADC的注入通道转换结果，返回值为浮点数类型，表示电压值。
    
        data.Range = 0; //设置测量范围为电压档50V
        data.Value = Vadc;

        xQueueOverwriteFromISR(xMailboxForDMM, &data, &xHigherPriorityTaskWoken); //xQueueOverwriteFromISR函数用于在中断服务例程中将数据写入队列，如果队列已满，则覆盖队列中的现有数据。它的参数包括队列句柄、指向要写入的数据的指针以及一个指向变量的指针，用于指示是否需要进行上下文切换。在这里，&data是指向DMMData_t结构体的指针，表示要写入队列的数据。
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //portYIELD_FROM_ISR宏用于在中断服务例程中触发上下文切换。如果xHigherPriorityTaskWoken为pdTRUE，表示有更高优先级的任务需要运行，则触发上下文切换。
    }
}

