#include "PIDlibF3.h"

//Funkcja moze zwracac strukture, struktura moze zawierac Array
float inertialGain = 0.01;

uint16_t currentGainP = 8;
uint16_t currentGainI = 430;
uint16_t currentGainK = 1;


//===============================================================================
int16_t velocityRegOut[3];
volatile int16_t velocityRegIntegral[3];
volatile int16_t velocityRegError[3];
volatile int16_t velocityRegSaturation[3];

void setPIVelocity(volatile int16_t referenceVelocity[3]) {
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
		referenceData.referenceVelocity[i] = velocityRegOut[i];
	}
}


//===============================================================================
int16_t currentRegOut[3];
volatile int16_t currentRegIntegral[3];
volatile int16_t currentRegError[3];
volatile int16_t currentRegSaturation[3];

void setPICurrent(volatile int16_t referenceCurrent[3]) {
	for (int i = 0; i < 3; i++) {
		currentRegError[i] = (*referenceCurrent - currentValue[i])
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
		 referenceData.referenceCurrent[i]=currentRegOut[i];
	}
}


//===============================================================================
void setInertialInput(volatile int16_t referenceInput[3]) {
	for (int i = 0; i < 3; i++) {
		referenceData.referenceInertial[i] += (int16_t) (inertialGain
				* (referenceInput[i]-referenceData.referenceInertial[i]));
	}
}


//===============================================================================
void setReferenceSpeed(int16_t speed1, int16_t speed2, int16_t speed3) {
	referenceData.referenceValu[0] = speed1;
	referenceData.referenceValu[1] = speed2;
	referenceData.referenceValu[2] = speed3;
}


//===============================================================================
void resetIntegralValue(void) {
	setPWM(0, 0, 0);
	for (int i = 0; i < 3; i++) {
		currentRegIntegral[i] = 0;
		currentRegSaturation[i] = 0;
		referenceData.referenceCurrent[i] = 0;
		velocityRegIntegral[i] = 0;
		velocityRegSaturation[i] = 0;
		referenceData.referenceVelocity[i] = 0;
		referenceData.referenceValu[i]=0;
		referenceData.referenceInertial[i]=0;
	}
}


//===============================================================================
void setPWMreg(volatile int16_t referenceDuty[3]) {
	setPWM(referenceDuty[0], referenceDuty[1], referenceDuty[2]);
}


//===============================================================================
void regulatorSilnikow(void) {
	setInertialInput(referenceData.referenceValu);
	setPWMreg(referenceData.referenceInertial);
}
