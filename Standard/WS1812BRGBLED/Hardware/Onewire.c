#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Onewire_SetPB1()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 1);
}
void Onewire_ResetPB1()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 0);
}


uint8_t OneWire_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOC的
	GPIO_InitTypeDef GPIO_InitStructure;					//定义结构体变量
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP ;		//GPIO模式，赋值为推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//GPIO引脚，赋值为第13号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO速度，赋值为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//将赋值后的构体变量传递给GPIO_Init函
	

	return 1;
}
void Send_A_bit(unsigned char VAL)
{
    if (VAL!=1)
    {
GPIOA->BSRR = GPIO_Pin_6;//必须寄存器，单总线在微妙级，误差在纳秒级
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
GPIOA->BRR = GPIO_Pin_6;
        
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
    else
    {
GPIOA->BSRR = GPIO_Pin_6;
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
GPIOA->BRR = GPIO_Pin_6;
        
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
}

void Reset_LED()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 1);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 0);
	Delay_ms(1);

}
