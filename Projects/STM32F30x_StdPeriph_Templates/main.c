#include "main.h"
#include "CANlibF3.h"
#include "PWMlibF3.h"
#include "GPIOlibF3.h"
#include "ADClibF3.h"
#include "ENKlibF3.h"
#include "PIDlibF3.h"
#include "UARTlibF3.h"
#include "EXTIlibF3.h"

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


	while (1) {
	}
}

//==================================================================================================
void Delay(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}

//==================================================================================================
void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}

	regulatorSilnikow();
	sendParam();
}

