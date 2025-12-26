#ifndef __MYI2C_H
#define __MYI2C_H
#include "stm32f10x.h"                  // Device heade
#include "delay.h"
 
#define QMC_PORT GPIOB
#define QMC_SCL_Pin GPIO_Pin_10
#define QMC_SDA_Pin GPIO_Pin_11
 
 
void QMC_IIC_Init(void);
void QMC_IIC_Start(void);
void QMC_IIC_Stop(void);
void QMC_IIC_Send_Byte(unsigned char byte);
unsigned char  QMC_IIC_Receive_Byte(void);
void QMC_IIC_Send_ACK(unsigned char ACK);
uint8_t QMC_IIC_ReceiveAck(void);
#endif