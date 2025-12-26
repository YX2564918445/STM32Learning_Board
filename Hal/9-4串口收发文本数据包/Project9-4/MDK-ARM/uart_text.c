#include "uart_text.h"
#include <string.h>

/**
 * @brief 初始化串口文本管理器
 * @param manager: 文本管理结构体指针
 */
void UART_InitTextManager(UartTextManager_t *manager) {
    if (manager != NULL) {
        manager->state = 0;
        manager->dataLength = 0;
    }
}

/**
 * @brief 发送文本数据包（带结束符）
 * @param huart: UART句柄
 * @param text: 要发送的文本
 * @return HAL状态
 */
HAL_StatusTypeDef UART_SendTextPacket(UART_HandleTypeDef *huart, const char *text) {
    if (huart == NULL || text == NULL) {
        return HAL_ERROR;
    }
    
    uint16_t textLen = strlen(text);
    if (textLen == 0 || textLen >= UART_MAX_TEXT_LEN) {
        return HAL_ERROR;
    }
    
    // 发送原始文本
    HAL_StatusTypeDef status = HAL_UART_Transmit(huart, (uint8_t*)text, textLen, 100);
    if (status != HAL_OK) {
        return status;
    }
    
    // 单独发送结束符，确保接收方能正确识别
    uint8_t endChar = TEXT_FRAME_END;
    return HAL_UART_Transmit(huart, &endChar, 1, 100);
}

/**
 * @brief 处理接收到的字节
 * @param manager: 文本管理结构体指针
 * @param byte: 接收到的字节
 * @param buffer: 存储接收文本的缓冲区
 * @return 1-接收完成，0-未完成
 */
uint8_t UART_ProcessTextByte(UartTextManager_t *manager, uint8_t byte, char *buffer) {
    if (manager == NULL || buffer == NULL) {
        return 0;
    }
    
    // 过滤回车符（处理Windows风格换行）
    if (byte == '\r') {
        return 0;
    }
    
    // 检查缓冲区是否溢出
    if (manager->dataLength >= UART_MAX_TEXT_LEN - 1) {
        manager->dataLength = 0;  // 溢出重置
        return 0;
    }
    
    // 检查是否是结束符
    if (byte == TEXT_FRAME_END) {
        buffer[manager->dataLength] = '\0';  // 添加字符串结束符
        uint16_t recvLen = manager->dataLength;
        manager->dataLength = 0;  // 重置接收长度
        return (recvLen > 0) ? 1 : 0;  // 确保接收到有效数据
    } else {
        // 正常字符，添加到缓冲区
        buffer[manager->dataLength++] = (char)byte;
        return 0;
    }
}
