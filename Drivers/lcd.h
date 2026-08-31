/*
 * lcd.h
 *
 *  Created on: Nov 21, 2025
 *      Author: liuyu
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>

#ifndef LCD_LIB
#include "fp.h"
#endif

/* 如果屏幕颜色不对，请使用此宏定义 */
// #define INVERT_SCREEN_COLOR

#ifdef INVERT_SCREEN_COLOR
#define lcd_init(Init) lcd_generic_init(Init, FP, 1)
#else
#define lcd_init(Init) lcd_generic_init(Init, FP, 0)
#endif

typedef struct
{
	void (*back_light_control_callback)(uint8_t on); /* 控制LCD屏的背光，on=1点亮，on=0熄灭 */
	void (*reset_callback)(void); /* 控制屏幕复位的回调函数 */
	void (*send_command_callback)(uint8_t cmd); /* 通过SPI发送命令的回调函数 */
	void (*send_data_callback)(uint8_t *data, uint16_t size); /* 通过SPI发送数据的回调函数 */

} LCD_InitTypeDef;

typedef struct
{
	const float *Waveform; // 波形数据
	uint16_t WaveLength;   // 波形数据的长度
	uint16_t XOffset;      // 波形显示的起始位置
	float XScale;          // X轴尺度因子，单位s/格
	float YScale;          // Y轴尺度因子，单位V/格
	uint32_t SampleRate;   // 采样率，单位Hz
	int8_t CursorX1Pct;   // x轴游标1的百分比位置，单位%，范围0~100
	int8_t CursorY1Pct;    // y轴游标1的百分比位置，单位%，范围-100~+100
	int8_t CursorX2Pct;   // x轴游标2的百分比位置，单位%，范围0~100
	int8_t CursorY2Pct;    // y轴游标2的百分比位置，单位%，范围-100~+100
	float Trigger;         // 触发电压，单位V

} WaveformParamTypeDef;

//
// @功能：初始化LCD屏幕
// @参数：Init - 用户提供的初始化参数，详见 @ref WaveformParamTypeDef
//
void lcd_generic_init(LCD_InitTypeDef *Init, const char *Param, uint8_t Invert);


//
// @功能：控制LCD屏幕背光的开关
// @参数：On  0 - 熄灭背光，非零 - 点亮背光
//
void lcd_bk_light_control(uint8_t on);

//
// @功能：设置光标的位置，LCD的坐标系原点在左上角
// @参数：x - 横坐标，范围0~319
// @参数：y - 纵坐标，范围0~239
//
void lcd_set_cursor(int16_t x, int16_t y);

//
// @功能：设置画笔颜色
// @参数：Color - 画笔颜色。格式RGB888，（0xff0000 - 红色，
//                                      0x00ff00 - 绿色，
//                                      0x0000ff - 蓝色）
//
void lcd_set_pen(uint32_t color);

//
// @功能：设置画刷颜色
// @参数：Color - 画刷颜色。格式RGB888，（0xff0000 - 红色，
//                                      0x00ff00 - 绿色，
//                                      0x0000ff - 蓝色）
//
void lcd_set_brush(uint32_t color);

//
// @功能：使用画刷颜色清空屏幕
// @注意：在调用该方法之前请使用LCD_SetBrush()来设置画刷的颜色
//
void lcd_clear(void);

//
// @功能：绘制矩形，包括边框和填充。
//        矩形的左上角点为当前坐标，填充色为画刷颜色，边框为画笔颜色
// @参数：Width - 要绘制的矩形的宽度
// @参数：Height - 要绘制的矩形的高度
//
void lcd_draw_rect(uint16_t width, uint16_t height);

//
// @功能：只绘制矩形边框，无颜色填充。
//        矩形的左上角点为当前坐标，边框为画笔颜色
// @参数：Width - 要绘制的矩形边框的宽度
// @参数：Height - 要绘制的矩形边框的高度
//
void lcd_draw_rect_frame(uint16_t width, uint16_t height);

//
// @功能：只填充矩形，无边框。
//        矩形的左上角点为当前坐标，填充色为画刷颜色
// @参数：Width - 要填充的矩形的宽度
// @参数：Height - 要填充的矩形的高度
//
void lcd_fill_rect(uint16_t width, uint16_t height);

//
// @功能：绘制单个字符
//        字符的左下角点位于当前光标处，前景色为画笔颜色，背景色为画刷颜色
// @参数：Unicode - 字符的Unicode编码
//
void lcd_print_char(uint32_t unicode);

