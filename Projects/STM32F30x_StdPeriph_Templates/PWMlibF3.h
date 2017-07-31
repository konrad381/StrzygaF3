#ifndef PWMlibF3_H
#define PWMlibF3_H

#include "stm32f30x.h"
#include "CANlibF3.h"
#include "GPIOlibF3.h"
#include "PIDlibF3.h"
#include "ADClibF3.h"




void setPWM(int16_t duty1, int16_t duty2, int16_t duty3);
void stopMotors(void);
void startMotors(void);
void initPwm(void);
void TIM1_BRK_TIM15_IRQHandler(void);

#endif
