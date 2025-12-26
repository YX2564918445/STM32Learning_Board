#ifndef __HMC5883L_H
#define __HMC5883L_H

#include "stm32f10x.h"



//void calibrateHMC5883L(int16_t* offsetX, int16_t* offsetY, int16_t* offsetZ);
void qmc_init(void);
uint8_t QMC_GetID(void);
uint8_t QMC_ReadReg(uint8_t RegAddress);
void QMC_WriteReg(uint8_t RegAddress, uint8_t Data);
void QMC_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ);

#endif

