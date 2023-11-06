#ifndef _CHASSIS_H
#define _CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "chassis_motor.h"

/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_MAX_SPEED						(4000.f) //6000

/** 
  * @brief  状态枚举
  */ 
typedef enum 
{
	CHASSIS_OFFLINE = 0,	
	
	CHASSIS_ONLINE,

}chassis_work_state_e;



/** 
  * @brief  底盘基本信息定义
  */ 
typedef struct __packed 
{
	
	int16_t      target_front_speed;//目标前进速度
	int16_t      target_right_speed;//目标右移速度
	int16_t      target_cycle_speed;//目标旋转速度
	
	int16_t      Front_speed;			  //当前前进速度
	int16_t      Right_speed;			  //当前右移速度
	int16_t      Cycle_speed;			  //当前旋转速度
	
	
	int16_t    	 output_chassisLF;
	int16_t    	 output_chassisLB;
	int16_t    	 output_chassisRF;
	int16_t    	 output_chassisRB;
	
	int16_t    	 target_chassisLF;
	int16_t    	 target_chassisLB;
	int16_t    	 target_chassisRF;
	int16_t    	 target_chassisRB;

	
}chassis_base_info_t;


/** 
  * @brief  底盘类定义
  */ 
typedef struct chassis_class_t
{	

	motor_t        		 *chassisLF;
	motor_t        		 *chassisLB;
	motor_t        		 *chassisRF;
	motor_t        		 *chassisRB;
	
	chassis_base_info_t   	base_info;
	motor_state_e   		work_sate;
	
	void                (*work)(struct chassis_class_t *chassis);
	

}chassis_t;


extern chassis_t chassis;



/* Exported functions --------------------------------------------------------*/

//总控
void Chassis_Work(chassis_t *chassis);




#endif
