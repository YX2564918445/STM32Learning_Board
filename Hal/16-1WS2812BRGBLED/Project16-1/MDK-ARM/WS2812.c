#include "WS2812.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

#define Code0       28
#define Code1       57
#define CodeReset   0

// ÂÌºìÀ¶
void WS2812_Update()
{
	static uint16_t data[] = {
		Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
	    Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 ,
		Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 ,
		
		Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
	    Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
		Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 , Code0 ,
		
		Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
		Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
		Code0 , Code0 , Code0 , Code1 , Code1 , Code1 , Code1 , Code1 ,
		
		CodeReset
	};
	HAL_TIM_PWM_Start_DMA(&htim3,TIM_CHANNEL_1,(uint32_t*)data,sizeof(data)/sizeof(uint16_t));
}