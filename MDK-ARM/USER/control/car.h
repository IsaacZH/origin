#ifndef __CAR_H
#define __CAR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "remote.h"
#include "config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* 车行动模式枚举 */
typedef enum 
{
  offline_CAR,        //离线模式     0
  init_CAR,           //初始化模式   1
  mec_CAR,            //机械模式     2
  gyro_CAR,           //陀螺仪模式   3
	cycle_CAR,					//小陀螺模式   4

}Car_Move_Mode_e;

/* 车控制模式枚举 */
typedef enum 
{
  RC_CTRL_MODE,        //遥控器模式  0
  KEY_CTRL_MODE,       //键盘模式    1

}Car_Ctrl_Mode_e;

/* 执行任务Flag  按键复位->OK 执行完->NO*/
typedef enum 
{
	CMD_RUN_NO, //命令不可执行
	CMD_RUN_OK, //可执行命令
	
}Cmd_Run_Flag_e;

/* 任务执行状态 */
typedef enum 
{
	CMD_FINISH, //执行完成
	CMD_RUNING, //执行中
	
}Cmd_Run_State_e;

/**
 * @brief car类结构体
 * 
 */
typedef struct
{
	Car_Move_Mode_e car_move_mode;


	Dev_Reset_State_e	 car_reset_state;
	Car_Ctrl_Mode_e  car_ctrl_mode;

	Cmd_Run_Flag_e car_l90_run_flag;
	Cmd_Run_Flag_e car_r90_run_flag;
	Cmd_Run_Flag_e gim_180_run_flag;
	Cmd_Run_Flag_e shooting_fire_run_flag;

	Cmd_Run_State_e gim_180_run_state;
	Cmd_Run_State_e shooting_fire_run_state;
	Cmd_Run_State_e shooting_firing_run_state;
}car_t ;

extern car_t car;

extern bool CAR_R90_CMD;      //右转90命令 
extern bool CAR_L90_CMD;      //左转90命令 
extern bool GIM_180_CMD;      //云台掉头命令
extern bool SHOOTING_FIRE_CMD; 		//发射打弹命令
extern bool SHOOTING_FIRING_CMD; //发射连发命令
/* Exported functions --------------------------------------------------------*/
void Car_Ctrl(car_t *car) ;

#endif
