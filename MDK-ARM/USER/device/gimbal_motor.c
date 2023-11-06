
/*
* gimbal_motor
*	注册电机
*	2023.7.19
* 云台
*/


#include "gimbal_motor.h"

motor_t gimbal_motor[GIMBAL_MOTOR_LIST] =
{
	[GIMBAL_YAW] = {
		.id.drive_type = M_CAN1,
		.id.motor_type = GM6020,
		.id.rx_id = ID_GIMBAL_YAW,
		
		.init = Motor_Class_Init,
	},
	[GIMBAL_PITCH] = {
		.id.drive_type = M_CAN2,
		.id.motor_type = GM6020,
		.id.rx_id = ID_GIMBAL_PITCH,
		
		.init = Motor_Class_Init,
	},

};

//GIMBAL_YAW PID define

//陀螺仪角度环																	/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_yaw_gyro_out_pid_param[7] 	 		= {		13,		0,		0,			0,				0,			0 ,			 500};
																							/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_yaw_gyro_in_pid_param[7] 	= {		500,		3,		0,			0,				4000,			4000,		28000};

//机械角度环																				/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_yaw_mec_out_pid_param[7] 	 		= {		1 ,		0,		0,			0,				0,			0 ,			 500};
																									/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_yaw_mec_in_pid_param[7] 	= {		600,		7,		0,			0,				4000,			4000,		28000};

//GIMBAL_PITCH PID define

//陀螺仪角度环																		/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_pitch_gyro_out_pid_param[7] 	 		= {		 15,		0,		0,			0,				0,			0,			 500};
																									/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_pitch_gyro_in_pid_param[7] 	= {		300,		3,		  0,		0,			5000,			5000,		   28000};

//机械角度环							  													/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_pitch_mec_out_pid_param[7] 	 		= {		1.3 ,		0,		0,			0,				0,			0 ,			 500};
																								/*kp		ki		kd		b_err		int_max		iout_max		out_max*/
float gimbal_pitch_mec_in_pid_param[7] 	= {		200 ,		0.4,		0,			0,				4000,			4000,		20000};


/**
  * @Name    gimbal_motor_init
  * @brief   电机包初始化，device中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void gimbal_motor_init(void)
{

	for(uint16_t i=0;i<GIMBAL_MOTOR_LIST;i++)
		{ //电机初始化
			gimbal_motor[i].init(&gimbal_motor[i]);
		}
		
	//电机速度pid初始化
	gimbal_motor[GIMBAL_YAW].pid_init(&gimbal_motor[GIMBAL_YAW].pid.gyro_out ,     gimbal_yaw_gyro_out_pid_param);
	gimbal_motor[GIMBAL_YAW].pid_init(&gimbal_motor[GIMBAL_YAW].pid.gyro_in  ,     gimbal_yaw_gyro_in_pid_param);
		
		
	gimbal_motor[GIMBAL_YAW].pid_init(&gimbal_motor[GIMBAL_YAW].pid.mec_out ,     gimbal_yaw_mec_out_pid_param);
	gimbal_motor[GIMBAL_YAW].pid_init(&gimbal_motor[GIMBAL_YAW].pid.mec_in  ,     gimbal_yaw_mec_in_pid_param);
		
		
	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.gyro_out , gimbal_pitch_gyro_out_pid_param);
	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.gyro_in  , gimbal_pitch_gyro_in_pid_param);
		
	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.mec_out , gimbal_pitch_mec_out_pid_param);
	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.mec_in  , gimbal_pitch_mec_in_pid_param);
		
//	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.angle , gimbal_pitch_mec_out_pid_param);
//	gimbal_motor[GIMBAL_PITCH].pid_init(&gimbal_motor[GIMBAL_PITCH].pid.angle_in  , gimbal_pitch_mec_in_pid_param);
} 

/**
  * @Name    gimbal_motor_heart_beat
  * @brief   电机心跳，device中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void gimbal_motor_heart_beat(void)
{
	
	for(uint16_t i=0;i<GIMBAL_MOTOR_LIST;i++)
	{ 
		gimbal_motor[i].heartbeat(&gimbal_motor[i]);
	}


}




