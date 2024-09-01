#include "my_task.h"
#include "device.h"
#include "can_protocol.h"
#include "drive.h"
#include "usart.h"
#include "crc.h"
/**
  * @Name    
  * @brief   
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
HAL_StatusTypeDef Usart_Test(UART_HandleTypeDef *huart);

/**
  * @Name    StartControlTask
  * @brief   控制任务
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
extern IWDG_HandleTypeDef hiwdg;
void StartControlTask(void const * argument)
{
	Device_Init();
	Drive_Init();
	RC_Init(&rc);				//device.h -> remote.h
  for(;;)
  {
		Device_Work();

		CAN_Send();
    Usart_Test(&huart1);
    Usart_Test(&huart3);
    Usart_Test(&huart6);
		
		osDelay(1);

  }
}

/**
  * @Name    StartRealTimeTask
  * @brief   实时任务更新
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void StartRealTimeTask(void const * argument)
{
	for(;;)
  {
		rc_ctrl(&rc);
		imu_sensor.update(&imu_sensor);
		HAL_IWDG_Refresh(&hiwdg);
    osDelay(1);
  }
}

/**
  * @Name    StartHeartBeatTask
  * @brief   监控任务
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void StartHeartBeatTask(void const * argument)
{
  for(;;)
  {
    Control_Info_Update(&control);
		Device_HeartBeat();
		rc_heart_beat(&rc);
    osDelay(1);
  }
}



HAL_StatusTypeDef Usart_Test(UART_HandleTypeDef *huart)
{
  uint8_t tx_buff[10];
  Append_CRC8_Check_Sum(tx_buff, 3);//添加CRC8校验码
	Append_CRC16_Check_Sum(tx_buff, sizeof(tx_buff));//添加CRC16校验码
  if(HAL_UART_Transmit_DMA(huart,tx_buff,sizeof(tx_buff)) == HAL_OK)//串口发送
	{
    return HAL_OK;
	}
  return HAL_ERROR;
}