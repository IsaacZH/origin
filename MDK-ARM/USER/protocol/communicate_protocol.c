/**
  ******************************************************************************
  * @file           : communicate_protocol.c
  * @brief          : 
  * @update         : 
  ******************************************************************************
  */

#include "communicate_protocol.h"
#include "stdbool.h"
#include "string.h"
#include "usart.h"
#include "crc.h"

communicate_tx_info_t communicate_tx_info = 
{
	.SOF = 0xA5,
};

communicate_rx_info_t communicate_rx_info;

uint8_t communicate_txBuf[50];


/**
  * @Name    Communicate_DataRx
  * @brief   板间通信收
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
Dev_Work_State_e Communicate_DataRx(uint8_t *rxBuf)
{
	/* 帧首字节是否为0xA5 */
	if(rxBuf[0] == 0xA5)
	{
		/* 帧头CRC8校验*/
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			/* 帧尾CRC16校验 */
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(communicate_rx_info_t)) == true)
			{
				memcpy(&communicate_rx_info, rxBuf, sizeof(communicate_rx_info_t));
				return DEV_OK;
			}
		}
	}
	return DEV_NO;
}


/**
  * @Name    Communicate_DataTx
  * @brief   板间通信发
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
Dev_Work_State_e Communicate_DataTx(void)
{
	memcpy(communicate_txBuf, &communicate_tx_info, sizeof(communicate_tx_info_t));
	Append_CRC8_Check_Sum(communicate_txBuf, 3);
	Append_CRC16_Check_Sum(communicate_txBuf, sizeof(communicate_tx_info_t));
	
	if(HAL_UART_Transmit_DMA(&huart3,communicate_txBuf,sizeof(communicate_tx_info_t)) == HAL_OK)
		return DEV_OK;
	
	return DEV_NO;
}

