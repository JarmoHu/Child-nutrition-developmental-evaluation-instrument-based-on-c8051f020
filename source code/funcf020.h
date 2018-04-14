#ifndef __FUNCF020_H_
#define __FUNCF020_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define key_F1 12
#define key_F2 13
#define key_F3 14
#define key_F4 15
#define key_F5 16
#define key_F6 17
#define key_F7 18
#define key_F8 19

#include "c8051f020.h"

extern uchar lighting_time;

sbit beep = P3 ^ 5;

void Init_Device(void);     //?????
void Oscillator_Init(void); //????16MHz
void Port_IO_Init(void);    //?????
void EMI_Init(void);
void Reset_Sources_Init(void); //?????
void Timer_Init(void);         //??????
void Interrupts_Init(void);    //?????

void Lcd_Delay1ms(uchar c); //LCD??1ms
void Delay10us(void);       //????10us
void Delay10ms(void);       //??????

void Lighting(bit i);
void work_status_change(void);
#endif