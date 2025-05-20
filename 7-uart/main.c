#include <stdio.h>
#define GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define GPJ2DAT 	(*(volatile unsigned long *) 0xE0200284)
#define GPH0CON     (*(volatile unsigned int *)0xE0200C00)  
#define GPH0DAT     (*(volatile unsigned int *)0xE0200C04)
#define	GPD0CON     (*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT     (*(volatile unsigned long *)0xE02000A4) 
#define	GPC1CON		(*((volatile unsigned long *)0xE0200060))
#define	GPC1DAT		(*((volatile unsigned long *)0xE0200064))
#define	GPC1PUD		(*((volatile unsigned long *)0xE0200068))
#define VIC0INTSELECT       *((volatile unsigned int *)0xF200000C)  
#define VIC0INTENABLE       *((volatile unsigned int *)0xF2000010)  
#define VIC0ADDRESS    		*((volatile unsigned int *)0xF2000F00)
#define VIC0VECTADDR2    	*((volatile unsigned int *)0xF2000108)  
#define VIC0VECTADDR3    	*((volatile unsigned int *)0xF200010C)
#define EXT_INT_0_CON    	*((volatile unsigned int *)0xE0200E00) 
#define EXT_INT_0_MASK    	*((volatile unsigned int *)0xE0200F00) 
#define EXT_INT_0_PEND      *((volatile unsigned int *)0xE0200F40)  
#define EXT_INT_1_CON    	*((volatile unsigned int *)0xE0200E04) 
#define EXT_INT_1_MASK    	*((volatile unsigned int *)0xE0200F04) 
#define EXT_INT_1_PEND      *((volatile unsigned int *)0xE0200F44)  

#define LEDS 1
int a=1;
extern void key_isr(void);
extern void uart_send_byte(unsigned char byte);
extern unsigned char uart_recv_byte();
extern void uart_send_string(char *str); 
void delay(volatile unsigned int t)
{
	volatile unsigned int t2 = 0xFFFF;
	while (t--)
		for (; t2; t2--);
}
    
void buzzer_init(void)
{ 
      GPD0CON |= 1<<8;
}

void buzzer_on(void)
{
      GPD0DAT |= 1<<2;
}

void buzzer_off(void)
{
      GPD0DAT &= ~(1<<2);

}

void key_handle()  
{     
    volatile unsigned char key_code = EXT_INT_0_PEND & 0xC;  
    VIC0ADDRESS = 0;        

    EXT_INT_0_PEND |= 0xC;     

    if (key_code == 0x08)      /* SW5 */
    {			
	if(a==1)
	{a=2;}
	else
	{a=1;}
			
	
    }
		 	
}  
					

 
int main()  {      
char byte;   
    GPJ2CON &= ~(0xFF << 0);
    GPJ2CON |= ((0x01 << 0) | (0x01 << 4) | (0x01 << 8) | (0x01 << 12));//((0x01 << 0) | (0x01 << 4));
    GPJ2DAT |= (0xFF << 0);
    GPH0CON |= 0xFF << 8;                 
    EXT_INT_0_CON &= ~(0xFF << 8);  		
    EXT_INT_0_CON |= (2 << 8) | (2 << 12);          
    EXT_INT_0_MASK &= ~0xC;     
    uart_send_string("\r\nUART Test in S5PV210\r\n");               
   
    VIC0INTSELECT &= ~0xC;                   
    VIC0INTENABLE |= 0xC;                     
    VIC0VECTADDR2 = (int)key_isr;          
    VIC0VECTADDR3 = (int)key_isr;  
    while(1)
{
char *number_strings[]={
"10\n","9\n","8\n","7\n","6\n","5\n","4\n","3\n","2\n","1\n","0\n",
};
int countdown =11;
	if(a==1){
		for(countdown;countdown>0;countdown--)
		{
		uart_send_string("\r\nCounting down \r\n");
		uart_send_string(number_strings[11-countdown]);
		delay(750000);
		}
		
	}
			
	else 
	{uart_send_string("\r\nwawiting to start\r\n");}
	}
   return 0;  
}


