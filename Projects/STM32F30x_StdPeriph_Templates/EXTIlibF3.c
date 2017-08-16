#include "EXTIlibF3.h"

void initEXTI(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	enkSetZero();
}

void enkSetZero(void) {
	for (int i = 0; i < 3; i++) {
		enkRawData[i] = 0;
	}
}

void enkReadRawData(void) {
	for (int i = 0; i < 3; i++) {
		enkPredkosc[i] = enkRawData[i];
	}
}

void EXTI3_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line3);
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) {
			//enkRawData[2]++;
		} else {
			//enkRawData[1]--;
		}
	}
}
void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line4);
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 0) {
		//	enkRawData[1]++;
		} else {
			//enkRawData[1]--;
		}
	}
}
void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line5);
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_SET) {
			enkRawData[0]++;
		} else {
			enkRawData[1]++;
		}
	}
}
