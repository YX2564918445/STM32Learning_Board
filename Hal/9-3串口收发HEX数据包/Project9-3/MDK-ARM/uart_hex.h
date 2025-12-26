#ifndef UART_HEX_H
#define UART_HEX_H

#include "stm32f1xx_hal.h" 

// 帧格式定义
#define FRAME_HEADER       0xAA  // 帧头
#define FRAME_TAIL         0x55  // 帧尾
#define MAX_PACKET_LENGTH  64    // 最大数据包长度，可根据需要修改

// 数据包结构体
typedef struct {
    uint8_t data[MAX_PACKET_LENGTH];  // 数据缓冲区
    uint16_t length;                  // 数据长度
} HexPacket_t;

// 接收状态枚举
typedef enum {
    WAIT_HEADER,    // 等待帧头
    WAIT_LENGTH,    // 等待长度
    WAIT_DATA,      // 等待数据
    WAIT_CHECKSUM,  // 等待校验和
    WAIT_TAIL       // 等待帧尾
} ReceiveState_t;

// 接收管理器结构体
typedef struct {
    ReceiveState_t state;             // 当前接收状态
    HexPacket_t currentPacket;        // 当前接收的数据包
    uint16_t dataIndex;               // 数据索引
    uint8_t checksum;                 // 校验和
} UartReceiveManager_t;

// 函数声明
void UART_InitReceiveManager(UartReceiveManager_t *manager);
uint8_t UART_CalculateChecksum(uint8_t *data, uint16_t length);
HAL_StatusTypeDef UART_SendHexPacket(UART_HandleTypeDef *huart, HexPacket_t *packet);
uint8_t UART_ProcessReceivedByte(UartReceiveManager_t *manager, uint8_t byte, HexPacket_t *receivedPacket);

#endif // UART_HEX_H
    