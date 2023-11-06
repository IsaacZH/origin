#ifndef _CHASSIS_MOTOR_H
#define _CHASSIS_MOTOR_H

#include "config.h"
#include "motor.h"

typedef enum
{
	CHASSIS_LF = 0,
	CHASSIS_LB,
	CHASSIS_RF,
	CHASSIS_RB,
  CHASSIS_MOTOR_LIST,
	
	
	
}chassis_motor_list_e;

extern motor_t chassis_motor[CHASSIS_MOTOR_LIST];

void chassis_motor_init(void);
void chassis_motor_heart_beat(void);

#define ID_CHASSIS_LF 0x201
#define ID_CHASSIS_LB 0x203
#define ID_CHASSIS_RF 0x202
#define ID_CHASSIS_RB 0x204



#endif



