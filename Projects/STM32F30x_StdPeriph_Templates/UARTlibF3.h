#ifndef UARTlibF3_H
#define UARTlibF3_H

#include "stm32f30x.h"
#include "PWMlibF3.h"

void initUART();
void sendData();
void USART1_IRQHandler();
void UART2wyslij(uint8_t* dataToSend, uint8_t dlugosc);
void wykonajPolecenie(void);

volatile char polecenie[20];

typedef struct {
	volatile uint8_t *sendBuffor;
	volatile uint8_t dataLenght;
} sendData_TypeDef;

volatile sendData_TypeDef sendDataArray[10];

#endif
