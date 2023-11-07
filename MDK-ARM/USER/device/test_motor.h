#ifndef _TEST_MOTOR_H
#define _TEST_MOTOR_H

#include "config.h"
#include "motor.h"
#include "control.h"
typedef enum
{
  TEST_MOTOR_LIST,
	
}test_motor_list_e;

extern motor_t test_motor;

void test_motor_init(void);
void test_motor_heart_beat(void);



#endif
