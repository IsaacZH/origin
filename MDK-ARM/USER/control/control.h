#ifndef _CONTROL_H
#define _CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motor_def.h"
#include "device.h"
#include "string.h"
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
 * @brief pid类型
 * 
 */
typedef enum
{
  PID_GM6020 = 0,
  PID_RM2006,
  PID_RM3508,
}pid_type_e;

/**
 * @brief 控制信息
 * 
 */
typedef struct __packed
{
    /*pid参数*/
  float GM6020_speed_pid_param[7];
  float GM6020_posit_out_pid_param[7];
  float GM6020_posit_in_pid_param[7];

  float RM3508_speed_pid_param[7];
  float RM3508_posit_out_pid_param[7];
  float RM3508_posit_in_pid_param[7];

  float RM2006_speed_pid_param[7];
  float RM2006_posit_out_pid_param[7];
  float RM2006_posit_in_pid_param[7];

	motor_rx_info_t    *rx_info;  //接收信息
	motor_state_info_t *state;    //状态信息
  
  pid_type_e     pid_type;         //pid类型
  control_mode_e control_mode;     //控制模式
  int16_t        target_speed;     //目标速度
  int32_t        target_position;  //目标位置
}control_t;


extern control_t control;
void Control_Info_Update(control_t *control);

#endif