#ifndef PIDlibF3_H
#define PIDlibF3_H

#include "stm32f30x.h"
#include "ENKlibF3.h"
#include "CANlibF3.h"
#include "PWMlibF3.h"

void setPICurrent(void);
void setPIVelocity(void);

void setReferenceCurrent(int16_t current1, int16_t current2, int16_t current3);
void setReferenceVelocity(int16_t velocity1, int16_t velocity2, int16_t velocity3);

void resetIntegralValue(void);

volatile int16_t referenceCurrent[3];
volatile int16_t referenceVelocity[3];

uint16_t currentGainP;
uint16_t currentGainI;
uint16_t currentGainK;

uint16_t velocityGainP;
uint16_t velocityGainI;
uint16_t velocityGainK;

#endif
