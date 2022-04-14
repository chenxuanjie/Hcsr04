#ifndef __HCSR04_H__
#define __HCSR04_H__

#define HCSR04_APBPERIPH		RCC_APB2Periph_GPIOB
#define HCSR04_GPIO				GPIOB
#define HCSR04_TRIG				GPIO_Pin_5
#define HCSR04_ECHO				GPIO_Pin_6

void Hcsr04_Init(void);
uint32_t Hcsr04_GetDistance(void);

#endif
