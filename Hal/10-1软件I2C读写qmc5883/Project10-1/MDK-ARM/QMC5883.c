#include "QMC5883.h"
#include "math.h"
#include "I2C.h"


uint16_t  QMC_X,QMC_Y,QMC_Z;
float QMC_Angel;

uint8_t QMC5883_Read(uint8_t Slave_Address, uint8_t Register_Address) 
{
   uint8_t Data = 0;
   I2C_Start(); 
   I2C_SendByte(Slave_Address);	
   I2C_ReceiveAck(); 
   I2C_SendByte(Register_Address);
   I2C_ReceiveAck();
   I2C_Start(); 
   I2C_SendByte(Slave_Address + 1);
   I2C_ReceiveAck(); 
   Data = I2C_ReceiveByte();
   I2C_SendAck(0);
   I2C_Stop();
		
   return Data;		
}

uint8_t QMC5883_Write(uint8_t Slave_Address, uint8_t Register_Address, uint8_t Register_Data)
{
    I2C_Start();      //GY_IIC_Start(); 
	I2C_SendByte(Slave_Address);//GY_IIC_Send_Byte(Slave_Address);//发送设备地址
    I2C_ReceiveAck();
    I2C_SendByte(Register_Address); //写寄存器地址
    I2C_ReceiveAck();          //等待应答 
	I2C_SendByte(Register_Data);    //发送数据
    I2C_ReceiveAck();  
    I2C_Stop();	 
 
    return 0;   
}

uint8_t SF=0;  //Self Test Flag
void QMC5883_Init(void)
{
	I2C_Init();
	QMC5883_Write(QMC5883L_ADDRESS,QMC5883L_CONFIG_3,0x01);
	QMC5883_Write(QMC5883L_ADDRESS,QMC5883L_CONFIG_1,0x09);//512采样速率 2G范围 100HZ输出 连续转换模式
 
	
}

void QMC5883_Get_Val(void)
{
	
    QMC_X=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_X_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_X_LSB);
	QMC_Y=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Y_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Y_LSB);
	QMC_Z=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Z_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Z_LSB);
	//printf("%d\r\n",QMC_X);
	QMC_Angel=atan2((double)QMC_Y,(double)QMC_X)*57.3;
}