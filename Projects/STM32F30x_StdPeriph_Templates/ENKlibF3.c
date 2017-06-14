#include "ENKlibF3.h"

void initENK(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef gpio;

	//=========piny pomocnicze (odczyt kierunku)================================
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &gpio);

	//==========enkoder1===========================================
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV4;
	TIM_ICInitStructure.TIM_ICFilter = 0x1;

	TIM_PrescalerConfig(TIM2, 250, TIM_PSCReloadMode_Immediate);
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

	TIM_SetCompare1(TIM2, 0x0000FFFF);
	TIM_SelectOCxM(TIM2, TIM_Channel_1, TIM_OCMode_Retrigerrable_OPM1);

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	//==========enkoder2===========================================
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV4;
	TIM_ICInitStructure.TIM_ICFilter = 0x1;

	TIM_PrescalerConfig(TIM16, 250, TIM_PSCReloadMode_Immediate);
	TIM_ICInit(TIM16, &TIM_ICInitStructure);
	TIM_Cmd(TIM16, ENABLE);
	TIM_ITConfig(TIM16, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

	//==========enkoder3===========================================
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_10);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM17_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV4;
	TIM_ICInitStructure.TIM_ICFilter = 0x1;

	TIM_PrescalerConfig(TIM17, 250, TIM_PSCReloadMode_Immediate);
	TIM_ICInit(TIM17, &TIM_ICInitStructure);
	TIM_Cmd(TIM17, ENABLE);
	TIM_ITConfig(TIM17, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);

}

//********************************************PRZERWANIA*****************************
//Obliczenie wartoœci prêdkoœci:
//sysClock /PrescalerTim *PrescalerInput /liczba impulsów *60(sekund)
//zeby byly obroty na minute 230400
//==========enkoder1===============================================
void TIM2_IRQHandler(void) {
	TIM_SetCounter(TIM2, 0);

	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

		enkPredkosc1 = 4608000 /TIM_GetCapture2(TIM2);

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) {
			enkPredkosc1 = (-enkPredkosc1);
		}
	} else if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		enkPredkosc1 = 0;
	}
}
//==========enkoder2===============================================
void TIM1_UP_TIM16_IRQHandler(void) {
	TIM_SetCounter(TIM16, 0);

	if (TIM_GetITStatus(TIM16, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM16, TIM_IT_CC1);

		enkPredkosc2 = 4608000/ TIM_GetCapture1(TIM16);

		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 0) {
			enkPredkosc2 = (-enkPredkosc2);
		}
	} else if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
		enkPredkosc2 = 0;
	}
}
//==========enkoder3===============================================
void TIM1_TRG_COM_TIM17_IRQHandler(void) {
	TIM_SetCounter(TIM17, 0);

	if (TIM_GetITStatus(TIM17, TIM_IT_CC1) != RESET) {
		TIM_ClearITPendingBit(TIM17, TIM_IT_CC1);

		enkPredkosc3 = 4608000/ TIM_GetCapture1(TIM17);

		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 0) {
			enkPredkosc3 = (-enkPredkosc3);
		}
	} else if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
		enkPredkosc3 = 0;
	}
}
