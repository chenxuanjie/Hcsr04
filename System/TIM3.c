#include "stm32f10x.h"                  // Device header

uint32_t T3Count;

void TIM3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);	
}
void TIM3_IRQHandler(void)
{
	//¶¨Ê±Æ÷ÖÐ¶Ï
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) == SET)
	{
		T3Count++;
		if (T3Count >= 1500)
		{
			GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12));
			T3Count = 0;
		}
	}	
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);	
}
