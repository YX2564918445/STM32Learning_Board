#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
															//使用各个外设前必须开启时钟，否则对外设的操作无效
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//GPIO引脚，赋值为第8号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函数
															//函数内部会自动根据结构体的参数配置相应寄存器
															//实现GPIOA的初始化
	
	/*主循环，循环体内的代码会一直循环执行*/
	while (1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);		//将PA8引脚设置为低电平，蜂鸣器鸣叫
		Delay_ms(100);							//延时100ms
		GPIO_SetBits(GPIOA, GPIO_Pin_8);		//将PA8引脚设置为高电平，蜂鸣器停止
		Delay_ms(100);							//延时100ms
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);		//将PA8引脚设置为低电平，蜂鸣器鸣叫
		Delay_ms(100);							//延时100ms
		GPIO_SetBits(GPIOA, GPIO_Pin_8);		//将PA8引脚设置为高电平，蜂鸣器停止
		Delay_ms(700);							//延时700ms
	}
}
