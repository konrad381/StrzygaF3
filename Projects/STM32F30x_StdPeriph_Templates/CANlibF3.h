/*
 * INSTRUKCJA OBS£UGI BIBLIOTEKI
 * 1)na poczatku programu musi zanlezc siê #include "CANlibF3.h"
 * 2)na poczatku programu nalezy zdefiniowac z ktora plytke bedziemy programowac
 * #define Plytka 1  lub  #define Plytka 2    //plytka 1 - prawa   plytak 2 - lewa
 * 3)w funkcji main (poza petla glowna) musi znalezc sie CAN_Config();
 * W razie potrzeby wykorzystanie przerwañ nalezy wprowadzic odpowiednie zmiany w pliku CANlib.c
 *
 * Adresowanie:
 *  Wiadomosc do wszystkich				0x00
 *  Wiadomosc do sterowanika prawego 	0x123
 *  Wiadomosc do sterowanika lewego 	0x124
 *  Adres sterownika glownego			0x125
 */

#ifndef CANlibF3_H
#define CANlibF3_H

#include "stm32f30x.h"
#include "PWMlibF3.h"
#include "PIDlibF3.h"
#include "ADClibF3.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;

volatile int8_t zadPredkosc1;
volatile int8_t zadPredkosc2;
volatile int8_t zadPredkosc3;



void initCan(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void USB_HP_CAN1_TX_IRQHandler(void);
void sendParam(void);

#endif
