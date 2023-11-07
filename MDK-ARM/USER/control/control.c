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
  .pid_param_1 = {8,0.33,0,0,10000,6000,9000},
  .pid_param_2 = {8,0.33,0,0,10000,6000,9000},
  .pid_param_3 = {8,0.33,0,0,10000,6000,9000},


  .pid_type = PID_1,
  .control_mode = SPEED_MODE,
  
  .target_speed = 100,
  
};

