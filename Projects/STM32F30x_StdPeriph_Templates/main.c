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
	//initCan();
	//initENK();
	initGpioMotors();
	initPwm();
	initADC();
	initUART();
	startMotors();

	while (1) {
		Delay(50);
		uint8_t bufor[10];
		bufor[0] = '#';
		bufor[1] = (adcWartosc[0] & 0xFF);
		bufor[2] = ((adcWartosc[0] & 0xFF00) >> 8);
		bufor[3] = (adcWartosc[1] & 0xFF);
		bufor[4] = ((adcWartosc[1] & 0xFF00) >> 8);
		bufor[5] = (adcWartosc[2] & 0xFF);
		bufor[6] = ((adcWartosc[2] & 0xFF00) >> 8);

		UART2wyslij(&bufor[0], 7);
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
	//setPID();
	//sendParam();
}

