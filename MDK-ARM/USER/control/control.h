#ifndef _CONTROL_H
#define _CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motor_def.h"
#include "device.h"
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
  
  float speed_pid_param[7];
  float posit_out_pid_param[7];
  float posit_in_pid_param[7];
    
  control_mode_e control_mode;

  int16_t target_speed;
  int32_t target_position;
}control_t;


extern control_t control;
void Control_Info_Update(control_t *control);

#endif