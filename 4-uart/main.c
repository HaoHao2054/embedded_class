#define GPA0CON		*((volatile unsigned int *)0xE0200000)
#define ULCON0		*((volatile unsigned int *)0xE2900000)
#define UCON0		*((volatile unsigned int *)0xE2900004)
#define UFCON0		*((volatile unsigned int *)0xE2900008)
#define UTRSTAT0	*((volatile unsigned int *)0xE2900010)
#define UTXH0		*((volatile unsigned int *)0xE2900020)
#define URXH0		*((volatile unsigned int *)0xE2900024)
#define UBRDIV0		*((volatile unsigned int *)0xE2900028)
#define UDIVSLOT0	*((volatile unsigned int *)0xE290002C)
#define GPJ2CON  	*((volatile unsigned int *)0xE0200280)  
#define GPJ2DAT 	*((volatile unsigned int *)0xE0200284)  


extern void uart0_Init(void);

void delay(volatile unsigned long dly)
{
	volatile unsigned int t = 0xFFFF;
	while(dly --)
		for(; t>0; t --);
}

int main(void)
{
	char c;
	int i,n;
	uart0_Init();
	GPJ2CON &= ~(0xFFFF << (0*4));
	GPJ2CON |=   0x1111 << (0*4);
	GPJ2DAT |= 0xF;
	fas("=============================\r\n");
	fas("S5PV210 UART Test:\r\n");
	fas("1.LED1 LED2 LED3 LED4 flash 5 times\r\n");
	fas("2.LED4 LED3 LED2 LED1 flash 5 times\r\n");
	fas("=============================\r\n");
	while (1)
	{
		c = shouc();
		fac(c);
		if(c=='1')
		{
			for(i = 0;i < 5;i++)
			{
				GPJ2DAT = 0xF & ~(0x1 << 0);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 1);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 2);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 3);
				delay(0x50000);
			}	
			GPJ2DAT |= 0xF;
		}
		else if(c== '2')
		{
			for(n = 0;n < 5;n++)
			{
				GPJ2DAT = 0xF & ~(0x1 << 3);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 2);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 1);
				delay(0x50000);
				GPJ2DAT = 0xF & ~(0x1 << 0);
				delay(0x50000);
			}	
			GPJ2DAT |= 0xF;
		}
	}
	return 0;

}
