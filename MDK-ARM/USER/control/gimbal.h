#ifndef _GIMBAL_H
#define _GIMBAL_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "gimbal_motor.h"

/* Private macro -------------------------------------------------------------*/
#define YAW_MOTOR_ANGLE_MIDDLE 1457  		//YAW电机中值 0x05B1
#define PITCH_MOTOR_ANGLE_MIDDLE 4604		//pitch电机中值 0x11FC
#define GIMBAL_MAX_GYRO_ANGEL		(30.f)	//pitch陀螺仪角度电控限位最大值
#define GIMBAL_MIN_GYRO_ANGEL		(-30.f)	//pitch陀螺仪角度电控限位最小值

#define GIMBAL_MAX_MEC_ANGEL   (740.f)				//pitchJ机械角度电控限位最大值
#define GIMBAL_MIN_MEC_ANGEL   (-511.f)			//pitch机械角度电控限位最小值

/* Private typedef -----------------------------------------------------------*/

/** 
  * @brief  云台基本信息定义
  */ 
typedef struct __packed 
{
	float yaw_imu_angle;										//云台陀螺仪yaw轴角度
	float yaw_imu_speed;										//云台陀螺仪yaw轴速度
	float pitch_imu_angle;									//云台陀螺仪pitch轴角度
	float pitch_imu_speed;                  //云台陀螺仪pitch轴速度
	
	float  yaw_imu_angle_target;         //陀螺仪模式目标yaw   世界坐标系 (-180°~180°) (顺时针为正)
	float  pitch_imu_angle_target;      //陀螺仪模式目标pitch  世界坐标系 (-90°~90°)   (向上为正)
	
	int16_t  yaw_motor_angle;           //yaw轴 相对底盘  角度(-4096~4096)      (顺时针为正)
	int16_t  yaw_motor_speed;           //yaw轴 相对底盘  速度(rpm)             (顺时针为正)
	
	int16_t  pitch_motor_angle;           //pitch轴 相对底盘   角度(-2048~2048)    (向上为正)
	int16_t  pitch_motor_speed;           //pitch轴 相对底盘   速度(rpm)           (向上为正)
	

	float  yaw_mec_angle_target;				//机械模式目标yaw		底盘坐标系(-180°~180°)
	float  pitch_mec_angle_target;			//机械模式目标pitch	底盘坐标系	(-2048~2048)  (向上为正)
	
	int16_t  output_gimbal_y;							//yaw轴电机输出
	int16_t  output_gimbal_p;							//pitch轴电机输出

	int16_t  cmd_yaw_angle_last;   //存储掉头前yaw角度
}gimbal_base_info_t;


/** 
  * @brief  云台类定义
  */ 
typedef struct gimbal_class_t
{	

	motor_t        		 *gimbal_p;
	motor_t        		 *gimbal_y;

	
	gimbal_base_info_t   	base_info;
//	motor_state_e   		work_sate;
	
	void                (*work)(struct gimbal_class_t *gimbal);
	Dev_Reset_State_e			 gimbal_reset_state;
	
}gimbal_t;


extern gimbal_t gimbal;



/* Private variables ---------------------------------------------------------*/


/* Exported variables ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void Gimbal_Work(gimbal_t *gimbal);													//总控
void Gimbal_Imu_Update(gimbal_t *gimbal);										//云台陀螺仪数据更新
#endif
