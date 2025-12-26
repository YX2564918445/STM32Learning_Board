#include "stm32f10x.h"                  // Device header
#include "QMI8658C.h"                  // Device header
#include "Delay.h"                  // Device header

void QMC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//¸ø¶¨³¬Ê±¼ÆÊýÊ±¼ä
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//Ñ­»·µÈ´ýÖ¸¶¨ÊÂ¼þ
	{
		Timeout --;										//µÈ´ýÊ±£¬¼ÆÊýÖµ×Ô¼õ
		if (Timeout == 0)								//×Ô¼õµ½0ºó£¬µÈ´ý³¬Ê±
		{
			/*³¬Ê±µÄ´íÎó´¦Àí´úÂë£¬¿ÉÒÔÌí¼Óµ½´Ë´¦*/
			break;										//Ìø³öµÈ´ý£¬²»µÈÁË
		}
	}
}
void QMI8658_WriteReg(uint8_t DeviceAddr, uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);										//Ó²¼þI2CÉú³ÉÆðÊ¼Ìõ¼þ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//µÈ´ýEV5
	
	I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);	//Ó²¼þI2C·¢ËÍ´Ó»úµØÖ·£¬·½ÏòÎª·¢ËÍ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//µÈ´ýEV6
	
	I2C_SendData(I2C2, RegAddress);											//Ó²¼þI2C·¢ËÍ¼Ä´æÆ÷µØÖ·
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//µÈ´ýEV8
	
	I2C_SendData(I2C2, Data);												//Ó²¼þI2C·¢ËÍÊý¾Ý
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//µÈ´ýEV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											//Ó²¼þI2CÉú³ÉÖÕÖ¹Ìõ¼þ
}
u8 QMI8658_ReadReg(uint8_t DeviceAddr, uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);										//Ó²¼þI2CÉú³ÉÆðÊ¼Ìõ¼þ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//µÈ´ýEV5
	
	I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);	//Ó²¼þI2C·¢ËÍ´Ó»úµØÖ·£¬·½ÏòÎª·¢ËÍ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//µÈ´ýEV6
	
	I2C_SendData(I2C2, RegAddress);											//Ó²¼þI2C·¢ËÍ¼Ä´æÆ÷µØÖ·
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//µÈ´ýEV8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);										//Ó²¼þI2CÉú³ÉÖØ¸´ÆðÊ¼Ìõ¼þ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//µÈ´ýEV5
	
	I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Receiver);		//Ó²¼þI2C·¢ËÍ´Ó»úµØÖ·£¬·½ÏòÎª½ÓÊÕ
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//µÈ´ýEV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);									//ÔÚ½ÓÊÕ×îºóÒ»¸ö×Ö½ÚÖ®Ç°ÌáÇ°½«Ó¦´ðÊ§ÄÜ
	I2C_GenerateSTOP(I2C2, ENABLE);											//ÔÚ½ÓÊÕ×îºóÒ»¸ö×Ö½ÚÖ®Ç°ÌáÇ°ÉêÇëÍ£Ö¹Ìõ¼þ
	
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//µÈ´ýEV7
	Data = I2C_ReceiveData(I2C2);											//½ÓÊÕÊý¾Ý¼Ä´æÆ÷
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);									//½«Ó¦´ð»Ö¸´ÎªÊ¹ÄÜ£¬ÎªÁË²»Ó°ÏìºóÐø¿ÉÄÜ²úÉúµÄ¶ÁÈ¡¶à×Ö½Ú²Ù×÷
	
	return Data;
}
void QMI8658_Init(void)
{
	/*????*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//??GPIOB???
//	
//	/*GPIO???*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					//?PB10?PB11??????????
//	
//	/*??????*/
//	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);			//??PB10?PB11????????????(??????)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//??I2C2???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//??GPIOB???
	
	/*GPIO???*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//?PB10?PB11????????????
	
	/*I2C???*/
	I2C_InitTypeDef I2C_InitStructure;						//???????
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//??,???I2C??
	I2C_InitStructure.I2C_ClockSpeed = 50000;				//????,???50KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//?????,??Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//??,????
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//????,??7?,????????
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//????,????????
	I2C_Init(I2C2, &I2C_InitStructure);						//????????I2C_Init,??I2C2
	
	/*I2C??*/
	I2C_Cmd(I2C2, ENABLE);									//??I2C2,????
	
	    u8 uChipID = 0;
    u16 AX=0,AY=0,AZ=0,GX=0,GY=0,GZ=0;
    // 1.Device Reset & check Device
    QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL9,0xA2); //PowerReset
    uChipID = QMI8658_ReadReg(AMI8658C_I2C_Add,0x00); // Read ID
    while(uChipID != 0x5) ;        // device check
	QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL1,0x40); //????,??
  QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL2,0x33); //?????? ??+-16g,???ODR==1kHZ
  QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL3,0x73); //?????  ??±2048 dps ,????940HZ
  QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL5,0x0); //????????
  QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL6,0x0); //???Motion on Demand
  QMI8658_WriteReg(AMI8658C_I2C_Add,QMI8658C_RegAdd_CTRL7,0x03); // ??????????  
}