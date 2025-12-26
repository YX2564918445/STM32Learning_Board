#include "QMC5883.h"
#include "math.h"
#include "delay.h"
 
uint16_t  QMC_X,QMC_Y,QMC_Z;
float QMC_Angel;
 
u8 QMC5883_Read(u8 Slave_Address, u8 Register_Address) 
{
   u8 Data = 0;
  QMC_IIC_Start(); 
	QMC_IIC_Send_Byte(Slave_Address);	
	QMC_IIC_ReceiveAck(); 
  QMC_IIC_Send_Byte(Register_Address);
  QMC_IIC_ReceiveAck();
  QMC_IIC_Start();
	QMC_IIC_Send_Byte(Slave_Address + 1);
  QMC_IIC_ReceiveAck(); 
	Data=QMC_IIC_Receive_Byte();
	QMC_IIC_Send_ACK(0);
  QMC_IIC_Stop();
		
	return Data;		
}
 
u8 QMC5883_Write(u8 Slave_Address, u8 Register_Address, u8 Register_Data)
{
   QMC_IIC_Start();//GY_IIC_Start(); 
	 QMC_IIC_Send_Byte(Slave_Address);//GY_IIC_Send_Byte(Slave_Address);//发送设备地址
   QMC_IIC_ReceiveAck();
   QMC_IIC_Send_Byte(Register_Address); //写寄存器地址
   QMC_IIC_ReceiveAck();          //等待应答 
	 QMC_IIC_Send_Byte(Register_Data);    //发送数据
   QMC_IIC_ReceiveAck();  
   QMC_IIC_Stop();	 
 
   return 0;   
}
 
uint8_t SF=0;  //Self Test Flag
void QMC5883_Init(void)
{
	QMC_IIC_Init();
	
	QMC5883_Write(QMC5883L_ADDRESS,QMC5883L_CONFIG_3,0x01);
	QMC5883_Write(QMC5883L_ADDRESS,QMC5883L_CONFIG_1,0x09);//512采样速率 2G范围 100HZ输出 连续转换模式
 
	
}
 
//void QMC5883_Calibration()
//{
//	
//}
// 
 
void QMC5883_Get_Val(void)
{
	
    QMC_X=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_X_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_X_LSB);
		QMC_Y=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Y_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Y_LSB);
		QMC_Z=QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Z_MSB)<<8|QMC5883_Read(QMC5883L_ADDRESS,QMC5883L_DATA_READ_Z_LSB);
	  //printf("%d\r\n",QMC_X);
		QMC_Angel=atan2((double)QMC_Y,(double)QMC_X)*57.3;
}
 
 
 
 