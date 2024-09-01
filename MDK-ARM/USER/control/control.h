#ifndef _CONTROL_H
#define _CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "motor_def.h"
#include "device.h"
#include "string.h"

#define OUTPOST_SPEED 824

#define OUTPOST_START_SPEED 20
#define OUTPOST_START_POSITION 1000
/**
 * @brief ����ģʽ
 * 
 */
typedef enum
{
  SPEED_MODE = 0,
  POSITION_MODE,  
}control_mode_e;

/**
 * @brief pid����
 * 
 */
typedef enum
{
  PID_GM6020 = 0,
  PID_RM2006,
  PID_RM3508,
}pid_type_e;

/**
 * @brief ǰ��վ״̬
 * 
 */
typedef enum
{
  OUTPOST_SLEEP,
  OUTPOST_LEFT,
  OUTPOST_RIGHT,
}outpost_status_e;

/**
 * @brief ������Ϣ
 * 
 */
typedef struct __packed
{
    /*pid����*/
  float GM6020_speed_pid_param[7];
  float GM6020_posit_out_pid_param[7];
  float GM6020_posit_in_pid_param[7];

  float RM3508_speed_pid_param[7];
  float RM3508_posit_out_pid_param[7];
  float RM3508_posit_in_pid_param[7];

  float RM2006_speed_pid_param[7];
  float RM2006_posit_out_pid_param[7];
  float RM2006_posit_in_pid_param[7];
	
	float outpost_speed_pid_param[7];

	motor_rx_info_t    *rx_info;  //������Ϣ
	motor_state_info_t *state;    //״̬��Ϣ
  
  pid_type_e     pid_type;         //pid����
  control_mode_e control_mode;     //����ģʽ
  int16_t        target_speed;     //Ŀ���ٶ�
  int32_t        target_position;  //Ŀ��λ��

  outpost_status_e outpost_status;
}control_t;


extern control_t control;
void Control_Info_Update(control_t *control);

#endif