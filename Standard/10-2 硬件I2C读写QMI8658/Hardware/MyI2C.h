#ifndef __MYI2C_H
#define __MYI2C_H


void MyI2C_Init();
void MyI2C_Start();
void MyI2C_SendByte(uint8_t Byte);
void MyI2C_SendAck(uint8_t Ack );
uint8_t  MyI2C_ReceiveByte();
uint8_t MyI2C_ReceiveAck();
void MyI2C_Stop();

#endif
