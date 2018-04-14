#include "keyout.h"
#include "lcdcontrol.h"
#include "who.h"
#include <intrins.h>
#include "c8051f020.h"
#include "funcf020.h"

#define function1 key_F1
#define function2 key_F2
#define function3 key_F3
#define function4 key_F4
#define maxdata 5

void Evaluate(void);		  //营养发育评价功能
void Forecast(void);		  //身高预测功能
void Flo_to_str(float child); //将数浮点数转化为字符串
void New_data(uchar i);
void View_old(uchar i);
void Ch_to_str(uchar i);
void Change_time(void);
void Set_alarm(void);

uchar heightstr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uchar xdata tempstr[4] = {0x00, 0x00, 0x00, 0x00};

bit key_wait; //键盘中断标志位
bit work_status;
bit alarm;

uchar xdata store_sex[maxdata];
uchar xdata store_month[maxdata];
uchar xdata store_year[maxdata];
uchar xdata store_week[maxdata];
float xdata store_height[maxdata];
float xdata store_weight[maxdata];

uchar lighting_time = 4;

uchar xdata month = 0, week = 0, sex = 0, year = 0;
float xdata height = 0, weight = 0;

uchar xdata hour = 0, minute = 0, second = 0;
uchar xdata hour1 = 0, minute1 = 0, second1 = 0;

void main()
{
	uchar key_menu;
	Init_Device();  //初始化硬件
	beep = 0;		//关闭蜂鸣器
	P4 = P4 | 0x30; //关闭LED0和LED1
	TR2 = 0;
	TR1 = 0;	 //关闭定时器0
	TR0 = 0;	 //关闭定时器1
	EA = 1;		 //打开中断
	Lighting(0); //进行一次闪烁,并中断延时关闭
	Lcd_Delay1ms(1);

again_menu:
	Init_lcd();					 //初始化LCD
	Chrt_disp("F1:Evaluate", 1); //显示功能菜单
	Chrt_disp("F2:Forecast", 2); //显示功能菜单
	work_status_change();
	key_menu = Getkey(); //获取菜单按键
	TR2 = 0;
	if (work_status == 1)
	{
		work_status = 0;
		goto again_menu;
	}
	switch (key_menu)
	{
	case function1:
		Evaluate();
		break;
	case function2:
		Forecast();
		break;
	case function3:
		Change_time();
		break;
	case function4:
		Set_alarm();
		break;
	default:
		goto again_menu;
	}
	goto again_menu; //返回开始界面
}

void timer0() interrupt 1 //中断1(定时器0)为键盘扫描定时子程序
{
	uchar keyword;
	static uchar i = 0;
	i++;
	if (i >= 3)
	{
		i = 0;
		keyword = P4;
		keyword = keyword & 0x0f;
		if (keyword != 0x0f)
		{
			Delay10ms();
			keyword = P4;
			keyword = keyword & 0x0f;
			if (keyword != 0x0f)
			{
				key_wait = 0; //指令置位有效
				TR0 = 0;
			}
		}
	}
	TL0 = 0xb0;
	TH0 = 0x3C;
}

void timer1() interrupt 3 //中断3(定时器1)为LED1自动关闭程序
{
	static uchar i = 0;
	i++;
	if (i >= lighting_time)
	{
		i = 0;
		beep = 0;
		P4 = P4 | 0x20; //关闭LED1
		TR1 = 0;
	}
	TL1 = 0xb0;
	TH1 = 0x3C;
}

void timer2() interrupt 5
{
	static uchar xdata i = 0;
	i++;
	if (i >= 200)
	{
		i = 0;
		work_status = 1;
		Wr_lcd(comm, 0x01);
		Chrt_disp("Welcome to use", 1);
		TR2 = 0;
	}
	T2CON = T2CON & 0x7f;
}

