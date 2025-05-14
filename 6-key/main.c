#define GPH0CON		*((volatile unsigned int*)0xE0200C00)
#define GPH1CON		*((volatile unsigned int*)0xE0200C20)	
#define GPH0DAT		*((volatile unsigned int*)0xE0200004)	
#define EXT_INT_0_CON	*((volatile unsigned int*)0xE0200E00)	
#define EXT_INT_0_MASK	*((volatile unsigned int*)0xE0200F00)	
#define EXT_INT_0_PEND	*((volatile unsigned int*)0xE0200F40)
#define EXT_INT_1_CON	*((volatile unsigned int*)0xE0200E04)	
#define EXT_INT_1_MASK	*((volatile unsigned int*)0xE0200F04)	
#define EXT_INT_1_PEND	*((volatile unsigned int*)0xE0200F44)	
#define VICOIRQSTATUS	*((volatile unsigned int*)0xF2000000)	
#define VICOINTSELECT	*((volatile unsigned int*)0xF200000C)
#define VICOINTENABLE	*((volatile unsigned int*)0xF2000010)	
#define VICOVECTADDRO	*((volatile unsigned int*)0xF2000100)	
#define VICOVECTADDR4	*((volatile unsigned int*)0xF2000110)//sw6
#define VICOVECTADDR8	*((volatile unsigned int*)0xF2000120)//sw7
#define VICOADDRESS	*((volatile unsigned int*)0xF2000F00)	
	
#define GPJ2CON         *((volatile unsigned int*)0xE0200280)
#define GPJ2DAT         *((volatile unsigned int*)0xE0200284)

#define GPD0CON         *((volatile unsigned int*)0xE02000A0)
#define GPD0DAT         *((volatile unsigned int*)0xE02000A4)

extern void IRQ_handle(void);	

void delay(volatile unsigned long dly)    //x50000
{
  volatile unsigned int t=0xFFFF;
  while(dly--);
		for( ; t>0 ;t--);
}

void gpio0_init(void)
{
  GPJ2CON &=~(0xFFFF);
  GPJ2CON |=  0x1111;
  GPJ2DAT |= (0xF);
	
  GPD0CON &=~(0xF << 2*4);
  GPD0CON |= (0x1 << 2*4);
  GPD0DAT |= (0x0);
  return;
}

void init_key(void)
{
	GPH0CON &= ~(0xF<<4*4);
	GPH0CON |=  (0xF<<4*4);
	GPH1CON &= ~(0xF);
	GPH1CON |=  (0xF);
	
	EXT_INT_0_CON  &= ~(0xF<<4*4);
	EXT_INT_0_CON  |=  (2<<4*4);
	EXT_INT_0_MASK &= ~(0x3<<4);

	EXT_INT_1_CON  &= ~(0xF);
	EXT_INT_1_CON  |=  0x2;
	EXT_INT_1_MASK &= ~(0x3);
}

void clear_int_pend()
{
	EXT_INT_0_PEND |= 0x3<<4;
	EXT_INT_1_PEND |= 0x3;
}

void led_L_R(void)
{
  unsigned int j=0;
  unsigned char n=0;
  while(1)
  {
		if(n==5){ break;}
		GPJ2DAT &=~(1<<j);
		j==3?(j=0,n++):j++; 
		delay(0x30000);
		GPJ2DAT |= (0xF) ;
  }
  return ;
}

void FMQ(void)
{
	unsigned long i=0;
	for(i=0;i<5;i++)
	{
		GPD0DAT =1<<2;
		delay(0x30000);
		GPD0DAT =0X0;
		delay(0x30000);
	}
	

}

void init_int(void)
{
	VICOINTSELECT   = ~((0x1<<4)|(0x1<<8));
	VICOADDRESS     =  0x0;
	VICOVECTADDR4   = (int)IRQ_handle;
	VICOVECTADDR8   = (int)IRQ_handle;
	VICOINTENABLE  |=  (0x1<<4)|(0x1<<8);
}


void clear_vectaddr(void)
{
	VICOADDRESS =  0x0;
}

unsigned long get_ieqstatus(void)
{
	return VICOIRQSTATUS; 
}


void irq_handler()
{
	volatile unsigned char key1_code = ((get_ieqstatus()&(0x1<<4))>>4)&0x1;
	volatile unsigned char key2_code= ((get_ieqstatus()&(0x1<<8))>>8)&0x1;
	

	if(key2_code==1)
	{

	led_L_R(); delay(0x30000);
	}
	else if(key1_code==1)
			{

			 FMQ();delay(0x30000);
			}
        clear_vectaddr();
	clear_int_pend();
	}


int main(void)
{
	init_key();
	gpio0_init();
	init_int();
	while(1);
}
