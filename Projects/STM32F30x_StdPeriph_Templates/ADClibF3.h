/**
 * autor: Konrad Aleksiejuk Data: 24.11.16r
 * Bibliotek do pomiaru napiêcia (proporcjonalnego do pr¹du) w sterownikach silników
 * pomiar na 3 kana³ach, dane przesy³ane za pomoc¹ DMA do tablicy: "adcWartosc[]"
 * Po inicjalizacji funkcja praktycznie nie zajmuje procesora
 *
 */

#ifndef ADClibF3_H
#define ADClibF3_H

#include "stm32f30x.h"

#define ADC12_CDR_ADDRESS    ((uint32_t)0x5000030C)

void initADC(void);

volatile int16_t adcValue[3];
volatile int16_t currentValue[3];

void lowPassFilterFIR(void);
void lowPassFilterIIR(void);
// Do filtru dolnoprzepustowego
static const float filtrCoefficients[]={0.0032336752634440106, 0.040192824366562636, 0.084067149188913476, 0.13912375443508521, 0.18360264434522341, 0.20077628315128423, 0.18360264434522341, 0.13912375443508521, 0.084067149188913476, 0.040192824366562636, 0.0032336752634440106};
static const int16_t offset[] = { 1800, 1800, 1800 };
volatile int16_t oldCurrentSamples[3][10];

static const float alfa = 0.1;

#endif