void timer3() interrupt 14
{
	static uchar xdata i = 0;
	i++;
	if (i >= 22)
	{
		i = 0;
		second++;
		if (second == 60)
		{
			second = 0;
			minute++;
			if (minute == 60)
			{
				minute = 0;
				hour++;
				if (hour == 24)
				{
					hour = 0;
				}
			}
		}
		if (work_status)
		{
			Wr_lcd(comm, 0xc0);
			Wr_lcd(dat, (hour / 10) | 0x30);
			Wr_lcd(dat, (hour % 10) | 0x30);
			Wr_lcd(dat, ':');
			Wr_lcd(dat, (minute / 10) | 0x30);
			Wr_lcd(dat, (minute % 10) | 0x30);
			Wr_lcd(dat, ':');
			Wr_lcd(dat, (second / 10) | 0x30);
			Wr_lcd(dat, (second % 10) | 0x30);
		}
		if (alarm)
		{
			if ((hour == hour1) && (minute == minute1) && (second == second1))
				Lighting(1);
		}
	}
	TMR3CN = TMR3CN & 0x7f;
}

void Forecast(void) //身高预测子程序
{
	float father, mother, child;
	uchar sex;

	/*获取性别*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("sex(0:boy 1:girl)", 1);
	Wr_lcd(comm, 0xc0);

again_sex1:
	sex = Getkey();
	if (sex != 1 && sex != 0) //性别判断
		goto again_sex1;

	/*获取父亲身高*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("father's height", 1);
	Wr_lcd(comm, 0xc0);
	father = Get_keynumber();

	/*获取母亲身高*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("mother's height", 1);
	Wr_lcd(comm, 0xc0);
	mother = Get_keynumber();

	child = Pre_length(sex, mother, father); //计算孩子身高平均值

	Wr_lcd(comm, 0x01);
	Chrt_disp("Max_height ", 1);
	Flo_to_str(child + 7.5);
	Chrt_disp_str(heightstr);
	Chrt_disp("Min_height ", 2);
	Flo_to_str(child - 7.5);
	Chrt_disp_str(heightstr);

	while (Getkey() != keynext) //等待键盘按下
		;

	return;
}

void Evaluate(void)
{
	uchar xdata data_string = 0, data_menu = 0;
	float xdata z_score[4] = {0, 0, 0, 0};

again_data_menu:
	Wr_lcd(comm, 0x01);
	Chrt_disp("F1:new data", 1);
	Chrt_disp("F2:view old", 2);
	data_menu = Getkey();
	if ((data_menu != key_F1) && (data_menu != key_F2))
		goto again_data_menu;

again_maxdata:
	Wr_lcd(comm, 0x01);
	Chrt_disp("child code: 1~", 1);
	Wr_lcd(dat, maxdata | 0x30);
	Wr_lcd(comm, 0xc0);
	data_string = Getkey();
	if ((data_string > maxdata) || (data_string == 0))
		goto again_maxdata;

	data_string--;
	switch (data_menu)
	{
	case key_F1:
		New_data(data_string);
		break;
	case key_F2:
		View_old(data_string);
		break;
	}

	/*分别计算z分数*/
	z_score[0] = Length_zcodes(sex, height, month, week);
	z_score[1] = Weigh_zcodes(sex, weight, month, week);
	if (((month <= 24) && (height >= 45) && (height <= 110)) || ((month > 24) && (height >= 65) && (height <= 120)))
		z_score[2] = We_len_zcodes(sex, weight, height, month);
	z_score[3] = BMI_zcodes(sex, weight, height, month, week);

	/*身高-年龄判断*/
	if (z_score[0] > 3)
	{
		P4 = P4 && 0xef; //开启LED0
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("L-A: excessive tallness");
	}
	else if (z_score[0] >= (-1))
	{
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("L-A: median tallness");
	}
	else if (z_score[0] >= (-2))
	{
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("L-A: stunted");
	}
	else
	{
		P4 = P4 && 0xef; //开启LED0
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("L-A: severely stunted");
	}

	while (Getkey() != keynext)
		;
	P4 = P4 | 0x10; //关闭LED0

	/*体重-年龄判断*/
	if (z_score[1] > 1)
	{
		P4 = P4 && 0xef; //开启LED0
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("W-A: possible risk of overweight");
	}
	else if (z_score[1] >= (-1))
	{
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("W-A: median weight");
	}
	else if (z_score[1] >= (-2))
	{
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("W-A: underweight");
	}
	else
	{
		P4 = P4 && 0xef; //开启LED0
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("W-A: severely underweight");
	}

	while (Getkey() != keynext)
		;
	P4 = P4 | 0x10; //关闭LED0

	/*体重-身高判断*/
	if (((month <= 24) && (height >= 45) && (height <= 110)) || ((month > 24) && (height >= 65) && (height <= 120)))
	{
		if (z_score[2] > 3)
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: obese");
		}
		else if (z_score[2] >= 2)
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: overweight");
		}
		else if (z_score[2] >= 1)
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: possible risk of overweight");
		}
		else if (z_score[2] >= (-2))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: median weight");
		}
		else if (z_score[2] >= (-3))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: wasted");
		}
		else
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("W-L: severely wasted");
		}
	}
	else
	{
		Wr_lcd(comm, 0x01);
		Chrt_disp_str("W-L: No standard");
	}

	while (Getkey() != keynext)
		;
	P4 = P4 | 0x10; //关闭LED0

	/*BMI判断*/
	if (month < 60)
	{
		if (z_score[3] > 3)
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: obese");
		}
		else if (z_score[3] >= 2)
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: overweight");
		}
		else if (z_score[3] >= 1)
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: possible risk of overweight");
		}
		else if (z_score[3] >= (-2))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: median weight");
		}
		else if (z_score[3] >= (-3))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: wasted");
		}
		else
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: severely wasted");
		}
	}
	else
	{
		if (z_score[3] > 2)
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: obesity");
		}
		else if (z_score[3] >= 1)
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: overweight");
		}
		else if (z_score[3] >= (-2))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: median weight");
		}
		else if (z_score[3] >= (-3))
		{
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: thinness");
		}
		else
		{
			P4 = P4 && 0xef; //开启LED0
			Wr_lcd(comm, 0x01);
			Chrt_disp_str("BMI: severe thiness");
		}
	}

	while (Getkey() != keynext)
		;
	P4 = P4 | 0x10; //关闭LED0

	return;
}

