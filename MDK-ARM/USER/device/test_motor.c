/**
 * @file test_motor.c
 * @author Isaac (1812924685@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include "test_motor.h"

void Motor_Class_Pid_Update(struct motor_pid *pid, float *buff);
void Test_Motor_Config_Update(void);

motor_t test_motor =
{		
	.id.drive_type = M_CAN1,
	.id.motor_type = GM6020,

	.init = Motor_Class_Init,
};

/**
  * @Name    test_motor_init
  * @brief   电机包初始化，device中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void test_motor_init(void)
{
	test_motor.init(&test_motor);
  
}

/**
  * @Name    test_motor_heart_beat
  * @brief   电机心跳，device中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void test_motor_heart_beat(void)
{
	test_motor.heartbeat(&test_motor);
  Test_Motor_Config_Update();
}

/**
 * @brief 电机配置更新
 * 
 */
void Test_Motor_Config_Update(void)
{

  get_rm_can_drvie(&test_motor);
  
  switch (control.pid_type)
  {
  case PID_1:
	  Motor_Class_Pid_Update(&test_motor.pid.speed,control.pid_param_1);
    break;
  case PID_2:
    Motor_Class_Pid_Update(&test_motor.pid.speed,control.pid_param_2);
    break;
  case PID_3:
    Motor_Class_Pid_Update(&test_motor.pid.speed,control.pid_param_3);
    break;
  default:
    break;
  }
}

/**
 * @brief 电机PID更新
 * 
 * @param pid 
 * @param buff 
 */
void Motor_Class_Pid_Update(struct motor_pid *pid, float *buff)
{

	//PID 参数
	pid->set.kp           = buff[0];
	pid->set.ki           = buff[1];	
	pid->set.kd           = buff[2];
	pid->set.blind_err    = buff[3];
	pid->set.integral_max = buff[4];
	pid->set.iout_max     = buff[5];	
	pid->set.out_max      = buff[6];
	//初始化成功
	pid->info.init_flag = M_INIT;
}	