//
// @功能：绘制字符串
//        字符串的左下角点位于当前光标处，前景色为画笔颜色，背景色为画刷颜色
// @参数：Str - 要绘制的字符串
//
void lcd_print_string(const char *str);

//
// @功能：格式化浮点数并在屏幕上显示
//        字符串的左下角点位于当前光标处，前景色为画笔颜色，背景色为画刷颜色
// @参数：Value - 要绘制的浮点数的值
// @参数：IntegerDigits - 整数部分要显示的位数，不足左侧填充0
// @参数：FractionalDigits - 小数部分要显示的位数，不足右侧填充0
//
void lcd_print_float(float value, uint16_t integerDigits, uint16_t fractionalDigits);

//
// @功能：以国际单位制打印浮点数
// @参数：Value - 要打印的浮点数
// @参数：Length - 字符串的总长度
// @参数：UnitStr - 单位字符串
//
void lcd_print_float_si(float value, uint16_t length, const char *unitStr);

//
// @功能：格式化整数并在屏幕上显示
//        字符串的左下角点位于当前光标处，前景色为画笔颜色，背景色为画刷颜色
// @参数：Value - 要绘制的整数的值
// @参数：IntegerDigits - 整数部分要显示的位数，不足左侧填充0
//
void lcd_print_integer(int value, uint16_t integerDigits);

//
// @功能：以十六进制形式格式化整数并显示在屏幕上
//        字符串的左下角点位于当前光标处，前景色为画笔颜色，背景色为画刷颜色
// @参数：Value - 要绘制的整数的值
// @参数：IntegerDigits - 整数部分要显示的位数，不足左侧填充0
//
void lcd_print_hex(unsigned int value, uint16_t digits);

//
// @功能：在屏幕上绘制图像，图像的右上角点位于当前光标处
// @参数：Width - 图像的宽度
// @参数：Height - 图像的高度
// @参数：pData - 要绘制的图像，按从左到右，从上倒下的顺序排列
//                颜色采用RGB565格式，即RRRRRGGGGGGBBBBB，
//                例如红色 = 1111 1000 0000 0000b = 0xf800
//                    绿色 = 0000 0111 1110 0000b = 0x07e0
//                    蓝色 = 0000 0000 0001 1111b = 0x001f
//
void lcd_draw_bitmap(uint16_t width, uint16_t height, const uint8_t *pData);

//
// @功能：绘制示波器波形
// @参数：WaveformParam.Waveform - 波形数据
// @参数：WaveformParam.Length - 波形数据的长度
// @参数：WaveformParam.Offset - 波形显示的起始位置
// @参数：WaveformParam.XScale - X轴缩放比，横坐标每格表示的时间长度，单位s/格
// @参数：WaveformParam.YScale - Y轴缩放比，纵坐标每格表示的电压大小，单位V/格
// @参数：WaveformParam.SampleRate - 采样率，单位Hz
// @参数：WaveformParam.CursorX1Pct - 横轴（时间轴）游标1的位置，单位百分比，范围0 ~ 100
// @参数：WaveformParam.CursorY1Pct - 纵轴（电压轴）游标1的位置，单位百分比，范围-100 ~ +100
// @参数：WaveformParam.CursorX2Pct - 横轴（时间轴）游标2的位置，单位百分比，范围0 ~ 100
// @参数：WaveformParam.CursorY2Pct - 纵轴（电压轴）游标2的位置，单位百分比，范围-100 ~ +100
// @参数：WaveformParam.TriggerLevel - 触发电压的大小，单位V
//
void lcd_draw_waveform(WaveformParamTypeDef *waveformParam);

//
// @功能：获取X轴游标的真实值
// @参数：XPos - X轴游标的当前位置，范围0~100
// @参数：XScale - X轴的缩放比例，单位：秒/格
// @返回：对应的游标值，单位：秒
//
float lcd_get_x_cusor_value(int8_t xPos, float xScale);

//
// @功能：获取Y轴游标的真实值
// @参数：YPos - Y轴游标的当前位置，范围-100~+100
// @参数：YScale - Y轴的缩放比例，单位：伏特/格
// @返回：对应的游标值，单位：伏特
//
float lcd_get_y_cursor_value(int8_t yPos, float yScale);

#endif /* INC_LCD_H_ */
