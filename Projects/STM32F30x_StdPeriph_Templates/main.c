//PROJEKT TESTOWY!!!!!!

#include "main.h"
#include "CANlibF3.h"
#include "PWMlibF3.h"
#include "GPIOlibF3.h"
#include "ADClibF3.h"
#include "ENKlibF3.h"
#include "PIDlibF3.h"
#include "UARTlibF3.h"

static __IO uint32_t TimingDelay;
void Delay(__IO uint32_t nTime);
RCC_ClocksTypeDef RCC_Clocks;
void RCC_Conf(void);

//==================================================================================================
int main(void) {
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1)
			;
	}
	initCan();
	initENK();
	initGpioMotors();
	initGpioLed();
	initPwm();
	initADC();
	//initUART();
	//startMotors();

	while (1) {
		Delay(50);
//		for(int i=0;i<3;i++){
//			adcRawData[i]=adcRawData[i]+(adcWartosc[i]-adcRawData[i]);
//			}

//		uint8_t bufor[10];
//		bufor[0] = '#';
//		bufor[1] = (currentValue[1] & 0xFF);
//		bufor[2] = ((currentValue[1] & 0xFF00) >> 8);
//		bufor[3] = (currentValue[2] & 0xFF);
//		bufor[4] = ((currentValue[2] & 0xFF00) >> 8);
//		bufor[5] = (currentValueRawADC[2] & 0xFF);
//		bufor[6] = ((currentValueRawADC[2] & 0xFF00) >> 8);
//
//		UART2wyslij(&bufor[0], 7);
//
//		uint8_t bufor[10];
//		bufor[0] = '#';
//		bufor[1] = (enkPredkosc1 & 0xFF);
//		bufor[2] = ((enkPredkosc1 & 0xFF00) >> 8);
//		bufor[3] = (enkPredkosc3 & 0xFF);
//		bufor[4] = ((enkPredkosc3 & 0xFF00) >> 8);
//		bufor[5] = (enkTest & 0xFF);
//		bufor[6] = ((enkTest & 0xFF00) >> 8);
//
//		UART2wyslij(&bufor[0], 7);
	}
}

//==================================================================================================
void Delay(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}

void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
	sendParam();
}

