#include "CANlibF3.h"

const int stronaPlytka = 1; //1-prawa 0-lewa

//==================================================================================================
//Funkcja inicjalizuje CAN na pinach PA11 CAN_RX / PA12 CAN_TX
void initCan(void) {
	//inicjalizacja struktur
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	/* NVIC configuration ***********************************/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//uruchomienie zegarów
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	//konfiguracja pinów CAN   PA11 CAN_RX / PA12 CAN_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_9);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_9);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//konfiguracja CAN
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	//ustawienie predkosci transmisji
	CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 4;

	//konfiguracja filtrow CAN
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;

	//wybór strony
	if (stronaPlytka == 1) {
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x2460;
	} else {
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x2480;
	}
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;

	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

	//Inicjalizacja CAN i filtrow
	CAN_DeInit(CAN1);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	//CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
	CAN_Init(CAN1, &CAN_InitStructure);
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Transmit Structure preparation */
	TxMessage.StdId = 0x0;
	TxMessage.ExtId = 0x0;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 1;
}

void readSpeed(void); //deklaracja funkcji znajdujacej sie nizej
void pwmStartStop(void);
void readPid(void);
//==================================================================================================
//przerwanie odbiorcze CAN
//odczytuje wartosci predkosci z nadeslanej ramki i ustawia rzadana predkosc na odpowiednich silnikach
void USB_LP_CAN1_RX0_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		switch (RxMessage.Data[0]) {
		case 'v':
			readSpeed();
			break;
		case 's':
			pwmStartStop();
			break;
		}

	}
}

//==================================================================================================
//Przerwanie nadawcze CAN
void USB_HP_CAN1_TX_IRQHandler(void) {
	if (CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET) {
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}
}

///==================================================================================================
//Ustawia odpowiednie predkosci dla kolejnych silnikow wedlug zawartosci odebranej ramki CAN
void readSpeed() {
	static volatile int8_t read1;
	static volatile int8_t read2;
	static volatile int8_t read3;
	read1 = RxMessage.Data[1];
	read2 = RxMessage.Data[2];
	read3 = RxMessage.Data[3];
	setPWM(read1*20, read2*20, read3*20);
}

void pwmStartStop() {
	if (RxMessage.Data[1] == 1) {
		startMotors();
	} else if (RxMessage.Data[1] == 0) {
		stopMotors();
	}
}

void sendSpeed(void) {
	TxMessage.StdId = 0x125;
	TxMessage.DLC = 7;
	if (stronaPlytka == 1) {
		TxMessage.Data[0] = 'V';
	} else {
		TxMessage.Data[0] = 'v';
	}
	TxMessage.Data[1] = enkPredkosc1 & 0xFF;
	TxMessage.Data[2] = (enkPredkosc1 & 0xFF00) >> 8;
	TxMessage.Data[3] = enkPredkosc2 & 0xFF;
	TxMessage.Data[4] = (enkPredkosc2 & 0xFF00) >> 8;
	TxMessage.Data[5] = enkPredkosc3 & 0xFF;
	TxMessage.Data[6] = (enkPredkosc3 & 0xFF00) >> 8;
	CAN_Transmit(CAN1, &TxMessage);
}

void sendCurrent(void) {
	TxMessage.StdId = 0x125;
	TxMessage.DLC = 7;
	if (stronaPlytka == 1) {
		TxMessage.Data[0] = 'I';
	} else {
		TxMessage.Data[0] = 'i';
	}
	TxMessage.Data[1] = currentValue[0] & 0xFF;
	TxMessage.Data[2] = (currentValue[0] & 0xFF00) >> 8;
	TxMessage.Data[3] = currentValue[1] & 0xFF;
	TxMessage.Data[4] = (currentValue[1] & 0xFF00) >> 8;
	TxMessage.Data[5] = currentValue[2] & 0xFF;
	TxMessage.Data[6] = (currentValue[2] & 0xFF00) >> 8;

	CAN_Transmit(CAN1, &TxMessage);
}

void sendParam(void) {
	lowPassFilterFIR();
	static int licznik = -1;
	if (licznik == -1 && stronaPlytka == 1) {
		licznik = 25;
	}
	licznik++;
	switch (licznik) {
	case 50:
		sendSpeed();
		break;
	case 100:
		sendCurrent();
		licznik = 0;
		break;
	}
}
