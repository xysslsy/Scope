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

 static SemaphoreHandle_t xSemaphrForDMA;

static void BacklightControl(uint8_t on);
static void Reset(void);
static void SendCommand(uint8_t cmd);
static void SendData(uint8_t *pData, uint16_t Size);
static void SetRowRange(uint16_t startRow, uint16_t endRow);
static void SetColumnRange(uint16_t startCol, uint16_t endCol);
static void ShowImage(const uint16_t *pImage, size_t Size);
static void ShowImage1(void);
static void ShowImage2(void);

void LCDTask_Init(void)
{
    xSemaphrForDMA = xSemaphoreCreateBinary();

    BacklightControl(1); // 打开背光
    Reset(); // 复位LCD
}

//
//@简介: LCD任务函数
//
void vLCDTask(void *pvParameters)
{
    uint8_t dataToSend[10];

    SendCommand(0x11); // 退出睡眠模式
    HAL_Delay(125); // 延时等待LCD稳定

    SendCommand(0x3a); // 设置像素格式

    dataToSend[0] = 0x55; // 16位颜色
    SendData(dataToSend, 1);

    SendCommand(0x36); // 设置扫描方向和颜色顺序
    dataToSend[0] = 0x80; // MY方向，RGB颜色顺序
    SendData(dataToSend, 1);

    SendCommand(0x29); // 打开显示

    for(;;)
    {
        ShowImage1(); // 显示图像1
        vTaskDelay(pdMS_TO_TICKS(2000)); // 延时2秒

        ShowImage2(); // 显示图像2
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

//
//@简介: 设置LCD的行范围
//@参数: startRow: 起始行
//@参数: endRow: 结束行
//
static void SetRowRange(uint16_t startRow, uint16_t endRow)
{
    uint8_t dataToSend[4];

    SendCommand(0x2b); // 设置行地址

    dataToSend[0] = (startRow >> 8) & 0xFF; // 高字节
    dataToSend[1] = startRow & 0xFF; // 低字节
    dataToSend[2] = (endRow >> 8) & 0xFF; // 高字节
    dataToSend[3] = endRow & 0xFF; // 低字节
    SendData(dataToSend, 4);
}

//
//@简介: 设置LCD的列范围
//@参数: startCol: 起始列
//@参数: endCol: 结束列
//
static void SetColumnRange(uint16_t startCol, uint16_t endCol)
{
    uint8_t dataToSend[4];

    SendCommand(0x2a); // 设置列地址

    dataToSend[0] = (startCol >> 8) & 0xFF; // 高字节
    dataToSend[1] = startCol & 0xFF; // 低字节
    dataToSend[2] = (endCol >> 8) & 0xFF; // 高字节
    dataToSend[3] = endCol & 0xFF; // 低字节
    SendData(dataToSend, 4);
}

//
//@简介: 显示图像到LCD
//@参数: pImage: 要显示的图像的数据
//@参数: Size: 图像数据大小
//
static void ShowImage(const uint16_t *pImage, size_t Size)
{
    SendCommand(0x2c); // 写内存开始
    SendData((uint8_t *)pImage, (uint16_t)(Size * sizeof(*pImage)));
}

#include "image1.h"

//
//@简介: 显示图像1
//
static void ShowImage1(void)
{
     SetRowRange(80, 239); // 设置行范围
    SetColumnRange(60, 193); // 设置列范围
     ShowImage(image1, sizeof(image1) / sizeof(image1[0])); // 显示图像1
}

#include "image2.h"

//
//@简介: 显示图像2
//
static void ShowImage2(void)
{
    SetRowRange(80, 239); // 设置行范围
    SetColumnRange(60, 221); // 设置列范围
    ShowImage(image2, sizeof(image2) / sizeof(image2[0])); // 显示图像2
}
