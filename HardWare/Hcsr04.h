#ifndef __HCSR04_H__
#define __HCSR04_H__

#define HCSR04_APBPERIPH		RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
#define HCSR04_GPIO				GPIOB
#define HCSR04_TRIG1_GPIO		GPIOA
#define HCSR04_TRIG1			GPIO_Pin_15
#define HCSR04_TRIG2			GPIO_Pin_3
#define HCSR04_TRIG3			GPIO_Pin_4
#define HCSR04_TRIG4			GPIO_Pin_5
#define HCSR04_ECHO1			GPIO_Pin_6
#define HCSR04_ECHO2			GPIO_Pin_7
#define HCSR04_ECHO3			GPIO_Pin_8
#define HCSR04_ECHO4			GPIO_Pin_9


void Hcsr04_Init(void);
uint32_t Hcsr04_GetDistance1(void);
uint32_t Hcsr04_GetDistance2(void);
uint32_t Hcsr04_GetDistance3(void);
uint32_t Hcsr04_GetDistance4(void);

#endif
