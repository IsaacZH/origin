#ifndef _CONFIG_TEST_H
#define _CONFIG_TEST_H

#include "stm32f4xx_hal.h"
#include "motor_def.h"


typedef struct __packed
{
  float RM3508_speed_pid_param[7];
  float RM2006_speed_pid_param[7];
  float GM6020_speed_pid_param[7];
  

  motor_type_e test_motor_type;

  int16_t target_speed;
  
}config_test_t;

extern config_test_t config_test;

#endif