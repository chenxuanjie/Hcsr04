#include "stm32f10x.h"                  // Device header
#include "Hcsr04.h"
#include "OLED.h"
#include "Delay.h"
#include "USART.h"

uint16_t Num,i,j;
uint32_t x1, x2, x3, x4;

int main(void)
{	
	USART1_Config();
	Hcsr04_Init();
	OLED_Init();
	printf("ok\n");
	OLED_ShowString(1, 1, "Distance1:  . cm");
	OLED_ShowString(2, 1, "Distance2:  . cm");
	OLED_ShowString(3, 1, "Distance3:  . cm");
	OLED_ShowString(4, 1, "Distance4:  . cm");
	while(1)
	{		
//		Num++;
//		OLED_ShowNum(4 , 14, Num ,3);
		
		x1 = Hcsr04_GetDistance1();
		Delay_ms(100);
		x2 = Hcsr04_GetDistance2();
		Delay_ms(100);
		x3 = Hcsr04_GetDistance3();
		Delay_ms(100);
		x4 = Hcsr04_GetDistance4();
		OLED_ShowNum(1 , 11, x1/10 ,2);
		OLED_ShowNum(2 , 11, x2/10 ,2);
		OLED_ShowNum(3 , 11, x3/10 ,2);
		OLED_ShowNum(4 , 11, x4/10 ,2);
		OLED_ShowNum(1 , 14, x1%10 ,1);
		OLED_ShowNum(2 , 14, x2%10 ,1);
		OLED_ShowNum(3 , 14, x3%10 ,1);
		OLED_ShowNum(4 , 14, x4%10 ,1);
		
		Delay_ms(100);
		
	}
}
