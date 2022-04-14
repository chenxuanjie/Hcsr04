#include "stm32f10x.h"                  // Device header
#include "Hcsr04.h"
#include "OLED.h"
#include "Delay.h"
#include "TIM3.h"
#include "USART.h"

uint16_t Num,i,j;
uint32_t x, Distance;

int main(void)
{	
	USART1_Config();
	Hcsr04_Init();
	printf("ok\n");
	while(1)
	{		
	//		Num++;
	//		OLED_ShowNum(4 , 14, Num ,3);
		
//		GPIO_SetBits(HCSR04_GPIO, HCSR04_TRIG);
//		Delay_us(15);
//		GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG);
//		if (Count != 0 && Up_Down_Flag==0)
//		{
//			x = T4Count[0] * 65536;
//			x += Count;
//			x = x*170/1000;	//µ¥Î»mm
//			printf("Distance:%d\n", x);
//			Count = 0;
//		}
		Delay_ms(500);	
		Hcsr04_GetDistance();	
	}
}

