#include "uart_hex.h"
#include <string.h>

/**
 * @brief 初始化UART接收管理器
 * @param manager 接收管理器指针
 */
void UART_InitReceiveManager(UartReceiveManager_t *manager) {
    if (manager == NULL) return;
    
    manager->state = WAIT_HEADER;
    manager->currentPacket.length = 0;
    manager->dataIndex = 0;
    manager->checksum = 0;
    memset(manager->currentPacket.data, 0, MAX_PACKET_LENGTH);
}

/**
 * @brief 计算校验和（异或校验）
 * @param data 数据指针
 * @param length 数据长度
 * @return 计算得到的校验和
 */
uint8_t UART_CalculateChecksum(uint8_t *data, uint16_t length) {
    uint8_t checksum = 0;
    if (data == NULL || length == 0) return checksum;
    
    for (uint16_t i = 0; i < length; i++) {
        checksum ^= data[i];  // 使用异或校验
    }
    return checksum;
}

/**
 * @brief 发送HEX数据包
 * @param huart UART句柄（此处应传入USART1的句柄）
 * @param packet 要发送的数据包
 * @return HAL状态
 */
HAL_StatusTypeDef UART_SendHexPacket(UART_HandleTypeDef *huart, HexPacket_t *packet) {
    if (huart == NULL || packet == NULL || packet->length > MAX_PACKET_LENGTH) {
        return HAL_ERROR;
    }
    
    // 计算校验和
    uint8_t checksum = UART_CalculateChecksum(packet->data, packet->length);
    
    // 构建发送缓冲区：帧头 + 长度 + 数据 + 校验和 + 帧尾
    uint8_t txBuffer[MAX_PACKET_LENGTH + 4];
    txBuffer[0] = FRAME_HEADER;
    txBuffer[1] = packet->length;
    memcpy(&txBuffer[2], packet->data, packet->length);
    txBuffer[2 + packet->length] = checksum;
    txBuffer[3 + packet->length] = FRAME_TAIL;
    
    // 发送数据包
    return HAL_UART_Transmit(huart, txBuffer, 4 + packet->length, HAL_MAX_DELAY);
}

/**
 * @brief 处理接收到的字节（状态机实现）
 * @param manager 接收管理器
 * @param byte 接收到的字节
 * @param receivedPacket 接收到的完整数据包
 * @return 1-接收到完整数据包，0-未接收到完整数据包
 */
uint8_t UART_ProcessReceivedByte(UartReceiveManager_t *manager, uint8_t byte, HexPacket_t *receivedPacket) {
    if (manager == NULL || receivedPacket == NULL) return 0;
    
    switch (manager->state) {
        case WAIT_HEADER:
            // 等待帧头，收到帧头后进入下一状态
            if (byte == FRAME_HEADER) {
                UART_InitReceiveManager(manager);  // 重置接收管理器
                manager->state = WAIT_LENGTH;      // 进入等待长度状态
            }
            break;
            
        case WAIT_LENGTH:
            // 接收数据长度，限制最大长度防止溢出
            if (byte <= MAX_PACKET_LENGTH) {
                manager->currentPacket.length = byte;
                // 如果长度为0，直接进入等待校验和状态
                manager->state = (byte > 0) ? WAIT_DATA : WAIT_CHECKSUM;
            } else {
                // 长度超过最大值，重新等待帧头
                manager->state = WAIT_HEADER;
            }
            break;
            
        case WAIT_DATA:
            // 接收数据内容并计算校验和
            manager->currentPacket.data[manager->dataIndex++] = byte;
            manager->checksum ^= byte;
            
            // 数据接收完毕，进入等待校验和状态
            if (manager->dataIndex >= manager->currentPacket.length) {
                manager->state = WAIT_CHECKSUM;
            }
            break;
            
        case WAIT_CHECKSUM:
            // 验证校验和
            if (byte == manager->checksum) {
                manager->state = WAIT_TAIL;  // 校验通过，等待帧尾
            } else {
                manager->state = WAIT_HEADER; // 校验失败，重新等待帧头
            }
            break;
            
        case WAIT_TAIL:
            // 验证帧尾
            if (byte == FRAME_TAIL) {
                // 接收完整数据包，复制到输出
                memcpy(receivedPacket->data, manager->currentPacket.data, manager->currentPacket.length);
                receivedPacket->length = manager->currentPacket.length;
                
                // 重置状态机，准备接收下一帧
                manager->state = WAIT_HEADER;
                return 1;  // 返回1表示接收到完整数据包
            } else {
                // 帧尾错误，重新等待帧头
                manager->state = WAIT_HEADER;
            }
            break;
            
        default:
            manager->state = WAIT_HEADER;
            break;
    }
    
    return 0;  // 未接收到完整数据包
}
    