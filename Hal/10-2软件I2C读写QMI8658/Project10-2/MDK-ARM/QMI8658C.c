#include "I2C.h"                  // Device header
#include "QMI8658C.h"              // Device header
#include "OLED.h"                  // Device header
#include "gpio.h"

uint8_t QMI8658_ReadReg(uint8_t dAddr, uint8_t Reg)
{
    uint8_t Value = 0;
    I2C_Start();
    I2C_SendByte((dAddr << 1)|0);
    if(I2C_ReceiveAck())
    {
        I2C_Stop();
        return 1;
    }
    I2C_SendByte(Reg);
    if(I2C_ReceiveAck())
    {
        I2C_Stop();
        return 1;
    }

    I2C_Start();
    I2C_SendByte((dAddr << 1)|1);
    //wait read ack
    if(I2C_ReceiveAck())
    {
        I2C_Stop();
        return 1;
    }
    
    Value = I2C_ReceiveByte();
    //stop
    I2C_Stop();
    return Value;
}

void QMI8658_WriteReg(uint8_t dAddr,uint8_t Reg,uint8_t Value)
{
    I2C_Start();
    I2C_SendByte((dAddr << 1)|0);
    while(I2C_ReceiveAck());
    I2C_SendByte(Reg);
    I2C_ReceiveAck();
    I2C_SendByte(Value);
	I2C_ReceiveAck();
    I2C_Stop();
}


void QMI8658_Init()
{
    uint8_t uChipID = 0;
    uint8_t AX=0,AY=0,AZ=0,GX=0,GY=0,GZ=0;
    // 1.Device Reset & check Device
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL9,0xA2); //PowerReset
    HAL_Delay(2000);                           // wait stable
    uChipID = QMI8658_ReadReg(AMI8658C_I2C_Add,0x00); // Read ID
    while(uChipID != 0x5) HAL_Delay(2000);        // device check
    
    //2.Device Settings
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL1,0x40); 
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL2,0x33); 
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL3,0x73); 
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL5,0x0); 
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL6,0x0); 
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL7,0x03);  
    
    
//    for(;;)
	{
        AX = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_H);
        AX = AX<<8;
        AX = AX | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_L);
        
        AY = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_H);
        AY = AY<<8;
        AY = AY | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_L);
        
        AZ = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_H);
        AZ = AZ<<8;
        AZ = AZ | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_L);
        
        GX = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_H);
        GX = GX<<8;
        GX = GX | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AX_L);
        
        GY = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_H);
        GY = GY<<8;
        GY = GY | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AY_L);
        
        GZ = (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_H);
        GZ = GZ<<8;
        GZ = GZ | (uint16_t)QMI8658_ReadReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_AZ_L);

	}
}
