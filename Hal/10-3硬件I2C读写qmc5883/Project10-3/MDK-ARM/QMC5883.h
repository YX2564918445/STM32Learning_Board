#ifndef QMC5883_H
#define QMC5883_H

#include "stm32f1xx_hal.h"  // 根据实际型号修改，如stm32f1xx_hal.h
#include <stdint.h>

// QMC5883L设备地址（7位地址，HAL库无需左移，内部自动处理读写位）
#define QMC5883L_ADDR         0x0D  // 原标准库中0x1A是左移后的值，7位地址为0x0D

// 寄存器地址定义（与原标准库保持一致）
#define QMC_REG_X_MSB         0x00
#define QMC_REG_X_LSB         0x01
#define QMC_REG_Y_MSB         0x02
#define QMC_REG_Y_LSB         0x03
#define QMC_REG_Z_MSB         0x04
#define QMC_REG_Z_LSB         0x05
#define QMC_REG_STATUS        0x06
#define QMC_REG_CONFIG1       0x09
#define QMC_REG_CONFIG3       0x0B
#define QMC_REG_ID            0x0D  // WHO_AM_I寄存器

// 外部声明I2C句柄（需与实际使用的I2C外设一致，如I2C2）
extern I2C_HandleTypeDef hi2c2;

// 函数声明
uint8_t QMC_ReadReg(uint8_t reg_addr);
void QMC_WriteReg(uint8_t reg_addr, uint8_t data);
uint8_t QMC_GetID(void);
void qmc_init(void);
void QMC_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ);

#endif