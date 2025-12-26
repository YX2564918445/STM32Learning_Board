#include "QMC5883.h"
#include "math.h"

/**
 * @brief  向QMC5883L指定寄存器写入数据
 * @param  reg_addr: 寄存器地址
 * @param  data: 要写入的数据
 */
void QMC_WriteReg(uint8_t reg_addr, uint8_t data)
{
    // 使用HAL库I2C写寄存器函数
    // 参数：I2C句柄、设备地址（7位）、寄存器地址、地址长度（8位）、数据指针、数据长度、超时时间
    HAL_I2C_Mem_Write(&hi2c2, QMC5883L_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

/**
 * @brief  从QMC5883L指定寄存器读取数据
 * @param  reg_addr: 寄存器地址
 * @retval 读取到的数据
 */
uint8_t QMC_ReadReg(uint8_t reg_addr)
{
    uint8_t data;
    // 使用HAL库I2C读寄存器函数
    HAL_I2C_Mem_Read(&hi2c2, QMC5883L_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

/**
 * @brief  获取QMC5883L的ID（用于验证通信是否正常）
 * @retval ID寄存器的值（默认应为0xFF，具体参考数据手册）
 */
uint8_t QMC_GetID(void)
{
    return QMC_ReadReg(QMC_REG_ID);  // 读取WHO_AM_I寄存器
}

/**
 * @brief  初始化QMC5883L传感器
 */
void qmc_init(void)
{
    // 原标准库初始化逻辑移植，保持寄存器配置一致
    QMC_WriteReg(QMC_REG_CONFIG3, 0x01);  // 配置寄存器3
    QMC_WriteReg(QMC_REG_CONFIG1, 0x09);  // 配置寄存器1（连续测量模式等）
    QMC_WriteReg(0x20, 0x40);             // 扩展配置寄存器（根据数据手册确认功能）
    QMC_WriteReg(0x21, 0x01);             // 扩展配置寄存器
    HAL_Delay(100);  // 等待初始化完成
}

/**
 * @brief  读取QMC5883L的三轴数据（X、Y、Z）
 * @param  AccX: X轴数据指针
 * @param  AccY: Y轴数据指针
 * @param  AccZ: Z轴数据指针
 */
void QMC_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ)
{
    uint8_t data[6];  // 存储6个连续寄存器的数据（X_H、X_L、Y_H、Y_L、Z_H、Z_L）
    
    // 一次性读取6个连续寄存器（从0x00开始），效率更高
    HAL_I2C_Mem_Read(&hi2c2, QMC5883L_ADDR, QMC_REG_X_MSB, I2C_MEMADD_SIZE_8BIT, data, 6, 100);
    
    // 拼接高低位数据（注意原标准库的拼接逻辑：低8位左移8位 | 高8位）
    *AccX = (data[1] << 8) | data[0];  // X轴：data[0]是MSB，data[1]是LSB
    *AccY = (data[3] << 8) | data[2];  // Y轴：data[2]是MSB，data[3]是LSB
    *AccZ = (data[5] << 8) | data[4];  // Z轴：data[4]是MSB，data[5]是LSB
}