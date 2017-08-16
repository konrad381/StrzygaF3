#ifndef EXTIlibF3_H
#define EXTIlibF3_H

#include "stm32f30x.h"
#include "main.h"
#include "ENKlibF3.h"

void initEXTI(void);
void enkSetZero(void);
void enkReadRawData(void);

void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

#endif
