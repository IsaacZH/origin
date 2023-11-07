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
  .speed_pid_param     = {0,10,0,0,2500,20000,20000},
  .posit_out_pid_param = {0.1,0,5,0,10000,6000,300},
  .posit_in_pid_param  = {0,10,0,0,10000,20000,20000},


  .control_mode = POSITION_MODE,
  
  .target_speed = 100,
  .target_position = 10000,
  
};

/**
 * @brief 信息更新 task中调用
 * 
 * @param control 
 */
void Control_Info_Update(control_t *control)
{
  control->rx_info = &test_motor.rx_info;
  control->state = &test_motor.state;
}

