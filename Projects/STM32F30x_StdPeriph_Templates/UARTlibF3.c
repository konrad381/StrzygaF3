#include "UARTlibF3.h"

void initUART() {
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7);

	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &gpio);

	uart.USART_BaudRate = 115200;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &uart);
	USART_Cmd(USART1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1,USART_IT_TXE,ENABLE);

}

void USART1_IRQHandler() {
	//ODBIERANIE ZNAKÓW
	char inputChar;
	volatile static uint8_t znakiDoOdebrania = 0;
	volatile static uint8_t odbiorRamki = 0;
	volatile static uint8_t licznik = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		inputChar = USART_ReceiveData(USART1);
		if (odbiorRamki > 0) {
			polecenie[licznik] = inputChar;
			licznik++;
			znakiDoOdebrania--;
			if (znakiDoOdebrania == 0) {
				odbiorRamki = 0;
				wykonajPolecenie();
			}
		}
		else {
			if (inputChar == '#') {
				odbiorRamki = 1;
				znakiDoOdebrania = 3;
				licznik = 0;
			}
		}
	}
	//wysylanie znaków
	if (USART_GetITStatus(USART1, USART_IT_TXE)) {
		USART_ClearITPendingBit(USART1, USART_IT_TXE);

		static volatile int k = 0;
		static volatile uint8_t trwaWysylanie = 0;

		if (trwaWysylanie == 1) {
			USART_SendData(USART1, *sendDataArray[k].sendBuffor);
			sendDataArray[k].sendBuffor++;
			sendDataArray[k].dataLenght--;
			if (sendDataArray[k].dataLenght == 0) {
				trwaWysylanie = 0;
			}
		}
		if (trwaWysylanie == 0) {
			for (k = 0; k < 9 && trwaWysylanie == 0;) {
				if (sendDataArray[k].dataLenght > 0) {
					USART_SendData(USART1, *sendDataArray[k].sendBuffor);
					sendDataArray[k].sendBuffor++;
					sendDataArray[k].dataLenght--;
					trwaWysylanie = 1;
				} else {
					k++;
				}
			}
			if (k >= 9) {
				k = 0;
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			}
		}
	}
}

void wykonajPolecenie(void) {

	setReferenceCurrent((int8_t) polecenie[0],(int8_t) polecenie[1], (int8_t)polecenie[2]);
	 setPWM((int8_t) polecenie[0],(int8_t) polecenie[1], (int8_t)polecenie[2]);
}

//===============================================================================
/**
 * @brief  funkcja wysyla dane ze wskazanego adresu tablicy do jednostki sterujacej za pomoca UART2
 * @note   funkcja wpisuje adres pierwszego znaku w tablicy i d³ugosc tabicy do tablicy struktur,
 * 		   która przechowuje dane i wysyla je pokolei( dane nie sa nadpisywane ani tracone)
 * @param 	dataToSend - wskaŸnik do pierwszego elementu tablicy zawieraj¹cej dane do wyslania
 * @param 	dlugosc - liczba znaków które maj¹ zostac wyslane od(1 do 99)
 * @retval None
 */
void UART2wyslij(uint8_t* dataToSend, uint8_t dlugosc) {
	uint8_t koniec = 0;
	for (int l = 0; l < 10 && koniec == 0; l++) {
		if (sendDataArray[l].dataLenght == 0) {
			sendDataArray[l].dataLenght = dlugosc;
			sendDataArray[l].sendBuffor = dataToSend;
			koniec = 1;
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) == RESET) {
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}
