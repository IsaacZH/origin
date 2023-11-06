/**
  ******************************************************************************
  * @file           : vision_potocol.c
  * @brief          : 
  * @update         : 
  ******************************************************************************
  */

#include "vision_protocol.h"
#include "stdbool.h"
#include "string.h"
#include "usart.h"
#include "crc.h"

vision_tx_info_t vision_tx_info = 
{
	.SOF = 0xA5,

};

vision_rx_info_t vision_rx_info;

uint8_t vision_txBuf[30];


/**
 * @brief 视觉通信收
 * 
 * @param rxBuf 
 * @return 收到数据：DEV_OK 没有到数据：DEV_NO
 * @note 
 */
Dev_Work_State_e Vision_DataRx(uint8_t *rxBuf)
{
	/* 帧首字节是否为0xA5 */
	if(rxBuf[0] == 0xA5)
	{
		/* 帧头CRC8校验*/
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			/* 帧尾CRC16校验 */
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(vision_rx_info_t)) == true)
			{
				memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
				return DEV_OK;
			}
		}
	}
	return DEV_NO;
}


/**
  * @Name    Vision_DataTx
  * @brief   视觉通信发(串口3)
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
uint32_t vision_tx_cnt = 0;
Dev_Work_State_e Vision_DataTx(UART_HandleTypeDef *huart)
{	
//	vision_tx_cnt ++;
	memcpy(vision_txBuf, &vision_tx_info, sizeof(vision_tx_info_t));//设置发送信息
	Append_CRC8_Check_Sum(vision_txBuf, 3);//添加CRC8校验码
	Append_CRC16_Check_Sum(vision_txBuf, sizeof(vision_tx_info_t));//添加CRC16校验码
	
	if(HAL_UART_Transmit_DMA(huart,vision_txBuf,sizeof(vision_tx_info_t)) == HAL_OK)//串口发送
		return DEV_OK;
	
	return DEV_NO;
}   

