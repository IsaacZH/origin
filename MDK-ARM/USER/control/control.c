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
  .RM3508_speed_pid_param     = {0.9,  0.00084,   15,     0,  20000000,     20000,      9000},
  .RM3508_posit_in_pid_param  = {0.9,  0.00084,   15,     0,  20000000,     20000,      9000},
  .RM3508_posit_out_pid_param = {0.08,    0.08,    9,     0,      2000,      6000,    100000},


  /**
   * @brief pid类型,选用哪一套pid参数
   * PID_GM6020(0) 
   * PID_RM2006(1) 
   * PID_RM3508(2)
   */
  .pid_type = PID_GM6020,
  
  /**
   * @brief 控制模式
   * 速度环：SPEED_MODE(0) 
   * 位置环：POSITION_MODE(1)
   */
  .control_mode = POSITION_MODE, 

  .target_speed    = 0,     //电机目标速度
  .target_position = 0,     //电机目标位置
  
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
  
}

