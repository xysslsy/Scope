#ifndef DMM_H_
#define DMM_H_

#include <stdint.h>

typedef struct
{
    uint8_t Range; //Range = 0:电压档50V;Range = 1:电压档10V；Range = 2:电压档2V; = 3:电阻档1kΩ; 4-10k ; 5-100k; 6-1M
    float Value; //测量具体的值
    float Vdda; //ADC的参考电压值
} DMMData_t;


void DMM_Init(void);
DMMData_t DMM_GetData(void); //前面的DMMData_t是返回值类型，DMM_GetData是函数名，void是参数列表，表示该函数不接受任何参数。

#endif /* DMM_H_ */
