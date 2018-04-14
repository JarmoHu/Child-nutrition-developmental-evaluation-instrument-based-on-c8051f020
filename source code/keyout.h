#ifndef __KEYOUT_H_
#define __KEYOUT_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

#define keynext 12
#define keydel 13

#include "c8051F020.h"

extern bit key_wait;  //引用main.c键盘中断标志位
sbit KEYSCL = P3 ^ 7; //定义行扫描74HC164时钟线
sbit KEYSDA = P3 ^ 6; //定义行扫描74HC164数据线

void KeySendByte(uchar datt);     //向74HC164发送1Byte数据控制行扫描
uchar Getkey();                   //获得当前按键值
void KeySendBit(uchar num);       //向74HC164发送1Bit数据控制行扫描
uchar Changeto2(uchar keyword);   //将列扫描转换成2进制
float Get_keynumber(void);        //获取当前输入的浮点数,并依次显示在LCD上
float Zhuanhua(uchar *numberstr); //将字符串转化为浮点
void Keywait(void);               //键盘等待
#endif