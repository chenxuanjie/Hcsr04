#include "stm32f10x.h"                  // Device header
#include "Hcsr04.h"
#include "Delay.h"
#include "TIM4.h"
#include "USART.h"

uint8_t Up_Down_Flag;
uint32_t Hcsr04_Value;
uint32_t T4Count[2] = {0};

void Hcsr04_Init(void)
{
	RCC_APB2PeriphClockCmd(HCSR04_APBPERIPH, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;
	GPIO_Init(HCSR04_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO, &GPIO_InitStructure);
			
	TIM4_Init();
	GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG);
}

uint32_t Hcsr04_GetDistance(void)
{
	static uint32_t Distance;
	if (Hcsr04_Value == 0)		//数据已经处理过或第一次调用
	{
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG, Bit_SET);
		Delay_us(15);
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG, Bit_RESET);	
	}
//	Delay_ms(20);
	if (Hcsr04_Value!=0 && Up_Down_Flag==0)
	{
		Distance = T4Count[0] * 65536;
		Distance += Hcsr04_Value;
		Distance = Distance*170/1000;	//单位mm
		printf("Distance1:%d\n", Distance);
		Hcsr04_Value = 0;
	}
	return Distance;
}

void TIM4_IRQHandler(void)
{	
	//定时器中断
	if (Up_Down_Flag == 1)
	{
		if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == SET)	//溢出加一
		{
			T4Count[0]++;
			printf("%d",T4Count[0]);
		}	
	}
	//软件上升沿中断	
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		if (Up_Down_Flag == 0)	//上升沿
		{
			Up_Down_Flag = 1;
			T4Count[0] = 0;
			TIM_SetCounter(TIM4, 0);
			Hcsr04_Value = 0;
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);
		}
		else 	//下降沿，或是成功捕获
		{
			Up_Down_Flag = 0;
			Hcsr04_Value = TIM_GetCapture1(TIM4);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising);
		}
	}
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update | TIM_IT_CC1);	
}
