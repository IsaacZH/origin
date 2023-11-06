/**
  ******************************************************************************
  * @file           : device.c/h
  * @brief          : 
  * @note           :  
  ******************************************************************************
  */
#include "device.h"
#include "test.h"
device_t device [] =
{	
	[DEVICE_01] = {
	 .motor = &test_motor,	
	},
	
};
/**
  * @Name    Device_Init
  * @brief   设备初始化
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_Init(void)
{
	test_motor_init();
}

/**
  * @Name    Device_HeartBeat
  * @brief   设备心跳
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_HeartBeat(void)
{
	test_motor_heart_beat();
}

/**
  * @Name    Device_Work
  * @brief   设备总控，任务中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_Work(void)
{
	test.work(&test);
	
	if(!RC_ONLINE)
	{
		for(uint16_t i=0;i<DEVICE_CNT;i++)
		{
			device[i].motor->base_info.motor_out = 0;
			device[i].motor->base_info.pid_out = 0;
		}
	}
}

