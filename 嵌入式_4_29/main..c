/*
ʵ������
���°���SW5��4��LED����������ˮ�ƣ�
�ٴΰ��°���SW5��4��LEDȫ��

���š��Ĵ�����
LED1��GPJ2_0��GPJ2CON[3:0]��GPJ2DAT[0] ��LED2��GPJ2_1��GPJ2CON[7:4]��GPJ2DAT[1]
LED3��GPJ2_2��GPJ2CON[11:8]��GPJ2DAT[2]��LED4��GPJ2_3��GPJ2CON[15:12]��GPJ2DAT[3]
SW5��EINT3��GPH0_3��GPH0CON[15:12]
*/
#define GPJ2CON (*(volatile unsigned long *)0xE0200280) // LED
#define GPJ2DAT (*(volatile unsigned long *)0xE0200284)
#define GPH0CON (*(volatile unsigned long *)0xE0200C00) // ����SW
#define GPH0DAT (*(volatile unsigned long *)0xE0200C04)

#define LED1_ON() GPJ2DAT &= ~(0x1 << 0)
#define LED1_OFF() GPJ2DAT |= (0x1 << 0)
#define LED1_TURN() GPJ2DAT ^= (0x1 << 0)
#define LED2_ON() GPJ2DAT &= ~(0x1 << 1)
#define LED2_OFF() GPJ2DAT |= (0x1 << 1)
#define LED2_TURN() GPJ2DAT ^= (0x1 << 1)
#define LED3_ON() GPJ2DAT &= ~(0x1 << 2)
#define LED3_OFF() GPJ2DAT |= (0x1 << 2)
#define LED3_TURN() GPJ2DAT ^= (0x1 << 2)
#define LED4_ON() GPJ2DAT &= ~(0x1 << 3)
#define LED4_OFF() GPJ2DAT |= (0x1 << 3)
#define LED4_TURN() GPJ2DAT ^= (0x1 << 3)

#define SW5 0x8

/*�жϼĴ���*/
#define EXT_INT_0_CON *((volatile unsigned int *)0xE0200E00)
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)
#define VIC0IRQSTATUS *((volatile unsigned int *)0xF2000000)
#define VIC0INTSELECT *((volatile unsigned int *)0xF200000C)
#define VIC0INTENABLE *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)//SW5
#define VIC0ADDRESS *((volatile unsigned int *)0xF2000F00)

unsigned char state=0; // ��ǰ״̬
unsigned long time = 200000; // LED��ʱʱ��
unsigned int j=0;
extern void IRQ_handle(void); // �ú��������ڻ�����

void delay(volatile unsigned long dly)
{
    volatile unsigned int t = 0xFFFF;
    while (dly--)
        for (; t > 0; t--);
}
// LED��ʼ��
void LED_Init(void)
{
    /*LED*/
    GPJ2CON &= ~(0xFFFF);
    GPJ2CON |= (0x1111); // ���ģʽ
	// ��ʼʱ����LED
    LED1_ON();LED2_ON();LED3_ON();LED4_ON();
}
// KEY��ʼ��
void KEY_Init(void)
{
    /*����SW*/
    GPH0CON &= ~(0xF << 3 * 4);
    GPH0CON |= (0xF << 3 * 4); // EXT_INTģʽ���жϹ���
    // ����EXT_INT[3]�ж�Ϊ�½��ش���������������
    EXT_INT_0_CON &= ~(0xF << 3 * 4);
    EXT_INT_0_CON |= (0x2 << 3 * 4);
    // ȡ�������ⲿ�ж�EXT_INT[3]
    EXT_INT_0_MASK &= ~0x8;
}
// ���жϹ���Ĵ���
void clear_int_pend()
{
    EXT_INT_0_PEND |= 0x8; // EXT_INT[3]
}
// ��ʼ���жϿ�����
void Int_Init(void)
{
    // ѡ���ж�����ΪIRQ
    VIC0INTSELECT &= ~0x8; // �ⲿ�ж�EXT_INT[3]ΪIRQ
    // ��VIC0ADDRESS
    VIC0ADDRESS = 0x0;
    // ����EXT_INT[3]��Ӧ���жϷ���������ڵ�ַ
    VIC0VECTADDR3 = (int)IRQ_handle;
    // ʹ���ⲿ�ж�EXT_INT[3]
    VIC0INTENABLE |= 0x8;
}
// �����Ҫ������жϵ��жϴ������ĵ�ַ
void clear_vectaddr(void)
{
    VIC0ADDRESS = 0x0;
}
// ���ж�״̬
unsigned long get_irqstatus(void)
{
    return VIC0IRQSTATUS;
}
void irq_handler(void)
{
    volatile unsigned char key_code = get_irqstatus() & 0x1C;
    clear_vectaddr(); /* ���ж������Ĵ��� */
    clear_int_pend(); /* ��pendingλ */
      if (key_code == SW5 && state == 0)
    {
        state = 1;delay(time);
    }
  //  else if (key_code == SW5 && state == 1)
  //  {
  //      state = 2;
  //  }
	else if	(key_code == SW5 && state == 1)
	{
		 state = 0;delay(time);

	}
}

