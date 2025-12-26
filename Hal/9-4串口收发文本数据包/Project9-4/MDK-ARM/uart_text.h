#ifndef UART_TEXT_H
#define UART_TEXT_H

#include "stm32f1xx_hal.h"  // 根据实际芯片型号修改

// 文本帧结束符（使用'\n'作为结束标记）
#define TEXT_FRAME_END '\n'
// 最大文本长度（根据需要调整）
#define UART_MAX_TEXT_LEN 64

// 串口文本管理结构体
typedef struct {
    uint8_t state;          // 接收状态
    uint16_t dataLength;    // 当前接收长度
} UartTextManager_t;

// 函数声明
void UART_InitTextManager(UartTextManager_t *manager);
HAL_StatusTypeDef UART_SendTextPacket(UART_HandleTypeDef *huart, const char *text);
uint8_t UART_ProcessTextByte(UartTextManager_t *manager, uint8_t byte, char *buffer);

#endif
