
/*
* shooting_motor
*	2023.7.19
* 发射
*/


#include "shooting_motor.h"

motor_t  shooting_motor[SHOOTING_MOTOR_LIST] =
{
	[FRICTION_LF] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id = ID_FRICTION_LF, 
		
		.init = Motor_Class_Init,
	},
	[FRICTION_RF] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id = ID_FRICTION_RF, 
		
		.init = Motor_Class_Init,
	},
	[DAIL] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = ID_DAIL, 
		
		.init = Motor_Class_Init,
	},
	[LIMIT] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id = ID_LIMIT, 
		
		.init = Motor_Class_Init,
	},
	
};


																						/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float shooting_3508_speed_pid_param[7] 	= {		27,		0.5,		0,			0,				6000,			  6000,			 10000};

																						/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float dail_speed_pid_param[7]           = {		4,		0.3,		0,			0,				20000,			  6000,			 8000};

																						        /*kp		ki		  kd		b_err		  int_max		  iout_max		out_max*/
float limit_position_in_pid_param[7]           = {		8,		0,	  	0,			0,				3000,			  3000,			 5000};
																						        /*kp		ki		  kd		 b_err		int_max		 iout_max		 out_max*/
float limit_position_out_pid_param[7]         = {		1,		  0,	  	0,			0,				0,			   0,			    5000};

/**
  * @name    shooting_motor_init
  * @brief   电机包初始化，device中调用
  * @param   void
  * @retval  void
  * @author  
  * @date    
**/

void shooting_motor_init(void)
{
	//电机初始化
	for(uint16_t i=0;i<SHOOTING_MOTOR_LIST;i++)
		{ 
			shooting_motor[i].init(&shooting_motor[i]);
		}
		

	//电机速度pid初始化

	//拨盘速度环pid
	shooting_motor[DAIL].pid_init(&shooting_motor[DAIL].pid.speed,dail_speed_pid_param);

	//限位位置环pid
	shooting_motor[LIMIT].pid_init(&shooting_motor[LIMIT].pid.speed,limit_position_in_pid_param);
	
}

/**
  * @Name    shooting_motor_heart_beat
  * @brief   电机心跳，device中调用
  * @param   
  * @retval 
  * @author  
  * @date    
**/

void shooting_motor_heart_beat(void)
{
	for(uint16_t i=0;i<SHOOTING_MOTOR_LIST;i++)
	{ 
		shooting_motor[i].heartbeat(&shooting_motor[i]);
	}
}




