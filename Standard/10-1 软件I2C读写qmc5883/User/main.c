#include "stm32f10x.h"                  // Device heade
#include "Delay.h"
#include "OLED.h"
#include "QMC5883.h"

int main()
{	
	
	OLED_Init();
	
//			OLED_Reverse();

		OLED_Update();
QMC5883_Init();
 
	while(1)
	{
		QMC5883_Get_Val();
		OLED_ShowNum(0, 0, QMC_X, 5, OLED_8X16);
		OLED_ShowNum(0, 16, QMC_Y, 5, OLED_8X16);
		OLED_ShowNum(0, 32, QMC_Z, 5, OLED_8X16);		OLED_Update();


	}

}
