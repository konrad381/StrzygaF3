#ifndef PIDlibF3_H
#define PIDlibF3_H

#include "stm32f30x.h"
#include "ENKlibF3.h"
#include "CANlibF3.h"
#include "PWMlibF3.h"
#include "EXTIlibF3.h"

typedef struct {
	volatile int16_t referenceValu[3];
	volatile int16_t referenceVelocity[3];
	volatile int16_t referenceCurrent[3];
	volatile int16_t referenceInertial[3];

} regulatorOut_TypeDef;

regulatorOut_TypeDef referenceData;

void resetIntegralValue(void);
void setReferenceSpeed(int16_t speed1, int16_t speed2, int16_t speed3);
void regulatorSilnikow(void);


uint16_t currentGainP;
uint16_t currentGainI;
uint16_t currentGainK;

uint16_t velocityGainP;
uint16_t velocityGainI;
uint16_t velocityGainK;

float inertialGain;

#endif
