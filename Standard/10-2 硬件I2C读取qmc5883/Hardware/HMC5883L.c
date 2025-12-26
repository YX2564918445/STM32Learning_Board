#include "HMC5883L.h"
#include "MyI2C.h"
#include "Delay.h"
#include "math.h"



void QMC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//给定超时计数时间
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//循环等待指定事件
	{
		Timeout --;										//等待时，计数值自减
		if (Timeout == 0)								//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;										//跳出等待，不等了
		}
	}
}
void QMC_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, 0x1a, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//等待EV8
	
	I2C_SendData(I2C2, Data);												//硬件I2C发送数据
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											//硬件I2C生成终止条件
}
uint8_t QMC_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, 0x1a, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成重复起始条件
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, 0x1a, I2C_Direction_Receiver);		//硬件I2C发送从机地址，方向为接收
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//等待EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);									//在接收最后一个字节之前提前将应答失能
	I2C_GenerateSTOP(I2C2, ENABLE);											//在接收最后一个字节之前提前申请停止条件
	
	QMC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//等待EV7
	Data = I2C_ReceiveData(I2C2);											//接收数据寄存器
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);									//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return Data;
}
uint8_t QMC_GetID(void)
{
	
	return QMC_ReadReg(0x0D);       //MPU6050_ReadReg(0x0D);		//返回WHO_AM_I寄存器的值
}
//void calibrateHMC5883L(int16_t* offsetX, int16_t* offsetY, int16_t* offsetZ) {     /////电磁矫正代码
//    const int samples = 100; // 读取样本数量
//    int16_t minX = INT16_MAX, minY = INT16_MAX, minZ = INT16_MAX;
//    int16_t maxX = INT16_MIN, maxY = INT16_MIN, maxZ = INT16_MIN;

//    // 进行多次读取
//    for (int i = 0; i < samples; i++) {
//       // int16_t x, y, z;
//        hmc_read_XYZ();
//        // 更新最小值和最大值
//        if (x < minX) minX = x;
//        if (y < minY) minY = y;
//        if (z < minZ) minZ = z;
//        if (x > maxX) maxX = x;
//        if (y > maxY) maxY = y;
//        if (z > maxZ) maxZ = z;

//        Delay_ms(50); // 延时以避免快速读取
//    }

//    // 计算偏移量
//    *offsetX = (minX + maxX) / 2;
//    *offsetY = (minY + maxY) / 2;
//    *offsetZ = (minZ + maxZ) / 2;

////    Serial.print("Calibration complete. Offsets: ");
////    Serial.print("X: "); Serial.print(*offsetX);
////    Serial.print(" Y: "); Serial.print(*offsetY);
////    Serial.print(" Z: "); Serial.println(*offsetZ);
//}

void qmc_init(void)
{

	QMC_WriteReg(0x09,0x0d);  //控制寄存器配置
	QMC_WriteReg(0x0b,0x01);  //设置清除时间寄存器
	QMC_WriteReg(0x20,0x40);  //
	QMC_WriteReg(0x21,0x01);  //	
	
}
void QMC_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = QMC_ReadReg(0x00);		//读取加速度计X轴的高8位数据
	DataL = QMC_ReadReg(0x01);		//读取加速度计X轴的低8位数据
	*AccX = (DataL << 8) | DataH;						//数据拼接，通过输出参数返回
	
	DataH = QMC_ReadReg(0x02);		//读取加速度计Y轴的高8位数据
	DataL = QMC_ReadReg(0x03);		//读取加速度计Y轴的低8位数据
	*AccY = (DataL << 8) | DataH;						//数据拼接，通过输出参数返回
	
	DataH = QMC_ReadReg(0x04);		//读取加速度计Z轴的高8位数据
	DataL = QMC_ReadReg(0x05);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataL << 8) | DataH;						//数据拼接，通过输出参数返回
	MyI2C_Stop();

}



