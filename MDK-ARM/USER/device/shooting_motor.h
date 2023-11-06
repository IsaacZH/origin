
#ifndef _SHOOTING_MOTOR_H
#define _SHOOTING_MOTOR_H

#include "config.h"
#include "motor.h"

/**
 * @brief 发射电机id
 * 
 */
#define ID_FRICTION_LF 0x206 //左前电机ID
#define ID_FRICTION_RF 0x207 //右前电机ID
#define ID_DAIL  			 0x207 //拨盘电机ID
#define ID_LIMIT 			 0x202 //限位电机ID

/**
 * @brief 发射电机列表
 * 
 */
typedef enum
{
	FRICTION_LF = 0,
	FRICTION_RF,
	DAIL,
	LIMIT,
  SHOOTING_MOTOR_LIST,
	
	
}shooting_motor_list_e;


extern motor_t shooting_motor[SHOOTING_MOTOR_LIST];

void shooting_motor_init(void);
void shooting_motor_heart_beat(void);





#endif