int main(void)
{
    
    LED_Init();                 // LED��ʼ��
    KEY_Init();                 // KEY��ʼ��
    Int_Init();                 // �жϳ�ʼ��
	state=1;
    while (1)
    {
		switch (state)
        {
			case 0:// ��ˮ��
			{
				LED1_ON();LED2_OFF();LED3_OFF();LED4_OFF();
				delay(time);
				LED1_OFF();LED2_ON();LED3_OFF();LED4_OFF();
				delay(time);
				LED1_OFF();LED2_OFF();LED3_ON();LED4_OFF();
				delay(time);
				LED1_OFF();LED2_OFF();LED3_OFF();LED4_ON();
				delay(time);
				LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();
				delay(time);
			}
			case 1:// ȫ��
			{
				LED1_OFF();LED2_OFF();LED3_OFF();LED4_OFF();
				break;
			}
			//case 1://ȫ��
			//{
			
			//	break;
			//}   
			default:
				break;
        }
    }
    return 0;
}
/* �Ĵ������� */
#define GPJ2CON   *((volatile unsigned int * )0xE0200280)
#define GPJ2DAT   *((volatile unsigned int * )0xE0200284)
#define GPH0CON   *((volatile unsigned int *)0xE0200C00)
#define GPH0DAT   *((volatile unsigned int *)0xE0200C04)
#define EXT_INT_0_CON  *((volatile unsigned int *)0xE0200E00)
#define EXT_INT_0_MASK *((volatile unsigned int *)0xE0200F00)
#define EXT_INT_0_PEND *((volatile unsigned int *)0xE0200F40)
#define VIC0IRQSTATUS  *((volatile unsigned int *)0xF2000000)
#define VIC0INTSELECT  *((volatile unsigned int *)0xF200000C)
#define VIC0INTENABLE  *((volatile unsigned int *)0xF2000010)
#define VIC0VECTADDR3 *((volatile unsigned int *)0xF200010C)
#define VIC0ADDRESS    *((volatile unsigned int *)0xF2000F00)
#define SW5 0x8
unsigned char flag = 0; 
void delay(volatile unsigned long dly)
{
	volatile unsigned int t=0xffff;
	while(dly--)
		for(;t>0;t--);
}

extern void IRQ_handle(void);

void init_leds(void)
{
	GPJ2CON &=~(0xFFFF);	  
	GPJ2CON |=(0x1111);
	GPJ2DAT |=0xF;	
}

void led_from_left_to_right(void)
{
	unsigned long j=0;
	for(j=0;j<4;j++)
	{
		GPJ2DAT =~(1<<j);
		delay(0x50000);		
	}
	GPJ2DAT |=0xF;
}

void led_off(void)
{
	GPJ2DAT |=0xF;
}

void init_key(void) 
{
	GPH0CON &= ~(0xF << 3*4);   
	GPH0CON |= (0xF << 3*4);    

	EXT_INT_0_CON &= ~(0xF << 3*4);
	EXT_INT_0_CON |= (0x2 << 3*4);  

	EXT_INT_0_MASK &= ~(0x1 << 3);

}

void key_val(void)
{
	unsigned long key_get=0;	
	key_get=(GPH0DAT &(0x1<<3)>>3);
}

void clear_int_pend(void)
{
	EXT_INT_0_PEND |= 0x1 << 3;
}
void clear_vectaddr(void)
{
	VIC0ADDRESS = 0x0;
}

unsigned long get_irqstatus(void)
{
	return VIC0IRQSTATUS;
}


void irq_handler(void)
{
    volatile unsigned char key_code = get_irqstatus() & 0x1C;
    clear_vectaddr(); 
    clear_int_pend(); 
      if (key_code == SW5 && flag == 0)
    {
        flag = 1;delay(100000);
    }

	else if	(key_code == SW5 && flag == 1)
	{
		 flag = 0;delay(1000000);

	}
}

void init_Int(void) 
{
	VIC0INTSELECT &= ~(0x1 << 3);
	VIC0ADDRESS = 0x0;
	VIC0VECTADDR3 = (int)IRQ_handle;
  
	VIC0INTENABLE |= 0x1 << 3;
}

int main(void)
{
	init_leds();
    
	init_key();     
	init_Int();     
    
	while (1){
	if (flag = 1)
	{led_from_left_to_right();}
	else if (flag = 0)
	{GPJ2DAT |=0xF;}	
}      
	
}