void New_data(uchar i)
{

	/*获取性别*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("sex 0:boy 1:girl", 1);
	Wr_lcd(comm, 0xc0);
again1:
	store_sex[i] = Getkey();
	if (store_sex[i] != 1 && store_sex[i] != 0)
		goto again1;

again_month:
	/*获取岁数*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("year   F1:Enter", 1);
	Wr_lcd(comm, 0xc0);
	store_year[i] = (uchar)Get_keynumber();

	/*获取月数*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("month", 1);
	Wr_lcd(comm, 0xc0);
	store_month[i] = (uchar)Get_keynumber();

	store_month[i] = store_month[i] + 12 * store_year[i];
	if (store_month[i] > 84) //判断月数是否超过可计算值
		goto again_month;

	if (store_month[i] <= 3)
	{
	again_week:
		/*获取周数*/
		Wr_lcd(comm, 0x01);
		Chrt_disp("week", 1);
		Wr_lcd(comm, 0xc0);
		store_week[i] = (uchar)Get_keynumber();
		if (store_week[i] > 13)
			goto again_week;
	}

	/*获取身高*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("height (cm)", 1);
	Wr_lcd(comm, 0xc0);
	store_height[i] = Get_keynumber();

	/*获取体重*/
	Wr_lcd(comm, 0x01);
	Chrt_disp("weight (kg)", 1);
	Wr_lcd(comm, 0xc0);
	store_weight[i] = Get_keynumber();

	sex = store_sex[i];
	month = store_month[i];
	week = store_week[i];
	height = store_height[i];
	weight = store_weight[i];
}

