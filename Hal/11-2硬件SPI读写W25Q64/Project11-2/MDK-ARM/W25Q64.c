#include "spi.h"
#include <stdint.h>
#include "W25Q64.h"
#include "gpio.h"
#include "W25Q64_Ins.h"

// CS引脚控制
#define W25Q64_CS_LOW()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)
#define W25Q64_CS_HIGH() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)

/**
  * 函    数：W25Q64初始化
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_Init(void)
{
	W25Q64_CS_HIGH();  // 默认不选中
}

/**
  * 函    数：MPU6050读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	uint8_t cmd = W25Q64_JEDEC_ID;
	uint8_t response[3];
	
	// 选中芯片
	W25Q64_CS_LOW();
	// 发送命令
	HAL_SPI_Transmit(&hspi2,&cmd,1,1000);
	// 接收ID数据
	HAL_SPI_Receive(&hspi2,response,3,1000);
	// 取消选中
	W25Q64_CS_HIGH();
	// 解析数据
	*MID = response[0];
	*DID = (response[1] << 8) | response[2];
}

/**
  * 函    数：W25Q64写使能
  * 参    数：无
  * 返 回 值：无
  */
// 正确写法：用临时变量存储命令
void W25Q64_WriteEnable(void)
{
    uint8_t cmd = W25Q64_WRITE_ENABLE; // 将宏值存入变量
    W25Q64_CS_LOW();
    // 传递变量地址（变量有合法内存地址）
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 1000);
    W25Q64_CS_HIGH();
}

/**
  * 函    数：W25Q64等待忙
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WaitBusy(void)
{
    uint32_t Timeout = 100000;
    uint8_t status;
    uint8_t cmd = W25Q64_READ_STATUS_REGISTER_1; // 临时变量存储命令
    
    W25Q64_CS_LOW();
    // 发送命令（使用变量地址）
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 1000);
    
    do {
        HAL_SPI_Receive(&hspi2, &status, 1, 1000);
        Timeout--;
        if (Timeout == 0) break;
    } while ((status & 0x01) == 0x01);
    
    W25Q64_CS_HIGH();
}

/**
  * 函    数：W25Q64页编程
  * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray	用于写入数据的数组
  * 参    数：Count 要写入数据的数量，范围：0~256
  * 返 回 值：无
  * 注意事项：写入的地址范围不能跨页
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint8_t cmd[4] = {W25Q64_PAGE_PROGRAM,(uint8_t)((Address >> 16)&0xFF),
		     (uint8_t)((Address >> 8)&0xFF),(uint8_t)(Address&0xFF)};
	
	W25Q64_WriteEnable();						//写使能
    W25Q64_CS_LOW();								//SPI起始
	HAL_SPI_Transmit(&hspi2,cmd,4,1000);		//交换发送页编程的指令和写入地址
	HAL_SPI_Transmit(&hspi2,DataArray,Count,1000);
	W25Q64_CS_HIGH();								//SPI终止
	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64扇区擦除（4KB）
  * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * 返 回 值：无
  */
void W25Q64_SectorErase(uint32_t Address)
{
	uint8_t cmd[4] = {W25Q64_SECTOR_ERASE_4KB,(uint8_t)((Address >> 16)&0xFF),
		     (uint8_t)((Address >> 8)&0xFF),(uint8_t)(Address&0xFF)};
	W25Q64_WriteEnable();						//写使能
	W25Q64_CS_LOW();								//SPI起始
	HAL_SPI_Transmit(&hspi2,cmd,4,1000);	//交换发送扇区擦除的指令、地址
	W25Q64_CS_HIGH();								//SPI终止
	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64读取数据
  * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * 参    数：Count 要读取数据的数量，范围：0~0x800000
  * 返 回 值：无
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint8_t cmd[4] = {W25Q64_READ_DATA,(uint8_t)((Address >> 16)&0xFF),
		     (uint8_t)((Address >> 8)&0xFF),(uint8_t)(Address&0xFF)};
	W25Q64_CS_LOW();								//SPI起始
	HAL_SPI_Transmit(&hspi2,cmd,4,1000);			//交换发送读取数据的指令、地址
    HAL_SPI_Receive(&hspi2,DataArray,Count,1000);
	W25Q64_CS_HIGH();								//SPI终止
}



