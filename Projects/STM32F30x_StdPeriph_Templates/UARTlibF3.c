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
		} else {
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





//
//
//
//
//
//
//
//#define MLX90614_ADDR (0b1101000 << 1)
//
//
//void MLX90614_Init(void) {
//	// RCC setup
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//
//	// TODO: enable sensor's power on Unit5
//
//
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_4);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_4);
//	// I2C1 SCL(PB6), SDA(PB7) pins setup
//	GPIO_InitTypeDef gpio_init;
//	gpio_init.GPIO_Mode = GPIO_Mode_AF;
//	gpio_init.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &gpio_init);
//
//
//	// I2C1 setup
//	I2C_DeInit(I2C1);
//	I2C_InitTypeDef I2C_InitStruct;
//	I2C_StructInit(&I2C_InitStruct);
//	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
//	//I2C_InitStruct.I2C_ClockSpeed = 100000;
//	//I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStruct.I2C_Timing = 100000;
//	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
//	I2C_Init(I2C1, &I2C_InitStruct);
//
//	// Enable I2C1
//	I2C_Cmd(I2C1, ENABLE);
//
//	// Wait for I2C1 to become ready
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//}
//
///**
// * @brief This function reads a single 16-bit value from the device's register
// * @param reg Register's address
// * @return Register's value
// */
//uint8_t i2c_read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer, uint16_t len)
//{
//  /* Test on BUSY Flag */
//
//  while(I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
//  {
//
//  }
//  I2C_TransferHandling(I2C1, DeviceAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
//
//
//  /* !!! Wait until TXIS flag is set !!! */
//
//  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET) // PROBLEM HERE!!!!!!!!!!!!!!!
//  {
//
//  }
//
//
//}
//
