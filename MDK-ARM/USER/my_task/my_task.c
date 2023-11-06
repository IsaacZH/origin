#include "my_task.h"
#include "device.h"
#include "can_protocol.h"
#include "vision_protocol.h"
#include "communicate_protocol.h"
#include "my_judge.h"
#include "judge_protocol.h"
#include "drive.h"
#include "gimbal.h"
#include "car.h"
#include "usart.h"
/**
  * @Name    
  * @brief   
  * @param   
  * @retval 
  * @author  
  * @Date    
**/


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
//		imu_sensor.update(&imu_sensor);
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
		Device_HeartBeat();
		rc_heart_beat(&rc);
		tick_task(1);
    osDelay(1);
  }
}



