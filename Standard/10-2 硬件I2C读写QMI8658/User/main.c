#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "QMI8658C.h"
#include "MyI2C.h"                  // Device header

uint8_t ID;								//定义用于存放ID号的变量
    int16_t AX=0,AY=0,AZ=0,GX=0,GY=0,GZ=0;

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	QMI8658_Init();
	        OLED_ShowSignedNum(1,1, GZ,4);
		    OLED_ShowSignedNum(2,1, GX,4);
		    OLED_ShowSignedNum(3,1, GY,4);
				OLED_ShowSignedNum(1,7, AZ,4);
		    OLED_ShowSignedNum(2,7, AX,4);
		    OLED_ShowSignedNum(3,7, AY,4);
	while (1)
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
        
        OLED_ShowSignedNum(1,1, GZ,4);
		    OLED_ShowSignedNum(2,1, GX,4);
		    OLED_ShowSignedNum(3,1, GY,4);
				OLED_ShowSignedNum(1,7, AZ,4);
		    OLED_ShowSignedNum(2,7, AX,4);
		    OLED_ShowSignedNum(3,7, AY,4);

//		OLED_Clear();
	}
}
