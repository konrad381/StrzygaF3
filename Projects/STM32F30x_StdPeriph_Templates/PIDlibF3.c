#include "PIDlibF3.h"

uint16_t currentGainP = 8;
uint16_t currentGainI = 430;
uint16_t currentGainK = 1;

volatile int16_t velocityRegOut[3];
volatile int16_t velocityRegIntegral[3];
volatile int16_t velocityRegError[3];
volatile int16_t velocityRegSaturation[3];

void setPIVelocity(void) {

	for (int i = 0; i < 3; i++) {
		velocityRegError[i] = (referenceVelocity[i] - enkPredkosc[i])
				* velocityGainP;
		velocityRegIntegral[i] += (velocityRegError[i]
				- (velocityRegSaturation[i] * velocityGainK)) * velocityGainI;
		velocityRegOut[i] = velocityRegError[i] + velocityRegIntegral[i];
		if (velocityRegOut[i] > 2000) {
			velocityRegSaturation[i] = velocityRegOut[i] - 2000;
			velocityRegOut[i] = 2000;
		} else if (velocityRegOut[i] < -2000) {
			velocityRegSaturation[i] = velocityRegOut[i] + 2000;
			velocityRegOut[i] = -2000;
		}
	}
	setReferenceCurrent(velocityRegOut[0], velocityRegOut[1],
			velocityRegOut[2]);
}

volatile int16_t currentRegOut[3];
volatile int16_t currentRegIntegral[3];
volatile int16_t currentRegError[3];
volatile int16_t currentRegSaturation[3];

void setPICurrent(void) {
	for (int i = 0; i < 3; i++) {
		currentRegError[i] = (referenceCurrent[i] - currentValue[i])
				* currentGainP;
		currentRegIntegral[i] += (currentRegError[i]
				- (currentRegSaturation[i] * currentGainK)) * currentGainI;
		currentRegOut[i] = currentRegError[i] + currentRegIntegral[i];
		if (currentRegOut[i] > 2000) {
			currentRegSaturation[i] = currentRegOut[i] - 2000;
			currentRegOut[i] = 2000;
		} else if (currentRegOut[i] < -2000) {
			currentRegSaturation[i] = currentRegOut[i] + 2000;
			currentRegOut[i] = -2000;
		}
	}
	setPWM(0, 0, currentRegOut[2]);
}

void setReferenceCurrent(int16_t current1, int16_t current2, int16_t current3) {
	referenceCurrent[0] = current1;
	referenceCurrent[1] = current2;
	referenceCurrent[2] = current3;
}

void setReferenceVelocity(int16_t velocity1, int16_t velocity2,
		int16_t velocity3) {
	referenceVelocity[0] = velocity1;
	referenceVelocity[1] = velocity2;
	referenceVelocity[2] = velocity3;
}

void resetIntegralValue(void) {
	setPWM(0,0,0);
	for (int i = 0; i < 3; i++) {
		currentRegIntegral[i] = 0;
		currentRegSaturation[i] = 0;
		referenceCurrent[i] = 0;
		velocityRegIntegral[i] = 0;
		velocityRegSaturation[i] = 0;
		referenceVelocity[i] = 0;
	}
}
