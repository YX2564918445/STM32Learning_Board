#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_10,(BitAction)BitValue);
	Delay_us (10);
}
void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit( GPIOB, GPIO_Pin_11,(BitAction)BitValue);
	Delay_us (10);
}
uint8_t MyI2C_R_SDA()
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit( GPIOB,  GPIO_Pin_11);
	Delay_us (10);
	return BitValue;
}
void MyI2C_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOC的
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD ;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;				//GPIO引脚，赋值为第13号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);	
}
void MyI2C_Start()
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte&(0x80>>i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}
void MyI2C_SendAck(uint8_t Ack )
{
		MyI2C_W_SDA(Ack);
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
}
uint8_t  MyI2C_ReceiveByte()
{
	uint8_t Byte=0x00,i;
	MyI2C_W_SDA(1);
	for(i=0;i<8;i++)
	{
		
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1){Byte|=(0x80>>i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}
uint8_t MyI2C_ReceiveAck()
{
	uint8_t Ack;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	Ack= MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	

	return Ack;
	
}
void MyI2C_Stop()
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	
	MyI2C_W_SDA(1);
}




