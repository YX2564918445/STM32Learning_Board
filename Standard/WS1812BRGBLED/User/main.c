#include "stm32f10x.h"                  // Device heade
#include "Delay.h"
#include "Onewire.h"
struct My_24bits                    //???
{
    unsigned char G_VAL;                 //??  0-255
    unsigned char R_VAL;                  //?? 0-255
    unsigned char B_VAL;                  //??0-255
};
 

struct My_24bits RGB;

/*??24???(??RGB???8?)*/
void Send_24bits(struct My_24bits RGB_VAL)
{
    unsigned char i;
    for(i=0; i<8; i++)
    {
        Send_A_bit(RGB_VAL.G_VAL>>(7-i)&0x01);//????????
    }
    for(i=8; i<16; i++)
    {
        Send_A_bit(RGB_VAL.R_VAL>>(15-i)&0x01);
    }
    for(i=16; i<24; i++)
    {
        Send_A_bit(RGB_VAL.B_VAL>>(23-i)&0x01);
    }

}
int main()
{	
	OneWire_Init();
//	        Reset_LED();
        Reset_LED();

	RGB.G_VAL = 10; RGB.R_VAL = 0;RGB.B_VAL = 0;
        Reset_LED();
             Reset_LED();

     Send_24bits(RGB);
		RGB.R_VAL = 10;
        Send_24bits(RGB);
		RGB.B_VAL = 10;
        Send_24bits(RGB);
	        Reset_LED();

	while(1)
	{
	  
	
		
		
	}

}
