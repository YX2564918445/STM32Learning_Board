#include <stdint.h>
#include "gpio.h"


/**
  * 函    数：FLASH读取一个32位的字
  * 参    数：Address 要读取数据的字地址
  * 返 回 值：指定地址下的数据
  */
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

/**
  * 函    数：FLASH读取一个16位的半字
  * 参    数：Address 要读取数据的半字地址
  * 返 回 值：指定地址下的数据
  */
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

/**
  * 函    数：FLASH读取一个8位的字节
  * 参    数：Address 要读取数据的字节地址
  * 返 回 值：指定地址下的数据
  */
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

/**
  * 函    数：FLASH全擦除
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，FLASH的所有页都会被擦除，包括程序文件本身，擦除后，程序将不复存在
  */
void MyFLASH_EraseAllPages(void)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    uint32_t PageError = 0;
    
    HAL_FLASH_Unlock();                 //解锁FLASH
    
    //配置擦除类型为全擦除
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_MASSERASE;
    FlashEraseInit.Banks = FLASH_BANK_1; //根据实际使用的FLASH银行配置
    
    //执行全擦除操作
    HAL_FLASHEx_Erase(&FlashEraseInit, &PageError);
    
    HAL_FLASH_Lock();                   //锁定FLASH
}

/**
  * 函    数：FLASH页擦除
  * 参    数：PageAddress 要擦除页的页地址
  * 返 回 值：无
  */
void MyFLASH_ErasePage(uint32_t PageAddress)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    uint32_t PageError = 0;
    
    HAL_FLASH_Unlock();                 //解锁FLASH
    
    //配置擦除参数
    FlashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    FlashEraseInit.PageAddress = PageAddress;
    FlashEraseInit.NbPages = 1;         //擦除页数：1页
    FlashEraseInit.Banks = FLASH_BANK_1; //根据实际使用的FLASH银行配置
    
    //执行页擦除操作
    HAL_FLASHEx_Erase(&FlashEraseInit, &PageError);
    
    HAL_FLASH_Lock();                   //锁定FLASH
}

/**
  * 函    数：FLASH编程字
  * 参    数：Address 要写入数据的字地址
  * 参    数：Data 要写入的32位数据
  * 返 回 值：无
  */
void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    HAL_FLASH_Unlock();                 //解锁FLASH
    
    //编程32位数据
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data);
    
    HAL_FLASH_Lock();                   //锁定FLASH
}

/**
  * 函    数：FLASH编程半字
  * 参    数：Address 要写入数据的半字地址
  * 参    数：Data 要写入的16位数据
  * 返 回 值：无
  */
void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    HAL_FLASH_Unlock();                 //解锁FLASH
    
    //编程16位数据
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, Data);
    
    HAL_FLASH_Lock();                   //锁定FLASH
}