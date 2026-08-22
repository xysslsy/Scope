/*
 * lcd_task.c
 *
 *  Created on: Aug 22, 2026
 *      Author: lqtghm
 */

 #include "main.h"

static void BacklightControl(uint8_t on);
static void Reset(void);
static void SendCommand(uint8_t cmd);
static void SendData(uint8_t *pData, uint16_t Size);

void LCDTask_Init(void)
{
    BacklightControl(1); // 打开背光
    Reset(); // 复位LCD

    uint8_t dataToSend[10];

    SendCommand(0x11); // 退出睡眠模式
    HAL_Delay(125); // 延时等待LCD稳定

    SendCommand(0x3a); // 设置像素格式

    dataToSend[0] = 0x55; // 16位颜色
    SendData(dataToSend, 1);

    SendCommand(0x29); // 打开显示

    //设置行范围
    SendCommand(0x2b); // 设置行地址

    dataToSend[0] = 0x00; // 起始行高字节
    dataToSend[1] = 0x9b; // 起始行低字节
    dataToSend[2] = 0x00; // 结束行高字节
    dataToSend[3] = 0xa4; // 结束行低字节
    SendData(dataToSend, 4);

    //设置列范围
    SendCommand(0x2a); // 设置列地址
    dataToSend[0] = 0x00; // 起始列高字节
    dataToSend[1] = 0x73; // 起始列低字节
    dataToSend[2] = 0x00; // 结束列高字节
    dataToSend[3] = 0x7c; // 结束列低字节
    SendData(dataToSend, 4);

    //显示彩色小方块
    SendCommand(0x2c); // 写内存开始

    const uint16_t image[100] = {[0 ... 49] = 0x00f8, [50 ... 99] = 0xe007};

    SendData((uint8_t *)image, sizeof(image));

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
    HAL_SPI_Transmit(&hspi1, pData, Size, HAL_MAX_DELAY); // 发送数据
    HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_SET); // 取消选中LCD
}
