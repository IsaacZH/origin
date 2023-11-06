/**
 * @file config_test.c
 * @author Isaac (1812924685@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "config_test.h"

/*kp		 ki		  kd		b_err	  int_max		iout_max	out_max*/
config_test_t config_test = 
{
  .GM6020_speed_pid_param = {8,0.33,0,0,10000,6000,9000},
  .RM2006_speed_pid_param = {8,0.33,0,0,10000,6000,9000},
  .RM3508_speed_pid_param = {8,0.33,0,0,10000,6000,9000},


  .test_motor_type = GM6020,
  .target_speed = 0,
};

