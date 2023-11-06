#ifndef _GIMBAL_MOTOR_H
#define _GIMBAL_MOTOR_H

#include "config.h"
#include "motor.h"

#define ID_GIMBAL_YAW 0x208
#define ID_GIMBAL_PITCH 0x205

typedef enum
{
	GIMBAL_YAW = 0,
	GIMBAL_PITCH,
	
	
  GIMBAL_MOTOR_LIST,
	
}gimbal_motor_list_e;

extern motor_t gimbal_motor[GIMBAL_MOTOR_LIST];

void gimbal_motor_init(void);
void gimbal_motor_heart_beat(void);



#endif