void View_old(uchar i)
{
	Wr_lcd(comm, 0x01);
	Chrt_disp("sex: ", 1);
	Chrt_disp_str(store_sex[i] ? "girl" : "boy");

	if (store_month[i] > 3)
	{
		Chrt_disp("month: ", 2);
		Ch_to_str(store_month[i]);
		Chrt_disp_str(tempstr);
	}
	else
	{
		Chrt_disp("week: ", 2);
		Ch_to_str(store_week[i]);
		Chrt_disp_str(tempstr);
	}
	while (Getkey() != keynext)
		;

	Wr_lcd(comm, 0x01);
	Chrt_disp("height: ", 1);
	Flo_to_str(store_height[i]);
	Chrt_disp_str(heightstr);
	Chrt_disp_str("cm");

	Chrt_disp("weight: ", 2);
	Flo_to_str(store_weight[i]);
	Chrt_disp_str(heightstr);
	Chrt_disp_str("kg");

	while (Getkey() != keynext)
		;

	sex = store_sex[i];
	month = store_month[i];
	week = store_week[i];
	height = store_height[i];
	weight = store_weight[i];
}

void Flo_to_str(float child)
{
	heightstr[0] = (((uchar)(child / 100)) % 10) | 0x30;
	heightstr[1] = (((uchar)(child / 10)) % 10) | 0x30;
	heightstr[2] = (((uchar)child) % 10) | 0x30;
	heightstr[3] = '.';
	heightstr[4] = (((uint)(child * 10)) % 10) | 0x30;
}

void Ch_to_str(uchar i)
{
	tempstr[0] = (((uchar)(i / 100)) % 10) | 0x30;
	tempstr[1] = (((uchar)(i / 10)) % 10) | 0x30;
	tempstr[2] = (i % 10) | 0x30;
}

void Change_time(void)
{
	TMR3CN = TMR3CN & 0xfb;

again_hour:
	Wr_lcd(comm, 0x01);
	Chrt_disp("hour", 1);
	Wr_lcd(comm, 0xc0);
	hour = (uchar)Get_keynumber();
	if (hour >= 24)
		goto again_hour;

again_minute:
	Wr_lcd(comm, 0x01);
	Chrt_disp("minute", 1);
	Wr_lcd(comm, 0xc0);
	minute = (uchar)Get_keynumber();
	if (minute >= 60)
		goto again_minute;

again_second:
	Wr_lcd(comm, 0x01);
	Chrt_disp("second", 1);
	Wr_lcd(comm, 0xc0);
	second = (uchar)Get_keynumber();
	if (second >= 60)
		goto again_second;

	TMR3CN = TMR3CN | 0x04;
}

void Set_alarm(void)
{
	Wr_lcd(comm, 0x01);
	Chrt_disp("0:close 1:open", 1);
	Wr_lcd(comm, 0xc0);

again_alarm:
	alarm = (bit)Getkey();
	if (alarm != 1 && alarm != 0) //闹铃判断
		goto again_alarm;
	if (alarm)
	{
	again_hour1:
		Wr_lcd(comm, 0x01);
		Chrt_disp("hour", 1);
		Wr_lcd(comm, 0xc0);
		hour1 = (uchar)Get_keynumber();
		if (hour1 >= 24)
			goto again_hour1;

	again_minute1:
		Wr_lcd(comm, 0x01);
		Chrt_disp("minute", 1);
		Wr_lcd(comm, 0xc0);
		minute1 = (uchar)Get_keynumber();
		if (minute1 >= 60)
			goto again_minute1;

	again_second1:
		Wr_lcd(comm, 0x01);
		Chrt_disp("second", 1);
		Wr_lcd(comm, 0xc0);
		second1 = (uchar)Get_keynumber();
		if (second1 >= 60)
			goto again_second1;
	}
}