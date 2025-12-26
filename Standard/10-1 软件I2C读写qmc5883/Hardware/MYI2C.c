#include "MYI2C.h"
 
 
 
void QMC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, QMC_SCL_Pin, (BitAction)BitValue);
	delay_us(10);
}
 
void QMC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, QMC_SDA_Pin, (BitAction)BitValue);
	delay_us(10);
}
 
uint8_t QMC_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, QMC_SDA_Pin);
	delay_us(10);
	return BitValue;
}
 
void QMC_IIC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = QMC_SCL_Pin | QMC_SDA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, QMC_SCL_Pin | QMC_SDA_Pin);
	
}
void QMC_IIC_Start(void)
{
	QMC_W_SDA(1);
	QMC_W_SCL(1);
	QMC_W_SDA(0);
	QMC_W_SCL(0);
}
void QMC_IIC_Stop(void)
{
		QMC_W_SDA(0);
	  QMC_W_SCL(1);
	  QMC_W_SDA(1);
}
 
 
void QMC_IIC_Send_Byte(unsigned char byte)
{
		uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		QMC_W_SDA(byte & (0x80 >> i));
		QMC_W_SCL(1);
		QMC_W_SCL(0);
	}
 
}
unsigned char  QMC_IIC_Receive_Byte(void)
{
		uint8_t i, Byte = 0x00;
	QMC_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		QMC_W_SCL(1);
		if (QMC_R_SDA() == 1){Byte |= (0x80 >> i);}
		QMC_W_SCL(0);
	}
	return Byte;
}
void QMC_IIC_Send_ACK(unsigned char ACK)
{
	QMC_W_SDA(ACK);
	QMC_W_SCL(1);
	QMC_W_SCL(0);
}
 
uint8_t QMC_IIC_ReceiveAck(void)
{
	uint8_t AckBit;
	QMC_W_SDA(1);
	QMC_W_SCL(1);
	AckBit = QMC_R_SDA();
	QMC_W_SCL(0);
	return AckBit;
}

