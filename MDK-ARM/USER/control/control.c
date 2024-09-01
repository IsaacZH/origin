/**
 * @file control.c
 * @author Isaac (1812924685@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "control.h"
#include "rp_math.h"

void Outpost_Work();

/*kp		 ki		  kd		b_err	  int_max		iout_max	out_max*/
control_t control = 
{
  /*6020 pid*/                  /*kp	    	ki		kd		b_err	  int_max		iout_max	 out_max*/
  .GM6020_speed_pid_param     = {0,         10,    0,     0,      2500,     20000,     20000},
  .GM6020_posit_in_pid_param  = {0,         10,    0,     0,     10000,     20000,     20000},
  .GM6020_posit_out_pid_param = {0.1,        0,    5,     0,     10000,      6000,       300},
  /*2006 pid*/                                  
  .RM2006_speed_pid_param     = {1.64, 0.00686, 0.82,     0,  20000000,     25000,     25000},
  .RM2006_posit_in_pid_param  = {1.64, 0.00686, 0.82,     0,  20000000,     25000,     25000},
  .RM2006_posit_out_pid_param = {0.7,        0, 15.6,     0,  20000000,     20000,     20000},
  /*3508 pid*/                                  
//  .RM3508_speed_pid_param     = {0.9,  0.00084,   15,     0,  20000000,     20000,      9000},
	.RM3508_speed_pid_param     =  {8,  1,   0,     0,  4000,     4000,      9000},

  .RM3508_posit_in_pid_param  = {0.9,  0.00084,   15,     0,  20000000,     20000,      9000},
  .RM3508_posit_out_pid_param = {0.08,    0.08,    9,     0,      2000,      6000,    100000},

	.outpost_speed_pid_param     = {8,  1,   0,     0,  4000,     4000,      9000},

  /**
   * @brief pid类型,选用哪一套pid参数
   * PID_GM6020(0) 
   * PID_RM2006(1) 
   * PID_RM3508(2)
   */
  .pid_type =PID_RM3508,
  
  /**
   * @brief 控制模式
   * 速度环：SPEED_MODE(0) 
   * 位置环：POSITION_MODE(1)
   */
  .control_mode = SPEED_MODE,                                                                                                                        

  .target_speed    = 0,     //电机目标速度
  .target_position = 0,     //电机目标位置

  .outpost_status = OUTPOST_SLEEP,
  
};

/**
 * @brief 信息更新 task中调用
 * 
 * @param control 
 */
void Control_Info_Update(control_t *control)
{
  control->rx_info = &test_motor.rx_info;
  control->state   = &test_motor.state;
  if (control->state->work_state != M_ONLINE)
  {
    memset(control->rx_info,0,sizeof(motor_rx_info_t));
  }
  Outpost_Work();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (control.pid_type)
  {
  case PID_GM6020:
    if (control.target_speed < 300)
    {
      control.target_speed += 50;
    }
    
    break;
  
  default:
    break;
  }
}

void Outpost_Work()
{
  static int32_t original_motor_angle = 0;
  static uint8_t start_cnt_flag = 0;
  if (control.outpost_status == OUTPOST_SLEEP)
  {
    if (abs(control.rx_info->speed) > OUTPOST_START_SPEED)
    {
      if (start_cnt_flag == 0)
      {
        start_cnt_flag = 1;
        original_motor_angle = control.rx_info->angle_sum;
      }
      
    }
    else
    {
      start_cnt_flag = 0;
    }
    
    if (start_cnt_flag == 1)
    {
      if (control.rx_info->angle_sum - original_motor_angle > OUTPOST_START_POSITION)
      {
        control.outpost_status = OUTPOST_RIGHT;
      }
      else if (original_motor_angle -control.rx_info->angle_sum > OUTPOST_START_POSITION)
      {
        control.outpost_status = OUTPOST_LEFT;
      }
    }
  }

  if (control.outpost_status == OUTPOST_RIGHT)
  {
    control.target_speed = OUTPOST_SPEED;
  }
  
  if (control.outpost_status == OUTPOST_LEFT)
  {
    control.target_speed = -OUTPOST_SPEED;
  }
  
  
}