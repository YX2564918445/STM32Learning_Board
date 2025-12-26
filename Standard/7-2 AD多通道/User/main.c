#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t AD7, AD8, AD9;	//定义AD值变量

int main(void)
{
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	AD_Init();					//AD初始化
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "AD7:");
	OLED_ShowString(2, 1, "AD8:");
	OLED_ShowString(3, 1, "AD9:");
	
	while (1)
	{
		AD7 = AD_GetValue(ADC_Channel_7);		//单次启动ADC，转换通道0
		AD8 = AD_GetValue(ADC_Channel_8);		//单次启动ADC，转换通道1
		AD9 = AD_GetValue(ADC_Channel_9);		//单次启动ADC，转换通道2
		
		OLED_ShowNum(1, 5, AD7, 4);				//显示通道0的转换结果AD0
		OLED_ShowNum(2, 5, AD8, 4);				//显示通道1的转换结果AD1
		OLED_ShowNum(3, 5, AD9, 4);				//显示通道2的转换结果AD2
		
		Delay_ms(100);			//延时100ms，手动增加一些转换的间隔时间
	}
}
