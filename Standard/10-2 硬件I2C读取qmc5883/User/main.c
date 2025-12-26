#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "HMC5883L.h"
#include "math.h"
#include "delay.h"
#include "sys.h"
#include "OLED.h"
#include "MyI2C.h"
#include "stdio.h"
int cnt;
float angle;
//int16_t offsetX, offsetY, offsetZ;        //矫正补偿
uint8_t ge=0,shi=0,bai=0,qian=0,wan=0;          //显示变量        
uint32_t  a;
	double Angle_XY;	
	double Angle_XZ;	
	double Angle_YZ;	
uint8_t BUF[8]; 
double x,y,z,h;

void Conversion(u16 temp_data)  
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      
    ge=temp_data+0x30; 	
}
int main(void)
{	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	int16_t X=0,Y=0,Z=0;
	Angle_XY=0;
	Angle_XZ=0;
	Angle_YZ=0;	

	MyI2C_Init();

	qmc_init();
	OLED_Init();			  
	
	while(1)
	{
		if(a==6)
		{printf("6666");

		}
		QMC_GetData(&X,&Y, &Z);
		if(X>0x7fff)X-=0xffff;	  
		if(Y>0x7fff)Y-=0xffff;
		if(Z>0x7fff)Z-=0xffff;	 

		Angle_XY= atan2((double)Y,(double)X) * (180 / 3.14159265) + 180; //计算XY平面角度
		Angle_XY*=10;
				
		OLED_ShowNum (2,1,X,5);
				OLED_ShowNum (3,1,Y,5);

				OLED_ShowNum (4,1,Z,5);


		Conversion(Angle_XY);       //计算XY平面角度数据和显示
		OLED_ShowHexNum (1,1,qian,1);
		OLED_ShowHexNum (1,2,bai,1);
		OLED_ShowHexNum (1,3,shi,1);
		OLED_ShowHexNum (1,5,ge,1);
		OLED_ShowString (1,4,".");
 
		
	}
}
