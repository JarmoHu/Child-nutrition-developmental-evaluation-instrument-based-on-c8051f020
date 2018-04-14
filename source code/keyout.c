#include "keyout.h"
#include "lcdcontrol.h"
#include "funcf020.h"

void KeySendByte(uchar datt)
{
	uchar a = 0;
	KEYSCL = 0;
	for (a = 0; a < 8; a++)
	{
		KEYSDA = datt;
		datt = datt >> 1;
		Delay10us();
		KEYSCL = 1;
		Delay10us();
		KEYSCL = 0;
		Delay10us();
	}
	KEYSDA = 0;
}

uchar Getkey()
{
	uchar keyword, keyword1, i;
again_key:
	P4 = P4 | 0x0f;
	KeySendByte(0x00);
	Delay10us();
	Keywait();

	while (key_wait)
		;

	KeySendByte(0xf8);
	for (i = 0; i < 5; i++)
	{
		KeySendBit(i == 0 ? 0 : 1);
		Delay10us();
		Lighting(0);
		keyword = P4 & 0x000f;
		if (keyword != 0x000f)
		{
			keyword1 = P4;
			while ((keyword1 & 0x000f) != 0x000f)
			{
				Delay10ms();
				keyword1 = P4;
			}

			return 4 * i + Changeto2(keyword);
		}
	}
	goto again_key;
}

void KeySendBit(uchar num)
{
	KEYSCL = 0;
	Delay10us();
	KEYSDA = num;
	Delay10us();
	KEYSCL = 1;
	Delay10us();
	KEYSCL = 0;
	Delay10us();
}

uchar Changeto2(uchar keyword)
{
	switch (keyword)
	{
	case 0xe:
		return 0;
		break;
	case 0xd:
		return 1;
		break;
	case 0xb:
		return 2;
		break;
	case 0x7:
		return 3;
		break;
	default:
		return 20;
		break;
	}
}

float Get_keynumber(void)
{
	bit dot_number = 0;
	uchar numberstr[16];
	uchar i = 0, k;
	for (i = 0; i < 16; i++)
	{
		k = Getkey();
		switch (k)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			numberstr[i] = k | 0x30;
			Wr_lcd(dat, numberstr[i]);
			break;

		case 10:
			if (dot_number == 0)
			{
				numberstr[i] = '.';
				Wr_lcd(dat, '.');
				dot_number = 1;
			}
			else
			{
				i--;
			}
			break;

		case keynext:
			if (i != 0)
			{
				numberstr[i] = 0x00;
				goto bre;
			}
			else
			{
				i--;
			}
			break;

		case keydel:
			if (i != 0)
			{
				i--;
				if (numberstr[i] == '.')
				{
					dot_number = 0;
				}
				Wr_lcd(comm, 0xC0 | i);
				Delay10us();
				Wr_lcd(dat, 0x20);
				Delay10us();
				Wr_lcd(comm, 0xC0 | i);
				i--;
			}
			break;
		}
	}
	numberstr[i] = 0x00;
bre:
	return Zhuanhua(numberstr);
}

float Zhuanhua(uchar *numberstr)
{
	uchar i = 0, len;
	float num = 0, dot = 0.1;
	len = Strlen1(numberstr);

	while ((numberstr[i] != '.') && (i < len))
	{
		num = num * 10 + numberstr[i] - '0';
		i++;
	}

	i++;

	while (i < len)
	{
		num = num + (numberstr[i] - '0') * dot;
		i++;
		dot = dot / 10;
	}

	return num;
}

void Keywait(void)
{
	TL0 = 0xB0;
	TH0 = 0x3C;
	TR0 = 1;		 //开启定时
	key_wait = 1;	//置位键盘标志位
	while (key_wait) //等待定时器扫描键盘
		;
	return;
}