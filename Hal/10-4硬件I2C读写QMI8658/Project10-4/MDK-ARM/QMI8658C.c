#include "QMI8658C.h"              // Device header
#include "OLED.h"                  // Device header
#include "i2c.h"                   // HAL I2C头文件

// 使用I2C2句柄，需要与CubeMX配置一致
extern I2C_HandleTypeDef hi2c2;

uint8_t QMI8658_ReadReg(uint8_t dAddr, uint8_t Reg)
{
    uint8_t Value = 0;
    // 使用HAL库I2C读取函数，从指定寄存器读取1个字节
    if(HAL_I2C_Mem_Read(&hi2c2, (dAddr << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100) != HAL_OK)
    {
        // 读取失败可以添加错误处理
        return 0xFF; // 返回错误标志
    }
    return Value;
}

void QMI8658_WriteReg(uint8_t dAddr, uint8_t Reg, uint8_t Value)
{
    // 使用HAL库I2C写入函数，向指定寄存器写入1个字节
    HAL_I2C_Mem_Write(&hi2c2, (dAddr << 1), Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 100);
}

void QMI8658_Init()
{
    uint8_t uChipID = 0;
    int16_t AX=0, AY=0, AZ=0, GX=0, GY=0, GZ=0; // 修改为int16_t以正确处理有符号数据
    
    // 1. 设备复位与检查
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL9, 0xA2); // 电源复位
    HAL_Delay(200); // 等待复位完成，2秒太长，200ms足够
    
    // 读取芯片ID，QMI8658的ID应该是0x05
    uChipID = QMI8658_ReadReg(AMI8658C_I2C_Add, 0x00);
    
    // 等待设备就绪
    while(uChipID != 0x05)
    {
        HAL_Delay(100);
        uChipID = QMI8658_ReadReg(AMI8658C_I2C_Add, 0x00);
    }
    
    // 2. 设备配置
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL1, 0x40); // 加速度计配置
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL2, 0x33); // 陀螺仪配置
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL3, 0x73); // 传感器使能
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL5, 0x00); // 过滤配置
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL6, 0x00); // 陀螺仪范围
    QMI8658_WriteReg(AMI8658C_I2C_Add, QMI8658C_RegAdd_CTRL7, 0x03); // 同时使能加速度计和陀螺仪
    
    // 注意：原代码中此处有一个死循环读取数据，实际应用中应该在主循环中读取
}

// 新增一个函数用于读取所有传感器数据
void QMI8658_ReadData(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t data[12]; // 用于存储读取的12个字节数据
    
    // 一次性读取多个寄存器，提高效率
    if(HAL_I2C_Mem_Read(&hi2c2, (AMI8658C_I2C_Add << 1),QMI8658C_RegAdd_AX_H, 
                       I2C_MEMADD_SIZE_8BIT, data, 12, 100) == HAL_OK)
    {
        // 组合高低字节，注意数据是有符号的
        *ax = (int16_t)((data[0] << 8) | data[1]);
        *ay = (int16_t)((data[2] << 8) | data[3]);
        *az = (int16_t)((data[4] << 8) | data[5]);
        *gx = (int16_t)((data[6] << 8) | data[7]);
        *gy = (int16_t)((data[8] << 8) | data[9]);
        *gz = (int16_t)((data[10] << 8) | data[11]);
    }
}
