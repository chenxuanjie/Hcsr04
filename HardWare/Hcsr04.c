#include "stm32f10x.h"                  // Device header
#include "Hcsr04.h"
#include "Delay.h"
#include "TIM4.h"
#include "USART.h"
#include "OLED.h"

uint8_t Up_Down_Flag1, Up_Down_Flag2, Up_Down_Flag3, Up_Down_Flag4;
uint32_t Hcsr04_Value1, Hcsr04_Value2, Hcsr04_Value3, Hcsr04_Value4;
uint32_t T4Count[4] = {0};

void Hcsr04_Init(void)
{
	RCC_APB2PeriphClockCmd(HCSR04_APBPERIPH | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO1 | HCSR04_ECHO2 | HCSR04_ECHO3 | HCSR04_ECHO4;
	GPIO_Init(HCSR04_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG2 | HCSR04_TRIG3 | HCSR04_TRIG4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HCSR04_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG1;
	GPIO_Init(HCSR04_TRIG1_GPIO, &GPIO_InitStructure);
			
	TIM4_Init();
	GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG1);
	GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG2);
	GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG3);
	GPIO_ResetBits(HCSR04_GPIO, HCSR04_TRIG4);
}

uint32_t Hcsr04_GetDistance1(void)
{
	static uint32_t Distance;
	if (Hcsr04_Value1 == 0)		//数据已经处理过或第一次调用
	{
		GPIO_WriteBit(HCSR04_TRIG1_GPIO, HCSR04_TRIG1, Bit_SET);
		Delay_us(15);
		GPIO_WriteBit(HCSR04_TRIG1_GPIO, HCSR04_TRIG1, Bit_RESET);	
	}
//	Delay_ms(20);
	if (Hcsr04_Value1!=0 && Up_Down_Flag1==0)
	{
		Distance = T4Count[0] * 65536;
		Distance += Hcsr04_Value1;
		Distance = Distance*170/1000;	//单位mm
		printf("Distance1:%d\n", Distance);
		Hcsr04_Value1 = 0;
	}
	return Distance;
}

uint32_t Hcsr04_GetDistance2(void)
{
	static uint32_t Distance;	
	if (Hcsr04_Value2 == 0)		//数据已经处理过或第一次调用
	{
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG2, Bit_SET);
		Delay_us(15);
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG2, Bit_RESET);
	}
	if (Hcsr04_Value2!=0 && Up_Down_Flag2==0)
	{
		Distance = T4Count[1] * 65536;
		Distance += Hcsr04_Value2;
		Distance = Distance*170/1000;	//单位mm
		printf("Distance2:%d\n", Distance);
		Hcsr04_Value2 = 0;
	}
	return Distance;
}

uint32_t Hcsr04_GetDistance3(void)
{
	static uint32_t Distance;	
	if (Hcsr04_Value3 == 0)		//数据已经处理过或第一次调用
	{
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG3, Bit_SET);
		Delay_us(15);
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG3, Bit_RESET);
	}
	if (Hcsr04_Value3!=0 && Up_Down_Flag3==0)
	{
		Distance = T4Count[2] * 65536;
		Distance += Hcsr04_Value3;
		Distance = Distance*170/1000;	//单位mm
		printf("Distance3:%d\n", Distance);
		Hcsr04_Value3 = 0;
	}
	return Distance;
}

uint32_t Hcsr04_GetDistance4(void)
{
	static uint32_t Distance;	
	if (Hcsr04_Value3 == 0)		//数据已经处理过或第一次调用
	{
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG4, Bit_SET);
		Delay_us(15);
		GPIO_WriteBit(HCSR04_GPIO, HCSR04_TRIG4, Bit_RESET);
	}
//	Delay_ms(1);
	if (Hcsr04_Value4!=0 && Up_Down_Flag4==0)
	{
		Distance = T4Count[3] * 65536;
		Distance += Hcsr04_Value4;
		Distance = Distance*170/1000;	//单位mm
		printf("Distance4:%d\n", Distance);
		Hcsr04_Value4 = 0;
	}
	return Distance;
}
void TIM4_IRQHandler(void)
{	
	//定时器中断
	if(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == SET)	//溢出加一
	{
		if (Up_Down_Flag1 == 1)
		{
			T4Count[0]++;
			printf("%d",T4Count[0]);
		}
		if (Up_Down_Flag2 == 2)
		{
			T4Count[1]++;
			printf("%d",T4Count[1]);
		}
		if (Up_Down_Flag3 == 3)
		{
			T4Count[2]++;
			printf("%d",T4Count[2]);
		}
		if (Up_Down_Flag4 == 4)
		{
			T4Count[3]++;
			printf("%d",T4Count[3]);
		}
	}	
	//超声波模块1边沿中断	
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		if (Up_Down_Flag1 == 0)	//上升沿
		{
			Up_Down_Flag1 = 1;
			T4Count[0] = 0;
			TIM_SetCounter(TIM4, 0);
			Hcsr04_Value1 = 0;
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);
		}
		else 	//下降沿，或是成功捕获
		{
			Up_Down_Flag1 = 0;
			Hcsr04_Value1 = TIM_GetCapture1(TIM4);
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising);
		}
	}
	//超声波模块2边沿中断	
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		if (Up_Down_Flag2 == 0)	//上升沿
		{
			Up_Down_Flag2 = 1;
			T4Count[1] = 0;
			TIM_SetCounter(TIM4, 0);
			Hcsr04_Value2 = 0;
			TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);
		}
		else 	//或是成功捕获
		{
			Up_Down_Flag2 = 0;
			Hcsr04_Value2 = TIM_GetCapture2(TIM4);
			TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising);
		}
	}
	//超声波模块3边沿中断	
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC3) != RESET)
	{
		if (Up_Down_Flag3 == 0)	//上升沿
		{
			Up_Down_Flag3 = 1;
			T4Count[2] = 0;
			TIM_SetCounter(TIM4, 0);
			Hcsr04_Value3 = 0;
			TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);
		}
		else 	//或是成功捕获
		{
			Up_Down_Flag3 = 0;
			Hcsr04_Value3 = TIM_GetCapture3(TIM4);
			TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising);
		}
	}
	//超声波模块4边沿中断	
	if(TIM_GetFlagStatus(TIM4, TIM_IT_CC4) != RESET)
	{
		if (Up_Down_Flag4 == 0)	//上升沿
		{
			Up_Down_Flag4 = 1;
			T4Count[3] = 0;
			TIM_SetCounter(TIM4, 0);
			Hcsr04_Value4 = 0;
			TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Falling);
		}
		else 	//或是成功捕获
		{
			Up_Down_Flag4 = 0;
			Hcsr04_Value4 = TIM_GetCapture4(TIM4);
			TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Rising);
		}
	}
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4);	
}
