/**
  ******************************************************************************
  * @file           : vision_potocol.c\h
  * @brief          : 
  * @update         : 
  ******************************************************************************
  */

#ifndef __VISION_PROTOCOL_H
#define __VISION_PROTOCOL_H

#include "config.h"

/**
 * @brief 视觉通信 接受信息结构体
 * 
 */
typedef struct __packed 
{
	uint8_t  SOF;
	uint8_t  datau8_1;//detect_mode
	uint8_t  CRC8;

	float    dataf_1;  //pitch
	float    dataf_2;  //yaw
	uint8_t  datau8_2; //is_find_target
	uint8_t  datau8_3;  //distance
	uint8_t  datau8_4;  //gyro_found
	uint8_t  datau8_5;//car_number
	uint16_t datau16_1;//UI
	uint16_t datau16_2;//UI

	uint16_t CRC16;
}vision_rx_info_t;

/**
 * @brief 视觉通信 发送信息结构体
 * 
 */
typedef struct __packed 
{
	uint8_t   SOF;
	uint8_t   datau8_1;//模式
	uint8_t   CRC8;

	float     datas_1;//yaw_angle
	float     datas_2;//pitch_angle
	uint8_t   datau8_3;//
	uint8_t   datau8_2;//己方颜色
	uint8_t   datau8_4;//切换数字

	uint16_t  CRC16;
}vision_tx_info_t;

extern vision_tx_info_t vision_tx_info;
extern vision_rx_info_t vision_rx_info;

Dev_Work_State_e Vision_DataTx(UART_HandleTypeDef *huart);
Dev_Work_State_e Vision_DataRx(uint8_t *rxBuf);

#endif
