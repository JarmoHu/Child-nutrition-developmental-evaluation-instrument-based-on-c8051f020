#ifndef __LCDCONTROL_H_
#define __LCDCONTROL_H_


#include "c8051F020.h"
/**********************************
包含头文件
**********************************/

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD_DATAPINS P2
#define comm 0 //指令定义为0
#define dat 1  //数据定义为1

sbit LCD_RW = P3 ^ 1;
sbit LCD_RS = P3 ^ 0;
sbit LCD_E = P3 ^ 2;

void Wr_lcd(uchar dat_comm, uchar content); //在AC位置写入一个字符
void Init_lcd(void);                        //lcd初始化设置和清屏
void Chrt_disp(uchar chrt1[], uchar n);     //分两行显示两个字符串
void Chrt_disp_str(uchar chrt1[]);          //在AC位置写入字符串
uchar Strlen1(uchar *numberstr);            //获取字符串长度
#endif