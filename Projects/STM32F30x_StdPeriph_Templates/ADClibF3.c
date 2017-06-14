#include "ADClibF3.h"

//----------------------------------------------------------------------------------
/**Funkcja inicjalizuje pomiar napiêcia ADC na trzech kana³ach (PA4, PB13, PC0) i przesy³anie ich za pomoc¹ DMA1 chanel1 do
 * zmiennej "adcWartosc[]" -> tablica 3 elementowa
 *
 */
void initADC(void) {
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
//	 NVIC_InitTypeDef NVIC_InitStructure;
	volatile int i;

	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//ADC1 IN_5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ADC1 IN_2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ADC1 IN_6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC12_CDR_ADDRESS;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &adcRawData[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	ADC_VoltageRegulatorCmd(ADC1, ENABLE);
	/* Insert delay equal to 10 µs */
	for (i = 0; i < 10000; i++)
		;

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_RegSimul;
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_SynClkModeDiv1;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; // 12-bit
	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 1;

	ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ExternalTrigEventEdge =
	ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;
	ADC_InitStructure.ADC_NbrOfRegChannel = 3;
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_61Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_61Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_61Cycles5);

	ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY))
		;

	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_StartConversion(ADC1);
}

float wynik = 0;

void lowPassFiletr(void) {
	for (int j = 0; j < 3; j++) {
		wynik = 0;
		for (int i = 9; i > 0; i++) {
			wynik += filtrOldSamples[j][i] * filtrCoefficients[i];
			filtrOldSamples[j][i] = filtrOldSamples[j][i - 1];
		}
		wynik += filtrOldSamples[j][0] * filtrCoefficients[0];
		filtrOldSamples[j][0] = adcRawData[j];
		adcWartosc[j] = (int16_t) wynik - offset;
	}
}

//void DMA1_Channel1_IRQHandler(void) {
//	if (DMA_GetITStatus(DMA_IT_TC) != RESET) {
//		//lowPassFiletr();
//		DMA_ClearITPendingBit(DMA_IT_TC);
//	}
//}
