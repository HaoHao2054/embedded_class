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


void uart0_Init(void)
{
	GPA0CON &= ~0xFF;
	GPA0CON |=  0x22;
	ULCON0 = (0x3<<0) | (0<<2) | (0<<3) | (0<<6);
	UCON0 = (1<<0) | (1<<2) | (1<<6) | (0<<10);
	UFCON0 = 0;
	UBRDIV0 = 34;
	UDIVSLOT0 = 0xDDDD;
	return;
}

void fac(unsigned char c)
{
	while(!(UTRSTAT0 & (1<<2)));
	UTXH0 = c;
	return;
}

unsigned char shouc(void)
{
	while(!(UTRSTAT0 & (1<<0)));
	return(URXH0);
}

void fas(char * str)
{
	char * p = str;
	while( * p)
	fac( * p++);
}

