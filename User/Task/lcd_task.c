/*
 * lcd_task.c
 *
 *  Created on: Aug 22, 2026
 *      Author: lqtghm
 */

 #include "main.h"
 #include "FreeRTOS.h"
 #include "task.h"
 #include "semphr.h"
 #include "lcd.h"
 #include "dmm_label.h"

 static SemaphoreHandle_t xSemaphrForDMA;

static void BacklightControl(uint8_t on);
static void Reset(void);
static void SendCommand(uint8_t cmd);
static void SendData(uint8_t *pData, uint16_t Size);

void LCDTask_Init(void)
{
    xSemaphrForDMA = xSemaphoreCreateBinary();

    LCD_InitTypeDef LCD_InitStruct = {0};

    LCD_InitStruct.back_light_control_callback = BacklightControl;
    LCD_InitStruct.reset_callback = Reset;
    LCD_InitStruct.send_command_callback = SendCommand;
    LCD_InitStruct.send_data_callback = SendData;

    lcd_init(&LCD_InitStruct);
}

//
//@简介: LCD任务函数
//
void vLCDTask(void *pvParameters)
{
    lcd_clear(); // 清屏

    for(;;)
    {
        DMMLable1_Repaint(); // 重绘DMM标签

        vTaskDelay(pdMS_TO_TICKS(2000)); // 延时2秒
    }
}

//
//@简介: 控制背光开关
//@参数: on: 1-开, 0-关
//
static void BacklightControl(uint8_t on)
{
    if(on)
    {
        // 打开背光
        HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
    }
    else
    {
        // 关闭背光
        HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
    }
}

//
//@简介: 复位LCD
//
static void Reset(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
}

//
//@简介: 发送命令到LCD
//@参数: cmd: 命令字节
//
static void SendCommand(uint8_t cmd)
{
    extern SPI_HandleTypeDef hspi1; // 声明外部SPI句柄

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET); // 设置为命令模式
    HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_RESET); // 选中LCD
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY); // 发送命令
    HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_SET); // 取消选中LCD
}

//
//@简介: 发送数据到LCD
//@参数: pData: 数据缓冲区指针
//@参数: Size: 数据大小
//
static void SendData(uint8_t *pData, uint16_t Size)
{
    extern SPI_HandleTypeDef hspi1; // 声明外部SPI句柄

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET); // 设置为数据模式
    HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_RESET); // 选中LCD
    // HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY); // 发送数据
    HAL_SPI_Transmit_DMA(&hspi1, pData, Size); // 使用DMA发送数据
    xSemaphoreTake(xSemaphrForDMA, portMAX_DELAY); // 等待DMA传输完成
    HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_SET); // 取消选中LCD
}

//
//@简介: SPI传输完成回调函数
//@参数: hspi: SPI句柄指针
//
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1) // 检查是否为SPI1的回调
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR(xSemaphrForDMA, &xHigherPriorityTaskWoken); // 释放信号量
        
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // 如果需要，切换任务
    }
}
