/*
 * Autor: Konrad Aleksiejuk & Konrad Husiaty�ski
 * Data:09.12.16r
 * Bibliotek do obs�ugi enkoder�w dla CRAWLERA
 * Informacje o pr�dko�ci odczytywane s� w trzech przerwaniach dla trzech silnik�w
 * Pr�dko�c zapisywana jest w zmiennych: enkPredkosc1, enkPredkosc2, enkPredkosc3
 * dla ujemnej pr�dko�ci zmienna przyjmuje warto�c ujemn�
 * Sygna� wejsciowy ma prescalar 4, sygnal zegarowy prescalar 250
 * Odczytywana predkosc jest wyskalowana w obrotach na minut� [rot/min] (prawdopodobnie ulegnie to zmianie)
 * Sygan� mierzony jest jako czas mi�dzy s�siednimi zboczami narastaj�cymi na jednym z wyj�c enkodera
 *
 * Silnik1 - Enkoder 1 - PB3(okres) - PA15(kierunek) - TIM2
 * Silnik2 - Enkoder 2 - PB4(okres) - PC10(kierunek) - TIM16
 * Silnik3 - Enkoder 3 - PB5(okres) - PC11(kierunek) - TIM17
 *
 */

#ifndef ENKlibF3_H
#define ENKlibF3_H

#include "stm32f30x.h"
#include "main.h"

void initENK(void);

int16_t enkPredkosc1;
int16_t enkPredkosc2;
int16_t enkPredkosc3;

int16_t enkPredkosc[3];

void TIM1_TRG_COM_TIM17_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);
void TIM2_IRQHandler(void);

#endif
