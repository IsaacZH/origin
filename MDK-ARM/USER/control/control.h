#ifndef _CONTROL_H
#define _CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motor_def.h"

/**
 * @brief pid类型
 * 
 */
typedef enum
{
  PID_1 = 1,
  PID_2,
  PID_3,
}pid_type_e;

/**
 * @brief 控制模式
 * 
 */
typedef enum
{
  SPEED_MODE = 0,
  POSITION_MODE,  
}control_mode_e;

/**
 * @brief 控制信息
 * 
 */
typedef struct __packed
{
  motor_rx_info_t        *rx_info;
	motor_state_info_t     *state;
  
  float pid_param_1[7];
  float pid_param_2[7];
  float pid_param_3[7];
  
  pid_type_e pid_type;
  control_mode_e control_mode;

  int16_t target_speed;
  int32_t target_position;
}control_t;


extern control_t control;

#endif