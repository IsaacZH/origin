/**
  ******************************************************************************
  * @file           : RP_drv
  * @brief          : 
  * @note           : start 2022-11-3 
  ******************************************************************************
  */

#include "drv_can.h"
#include "string.h"
#include "config_can.h"
#include "config.h"


CAN_FilterTypeDef CAN_FillterType;
CAN_TxHeaderTypeDef CAN_TxHeadeType;
CAN_RxHeaderTypeDef CAN_RxHeadeType;

/*
	can1 can2实例
*/
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


/**
  * @brief  CAN1初始化
  * @param  
  * @retval 
  */
void CAN1_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	// 配置CAN标识符滤波器
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	
	// 使能接收中断
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	// 开启CAN1
	HAL_CAN_Start(&hcan1);
}

/**
  * @brief  CAN2初始化
  * @param  
  * @retval 
  */
void CAN2_Init(void)
{
	CAN_FilterTypeDef sFilterConfig;
	
	// 配置CAN标识符滤波器
	CAN_Filter_ParamsInit(&sFilterConfig);
	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
	
	// 使能接收中断
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	// 开启CAN2
	HAL_CAN_Start(&hcan2);
}

/**
  * @brief  配置CAN标识符滤波器
  * @param  
  * @retval 
  */
void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig)
{
	sFilterConfig->FilterIdHigh = 0;						
	sFilterConfig->FilterIdLow = 0;							
	sFilterConfig->FilterMaskIdHigh = 0;					// 不过滤
	sFilterConfig->FilterMaskIdLow = 0;						// 不过滤
	sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0;	// 过滤器关联到FIFO0
	sFilterConfig->FilterBank = 0;							// 设置过滤器0
	sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;		// 标识符屏蔽模式
	sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;		// 32位宽
	sFilterConfig->FilterActivation = ENABLE;				// 激活滤波器
	sFilterConfig->SlaveStartFilterBank = 0;
}


void HAL_CAN_TxHeadeInit(uint16_t ID)
{
	CAN_TxHeadeType.StdId = ID;
	CAN_TxHeadeType.ExtId = 0x0000;
	CAN_TxHeadeType.DLC = 8;
	CAN_TxHeadeType.IDE = CAN_ID_STD;
	CAN_TxHeadeType.RTR = CAN_RTR_DATA;
}



