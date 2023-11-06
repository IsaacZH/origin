/**
  ******************************************************************************
  * @file           : communicate_potocol.c\h
  * @brief          : 
  * @update         : 
  ******************************************************************************
  */

#ifndef __COMMUNICATE_PROTOCOL_H
#define __COMMUNICATE_PROTOCOL_H

#include "config.h"

typedef struct __packed 
{
	uint8_t  	SOF;
	uint8_t  	datau8_1;
	uint8_t  	CRC8;
	
	
	uint16_t CRC16;
}communicate_rx_info_t;

typedef struct __packed 
{
	uint8_t  	SOF;
	uint8_t  	datau8_1;
	uint8_t  	CRC8;
	

	uint16_t CRC16;
}communicate_tx_info_t;

extern communicate_tx_info_t communicate_tx_info;
extern communicate_rx_info_t communicate_rx_info;

Dev_Work_State_e Communicate_DataTx(void);
Dev_Work_State_e Communicate_DataRx(uint8_t *rxBuf);

#endif
