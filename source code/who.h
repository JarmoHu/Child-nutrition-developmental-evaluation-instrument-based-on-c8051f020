#ifndef __WHO_H_
#define __WHO_H_

#define boy 0
#define girl 1

#include <math.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif

float Calcu_zcords(float L, float M, float S, float num);                           //根据LMS计算Z分数
float Length_zcodes(uchar sex, float length, uchar months, uchar weeks);            //得到身高-年龄Z分数
float Weigh_zcodes(uchar sex, float weight, uchar months, uchar weeks);             //得到体重-年龄Z分数
float We_len_zcodes(uchar sex, float weight, float length, uchar months);           //得到体重-身高Z分数
float BMI_zcodes(uchar sex, float weight, float length, uchar months, uchar weeks); //得到BMI Z分数
float Pre_length(uchar sex, float mum, float dad);                                  //由性别和父母身高得到预测身高平均值，结果需要偏差7.5cm
//WHO_1[2][14][3]   0-13周 体长
//WHO_2[2][81][3] 13-7年 体长
//WHO_3[2][14][3]  0-13周 体重
//WHO_4[2][81][3]   13-7年 体重
//WHO_5[2][131][3]  0-2年体重-体长
//WHO_6[2][111][3]  2-5年体重-体长
//WHO_7[2][14][3]   0-13周BMI指数
//WHO_8[2][81][3]   13-7年BMI指数

extern float code WHO_1[2][14][3];
extern float code WHO_2[2][81][3];
extern float code WHO_3[2][14][3];
extern float code WHO_4[2][81][3];
extern float code WHO_5[2][131][3];
extern float code WHO_6[2][111][3];
extern float code WHO_7[2][14][3];
extern float code WHO_8[2][81][3];

#endif
