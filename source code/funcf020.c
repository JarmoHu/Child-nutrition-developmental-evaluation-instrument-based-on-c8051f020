#include "funcf020.h"
#include "c8051f020.h"

void EMI_Init(void)
{
	EMI0CF = 0x00;
	EMI0TC = 0x00;
}

void Port_IO_Init(void)
{
	// P0.0  -  Unassigned,  Push-Pull,  Digital
	// P0.1  -  Unassigned,  Push-Pull,  Digital
	// P0.2  -  Unassigned,  Push-Pull,  Digital
	// P0.3  -  Unassigned,  Push-Pull,  Digital
	// P0.4  -  Unassigned,  Push-Pull,  Digital
	// P0.5  -  Unassigned,  Push-Pull,  Digital
	// P0.6  -  Unassigned,  Push-Pull,  Digital
	// P0.7  -  Unassigned,  Push-Pull,  Digital

	// P1.0  -  Unassigned,  Push-Pull,  Digital
	// P1.1  -  Unassigned,  Push-Pull,  Digital
	// P1.2  -  Unassigned,  Push-Pull,  Digital
	// P1.3  -  Unassigned,  Push-Pull,  Digital
	// P1.4  -  Unassigned,  Push-Pull,  Digital
	// P1.5  -  Unassigned,  Push-Pull,  Digital
	// P1.6  -  Unassigned,  Push-Pull,  Digital
	// P1.7  -  Unassigned,  Push-Pull,  Digital

	// P2.0  -  Unassigned,  Push-Pull,  Digital
	// P2.1  -  Unassigned,  Push-Pull,  Digital
	// P2.2  -  Unassigned,  Push-Pull,  Digital
	// P2.3  -  Unassigned,  Push-Pull,  Digital
	// P2.4  -  Unassigned,  Push-Pull,  Digital
	// P2.5  -  Unassigned,  Push-Pull,  Digital
	// P2.6  -  Unassigned,  Push-Pull,  Digital
	// P2.7  -  Unassigned,  Push-Pull,  Digital

	// P3.0  -  Unassigned,  Push-Pull,  Digital
	// P3.1  -  Unassigned,  Push-Pull,  Digital
	// P3.2  -  Unassigned,  Push-Pull,  Digital
	// P3.3  -  Unassigned,  Push-Pull,  Digital
	// P3.4  -  Unassigned,  Push-Pull,  Digital
	// P3.5  -  Unassigned,  Push-Pull,  Digital
	// P3.6  -  Unassigned,  Push-Pull,  Digital
	// P3.7  -  Unassigned,  Push-Pull,  Digital

	P0MDOUT = 0xFF;
	P1MDOUT = 0xFF;
	P2MDOUT = 0xFF;
	P3MDOUT = 0xFF;
	XBR2 = 0x40;
}

void Oscillator_Init(void)
{
	OSCICN = 0x17;
}

void Init_Device(void)
{
	Reset_Sources_Init();
	Timer_Init();
	EMI_Init();
	Port_IO_Init();
	Oscillator_Init();
	Interrupts_Init();
}

void Reset_Sources_Init(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
}

void Interrupts_Init(void)
{
	IE = 0xAA;
	EIE2 = 0x01;
	EIP2 = 0x01;
}

void Timer_Init(void)
{
	TCON = 0x50;
	TMOD = 0x11;
	TL0 = 0xB0;
	TL1 = 0xB0;
	TH0 = 0x3C;
	TH1 = 0x3C;
	T2CON = 0x04;
	RCAP2L = 0x01;
	RCAP2H = 0x00;
	TL2 = 0x01;
	TH2 = 0x00;
	TMR3CN = 0x04;
	TMR3RLL = 0x01;
	TMR3L = 0x01;
	TMR3H = 0x00;
	TMR3RLH = 0x00;
}

void Lcd_Delay1ms(uchar c) //误差 0us
{
	uchar xdata a, b;
	for (; c > 0; c--)
	{
		for (b = 200; b > 0; b--)
		{
			for (a = 3; a > 0; a--)
				;
		}
	}
}

void Delay10us(void)
{
	uchar xdata a, b, c, d;
	for (d = 1; d > 0; d--)
		for (c = 2; c > 0; c--)
			for (b = 1; b > 0; b--)
				for (a = 1; a > 0; a--)
					;
}

void Delay10ms(void)
{
	uchar xdata a, b, c;
	for (c = 5; c > 0; c--)
		for (b = 12; b > 0; b--)
			for (a = 175; a > 0; a--)
				;
}

void Lighting(bit i)
{
	if (i)
		lighting_time = 40;
	else
		lighting_time = 4;
	beep = 1;
	P4 = P4 & 0xdf;
	TL1 = 0xb0;
	TH1 = 0x3C;
	TR1 = 1;
}

void work_status_change(void)
{
	TH2 = 0;
	TL2 = 1;
	TR2 = 1;
}