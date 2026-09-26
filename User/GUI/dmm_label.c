/*
 * dmm_label.c
 *
 *  Created on: Sep 26, 2026
 *      Author: lqtghm
 */

 #include "dmm.h"
 #include "lcd.h"

//
//@简介：
//
 void DMMLable1_Repaint(void)
 {
    //获取DMM模块的测量结果
    DMMData_t data = DMM_GetData();

    //绘制背景
    lcd_set_brush(0xCCCCCC);
    lcd_set_cursor(110,215);
    lcd_fill_rect(120,15);

    //绘制挡位信息
    const char *rangeNames[] = {"DMM-50V:", "DMM-10V:", "DMM-2V:", "DMM-1kΩ:", "DMM-10kΩ:", "DMM-100kΩ:", "DMM-1MΩ:"};
    lcd_set_pen(0x000000);
    lcd_set_cursor(116,226);
    lcd_print_string(rangeNames[data.Range]);

    //绘制测量值
    const char *uintNames[] = {"V", "V", "V", "R", "R", "R"};
    lcd_print_float_si(data.Value, 5, uintNames[data.Range]);
 }
