#ifndef _SHOOTING_H
#define _SHOOTING_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "shooting_motor.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//限位参数
#define LIMIT_POSIT_VALUE -160000 		//限位拨弹位置增量（负）
#define LIMIT_SPEED       -8000       //限位转速（负）
//拨盘参数
#define DAIL_REVERT_PISITION -5000  //拨盘反转要走的角度（反转为负）
#define DAIL_FEED_SPEED 3000  				//拨盘供弹速度
#define DAIL_STUCK_SPEED -3000    	//拨盘反转处理卡弹的速度(反转为负)
#define DAIL_MAX_WORK_TIMES 1500   //拨盘最大工作时间（判断弹仓是否为空）
#define DAIL_MAX_F_STUCK_CNT 2    	//拨盘正向堵转最大次数（判断是否供弹完成）
/* Exported variables ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/


/**
 * @brief 装填状态枚举
 * 
 */
typedef enum 
{
	load_OK = 0,		//装填完毕	 
	load_NO,     		//需要装弹

}shooting_load_state_e;

/**
 * @brief 拨盘状态枚举
 * 
 */
typedef enum
{
	DAIL_SLEEP = 0, //拨盘休眠等待指令
	DAIL_RELOAD,    //拨盘正转补弹丸
	DAIL_REVERT,    //拨盘反转
}dail_state_e;

/**
 * @brief 拨盘类信息
 * 
 */
typedef struct dail
{
	float    target_speed;     //拨盘目标速度
	int32_t  target_position;  //拨盘反转目标位置
	uint8_t  f_stucking_cnt;   //向前堵转次数
	uint8_t  b_stucking_cnt;   //向后堵转次数
	uint16_t work_times;			//拨盘工作时间
	dail_state_e work_state;  //拨盘工作状态
}dail_info_t;


/**
 * @brief 限位状态枚举
 * 
 */
typedef enum
{
	LIMIT_SLEEP = 0, //限位休眠等待指令
	LIMIT_RUNING,    //限位正在转
}limit_state_e;

/**
 * @brief 限位类信息
 * 
 */
typedef struct limit
{
	float         target_speed;     //限位目标速度
	int32_t       target_position;  //限位目标位置
	limit_state_e work_state;  //限位工作状态
}limit_info_t;

/** 
  * @brief  发射基本信息定义
  */ 
typedef struct __packed 
{
	
	int16_t    	 output_dail;    //拨盘输出
	int16_t			 output_limit;   //限位输出

	dail_info_t  dail_info;//拨盘信息
	limit_info_t limit_info;//限位信息

}shooting_base_info_t;


/**
 * @brief 发射类信息
 * 
 */
typedef struct shooting_class_t
{	
	/*注册电机*/
	motor_t        				 *frictionLF;
	motor_t        				 *frictionRF;
	motor_t        				 *dail;
	motor_t        				 *limit;
	
	shooting_base_info_t   base_info;	

	void               	   (*work)(struct shooting_class_t *shooting);
	
	Dev_Reset_State_e			 shooting_reset_state; //发射初始化状态

  shooting_load_state_e  load_state;  //弹丸装填状态  装好=load_OK
}shooting_t;


extern shooting_t shooting;



/* Exported functions --------------------------------------------------------*/

/*work*/
void Shooting_Work(shooting_t *shooting);

/*pid_cal*/
void Shooting_Pid_Calculating(shooting_t *shooting);

/*发射初始化模式*/
void Shooting_Work_Init(shooting_t *shooting);

/*发射打弹模式*/
uint8_t Shooting_Work_Fire(shooting_t *shooting);

/*补弹*/
uint8_t Shooting_Work_Reloading(shooting_t *shooting);
#endif
