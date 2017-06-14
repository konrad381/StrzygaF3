/**
 * autor: Konrad Aleksiejuk Data: 24.11.16r
 * Bibliotek do pomiaru napi�cia (proporcjonalnego do pr�du) w sterownikach silnik�w
 * pomiar na 3 kana�ach, dane przesy�ane za pomoc� DMA do tablicy: "adcWartosc[]"
 * Po inicjalizacji funkcja praktycznie nie zajmuje procesora
 *
 */

#ifndef ADClibF3_H
#define ADClibF3_H

#include "stm32f30x.h"

#define ADC12_CDR_ADDRESS    ((uint32_t)0x5000030C)
static const float filtrCoefficients[]={0.0032336752634440106, 0.040192824366562636, 0.084067149188913476, 0.13912375443508521, 0.18360264434522341, 0.20077628315128423, 0.18360264434522341, 0.13912375443508521, 0.084067149188913476, 0.040192824366562636, 0.0032336752634440106};
static const int16_t offset = 1500;

void initADC(void);
void DMA1_Channel1_IRQHandler(void);
void lowPassFiletr(void);

volatile int16_t adcWartosc[3];
volatile uint16_t adcRawData[3];
volatile int filtrOldSamples[3][10];

#endif
