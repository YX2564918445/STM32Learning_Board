#include "gpio.h"
#include <stdint.h>
#include "I2C.h"

// 设置SCL状态
void I2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,BitValue);
}
// 设置SDA状态
void I2C_W_SDA (uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,BitValue);
}
// 读取SDA状态
uint8_t I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11);
	return BitValue;
}
// 软件IIC初始化
void I2C_Init(void)
{
	MX_GPIO_Init();
}
// IIC开始
void I2C_Start(void)
{
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_W_SDA(0);
	I2C_W_SCL(0);
}
// IIC停止
void I2C_Stop(void)
{
	I2C_W_SDA(0);
	I2C_W_SCL(1);
	I2C_W_SDA(1);
}
// IIC发送一字节数据
void I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0;i < 8;i++)
	{
		I2C_W_SDA(!!(Byte & (0x80 >> i)));
		I2C_W_SCL(1);
		I2C_W_SCL(0);
	}
}
// IIC接收一字节数据
uint8_t I2C_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	I2C_W_SDA(1);
	for (i = 0;i < 8;i++)
	{
		I2C_W_SCL(1);
		if (I2C_R_SDA())
		{
			Byte |= (0x80 >> i);
		}
		I2C_W_SCL(0);
	}
	return Byte;
}
// IIC发送ACK
void I2C_SendAck(uint8_t AckBit)
{
	I2C_W_SDA(AckBit);
	I2C_W_SCL(1);
	I2C_W_SCL(0);
}
// IIC接收ACK
uint8_t I2C_ReceiveAck(void)
{
	uint8_t AckBit;
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	AckBit = I2C_R_SDA();
	I2C_W_SCL(0);
	return AckBit;
}