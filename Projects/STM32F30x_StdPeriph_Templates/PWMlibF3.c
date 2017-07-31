#include "PWMlibF3.h"

TIM_OCInitTypeDef channel;

//==================================================================================================
//Ustawia pedkosci kolejnych silnikow (wypelnieniePWM)
void setPWM(int16_t duty1, int16_t duty2, int16_t duty3) {
	if (duty1 > 0) {
		channel.TIM_Pulse = 0;
		TIM_OC1Init(TIM15, &channel);
		channel.TIM_Pulse = duty1;
		TIM_OC2Init(TIM15, &channel);
	} else {
		channel.TIM_Pulse = -duty1;
		TIM_OC1Init(TIM15, &channel);
		channel.TIM_Pulse = 0;
		TIM_OC2Init(TIM15, &channel);
	}
	if (duty2 > 0) {
		channel.TIM_Pulse = 0;
		TIM_OC1Init(TIM1, &channel);
		channel.TIM_Pulse = duty2;
		TIM_OC2Init(TIM1, &channel);
	} else {
		channel.TIM_Pulse = -duty2;
		TIM_OC1Init(TIM1, &channel);
		channel.TIM_Pulse = 0;
		TIM_OC2Init(TIM1, &channel);
	}
	if (duty3 > 0) {
		channel.TIM_Pulse = 0;
		TIM_OC3Init(TIM1, &channel);
		channel.TIM_Pulse = duty3;
		TIM_OC4Init(TIM1, &channel);
	} else {
		channel.TIM_Pulse = -duty3;
		TIM_OC3Init(TIM1, &channel);
		channel.TIM_Pulse = 0;
		TIM_OC4Init(TIM1, &channel);
	}
}

//==================================================================================================
//Funkcja odblokowuje wszystkie silniki
void startMotors() {
	//Motor2
	//D1
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	//D2
	GPIO_SetBits(GPIOB, GPIO_Pin_11);

	//Motor1
	//D1
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	//D2
	GPIO_SetBits(GPIOC, GPIO_Pin_7);

	//Motor3
	//D1
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
	//D2
	GPIO_SetBits(GPIOC, GPIO_Pin_15);

	channel.TIM_Pulse = 0;
	TIM_OC1Init(TIM15, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC2Init(TIM15, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC3Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC4Init(TIM1, &channel);

	resetIntegralValue();
}

//==================================================================================================
//Funkcja zatrzymuje wszystkie silniki
void stopMotors() {
	//Motor2
	//D1
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	//D2
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);

	//Motor1
	//D1
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	//D2
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);

	//Motor3
	//D1
	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
	//D2
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);

	channel.TIM_Pulse = 0;
	TIM_OC1Init(TIM15, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC2Init(TIM15, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC3Init(TIM1, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC4Init(TIM1, &channel);

}

//==================================================================================================
//funkcja inicjalizuje wszystkie niezbedne Timery w tryb PWM
void initPwm() {

	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim1;
	TIM_TimeBaseInitTypeDef tim15;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	//uruchamiamy szynê A
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	//uruchamiamy szynê B
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);	//uruchamiamy szynê C

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);

//Motor2 tim1 ch1 ch2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_6);
//Motor3 tim1 ch3 ch4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_2);
//Motor1	tim 15 ch1 ch2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_9);
//Motor2
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &gpio);
//motor3
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &gpio);
//Motor1
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &gpio);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);

	TIM_TimeBaseStructInit(&tim1);
	tim1.TIM_CounterMode = TIM_CounterMode_Up;
	tim1.TIM_Prescaler = 1;
	tim1.TIM_Period = 2000;
	TIM_TimeBaseInit(TIM1, &tim1);

	TIM_TimeBaseStructInit(&tim15);
	tim15.TIM_CounterMode = TIM_CounterMode_Up;
	tim15.TIM_Prescaler = 1;
	tim15.TIM_Period = 2000;
	TIM_TimeBaseInit(TIM15, &tim15);

	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;

	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	TIM_Cmd(TIM15, ENABLE);
	TIM_CtrlPWMOutputs(TIM15, ENABLE);

	resetIntegralValue();
}

//==================================================================================================
//przerwanie z regulatorem PI pr¹du
void TIM1_BRK_TIM15_IRQHandler(void) {
	if (TIM_GetITStatus(TIM15, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
		//lowPassFilterSimple();
		//setPWM(referenceCurrent[0]*20, referenceCurrent[1]*20, referenceCurrent[2]*20);
	}
}
