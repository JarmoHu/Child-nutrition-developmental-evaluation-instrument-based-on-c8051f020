#include "lcdcontrol.h"
#include "funcf020.h"

void Wr_lcd(uchar dat_comm, uchar content)
{
	if (dat_comm)
	{
		LCD_RS = 1; //_data
		LCD_RW = 0; //write
	}
	else
	{
		LCD_RS = 0; //command
		LCD_RW = 0; //write
	}
	LCD_DATAPINS = content; //output data or comm
	Lcd_Delay1ms(2);
	LCD_E = 1;
	Lcd_Delay1ms(2);
	LCD_E = 0;
}

void Init_lcd(void)
{
	LCD_E = 0;
	Wr_lcd(comm, 0x01); /*清屏，地址指针指向 00H*/
	Wr_lcd(comm, 0x06); /*光标的移动方向*/
	Wr_lcd(comm, 0x0c); /*开显示，关光标*/
	Wr_lcd(comm, 0x38); /*8 位接口两行显示模式*/
}

void Chrt_disp(uchar chrt1[], uchar n)
{
	if (n == 2)
	{
		Wr_lcd(comm, 0xc0);
	}
	else if (n == 1)
	{
		Wr_lcd(comm, 0x80);
	}
	Chrt_disp_str(chrt1);
}

void Chrt_disp_str(uchar chrt1[])
{
	uchar len1 = Strlen1(chrt1), i;
	if (len1 <= 16)
	{
		for (i = 0; i < len1; i++)
		{
			Wr_lcd(dat, chrt1[i]);
		}
	}
	else
	{
		for (i = 0; i < 16; i++)
		{
			Wr_lcd(dat, chrt1[i]);
		}
		Wr_lcd(comm, 0xc0);
		for (i = 16; i < len1; i++)
		{
			Wr_lcd(dat, chrt1[i]);
		}
	}
}

uchar Strlen1(uchar *numberstr)
{
	uchar i = 0;
	while (numberstr[i] != 0x00)
	{
		i++;
	}
	return i;
}
