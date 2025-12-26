#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"                  // Device header
#include "QMI8658C.h"                  // Device header
#include "Delay.h"                  // Device header
#include "OLED.h"                  // Device header

u8 QMI8658_ReadReg(u8 dAddr, u8 Reg)
{
    u8 Value = 0;
    //1.??IIC
    MyI2C_Start();
    //2.?????
    MyI2C_SendByte((dAddr << 1)|0);
    //3.wait
    if(MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return 1;
    }
    //4.?????
    MyI2C_SendByte(Reg);
    //5.wait
    if(MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return 1;
    }
    //6.??IIC
    MyI2C_Start();
    //7.????
    MyI2C_SendByte((dAddr << 1)|1);
    //8.wait read ack
    if(MyI2C_ReceiveAck())
    {
        MyI2C_Stop();
        return 1;
    }
    
    Value = MyI2C_ReceiveByte(1);
    //9.stop
    MyI2C_Stop();
    return Value;
}

void QMI8658_WriteReg(u8 dAddr, u8 Reg, u8 Value)
{
    //1.??IIC
    MyI2C_Start();
    //2.?????
    MyI2C_SendByte((dAddr << 1)|0);
    //3.wait
    while(MyI2C_ReceiveAck());
    //4.??????
    MyI2C_SendByte(Reg);
    //5.wait
    MyI2C_ReceiveAck();
    //6.?value
    MyI2C_SendByte(Value);
    //7.wait
    MyI2C_ReceiveAck();
    //8.stop
    MyI2C_Stop();
}


void QMI8658_Init()
{
    u8 uChipID = 0;
    u16 AX=0,AY=0,AZ=0,GX=0,GY=0,GZ=0;
    // 1.Device Reset & check Device
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL9,0xA2); //PowerReset
    Delay_ms(2000);                           // wait stable
    uChipID = QMI8658_ReadReg(AMI8658C_I2C_Add,0x00); // Read ID
    while(uChipID != 0x5) Delay_ms(2000);        // device check
    
    //2.Device Settings
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL1,0x40); //????,??
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL2,0x33); //?????? ??+-16g,???ODR==1kHZ
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL3,0x73); //?????  ??±2048 dps ,????940HZ
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL5,0x0); //????????
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL6,0x0); //???Motion on Demand
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL7,0x03); // ??????????  
    
    
//    for(;;)
	{
        AX = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_H);
        AX = AX<<8;
        AX = AX | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_L);
        
        AY = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_H);
        AY = AY<<8;
        AY = AY | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_L);
        
        AZ = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_H);
        AZ = AZ<<8;
        AZ = AZ | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_L);
        
        GX = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_H);
        GX = GX<<8;
        GX = GX | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_L);
        
        GY = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_H);
        GY = GY<<8;
        GY = GY | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_L);
        
        GZ = (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_H);
        GZ = GZ<<8;
        GZ = GZ | (u16)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_L);

	}
}
