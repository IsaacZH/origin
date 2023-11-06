#ifndef _TEST_H
#define _TEST_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "test_motor.h"
#include "config_test.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Exported variables ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/** 
  * @brief  状态枚举
  */ 
typedef enum 
{
	TEST_OFFLINE = 0,	
	
	TEST_ONLINE,

}test_work_state_e;

/** 
  * @brief  基本信息定义
  */ 
typedef struct __packed 
{
	
	int16_t      target_speed;
	
	int16_t    	 output;
}test_base_info_t;

/** 
  * @brief  
  */ 
typedef struct test_class_t
{	
	/*外部电机*/
	motor_t        		 *motor;

	test_base_info_t  base_info;

	void                (*work)(struct test_class_t *sucker);
	
}test_t;


extern test_t test;

/* Exported functions --------------------------------------------------------*/

/*work*/
void Test_Work(test_t *test);

#endif
