/*
* gimbal
*	
*	
* 云台
*/


/* Includes ------------------------------------------------------------------*/
#include "gimbal.h"
#include "device.h"
#include "rp_math.h"
#include "car.h"
/* Private function prototypes -----------------------------------------------*/

void Gimbal_Can_Update(gimbal_t *gimbal);								//云台 yaw pitch can 更新

void Gimbal_Gyro_Update(gimbal_t *gimbal,uint8_t ctrl_mode);      //陀螺仪模式遥控器更新
void Gimbal_Mec_Update(gimbal_t *gimbal,uint8_t ctrl_mode);       //机械模式遥控器更新

void Gimbal_Pitch_Gyro_Angle_Limit(gimbal_t *gimbal);						//云台pitch轴陀螺仪角度限位
void Gimbal_Pitch_Mec_Angle_Limit(gimbal_t *gimbal);					//云台pitch轴机械角度限位
void Gimbal_Yaw_Angle_Check(gimbal_t *gimbal);              //云台yaw目标角度检查

void Gimbal_Gyro_Pid_Calculating(gimbal_t *gimbal);					//陀螺仪模式 PID Calculating
void Gimbal_Mec_Pid_Calculating(gimbal_t *gimbal);          //机械模式 PID Calculating 
	
void Gimbal_Work_Gyro(gimbal_t *gimbal);											//陀螺仪模式
/* Exported variables --------------------------------------------------------*/

gimbal_t gimbal = 
{
	.gimbal_y = &gimbal_motor[GIMBAL_YAW],
	.gimbal_p = &gimbal_motor[GIMBAL_PITCH],

	
	.work = Gimbal_Work,
	
	.gimbal_reset_state = DEV_RESET_NO,
};
/* Function  body --------------------------------------------------------*/





/**
  * @brief  云台陀螺仪数据更新
  * @param  
  * @retval 
  */
void Gimbal_Imu_Update(gimbal_t *gimbal)
{

	gimbal->base_info.yaw_imu_angle = -imu_sensor.info->base_info.yaw;
//	vector_update(&vector_yaw_imu_angle, info->yaw_imu_angle);
	gimbal->base_info.yaw_imu_speed = -imu_sensor.info->base_info.rate_yaw;
	gimbal->base_info.pitch_imu_angle = -imu_sensor.info->base_info.pitch;
	gimbal->base_info.pitch_imu_speed = -imu_sensor.info->base_info.rate_pitch;
  
//	vector_update(&vector_yaw_imu_speed, info->yaw_imu_speed);
}

/**
  * @brief  云台pitch轴陀螺仪角度限位
  * @param  
  * @retval 
  */
void Gimbal_Pitch_Gyro_Angle_Limit(gimbal_t *gimbal)
{

	float angle = gimbal->base_info.pitch_imu_angle_target;
	if(angle > GIMBAL_MAX_GYRO_ANGEL)
	{
		angle = GIMBAL_MAX_GYRO_ANGEL;
	}
	if(angle < GIMBAL_MIN_GYRO_ANGEL)
	{
		angle = GIMBAL_MIN_GYRO_ANGEL;
	}
	gimbal->base_info.pitch_imu_angle_target = angle;
}

/**
  * @brief  云台pitch轴机械角度限位
  * @param  
  * @retval 
  */
void Gimbal_Pitch_Mec_Angle_Limit(gimbal_t *gimbal)
{

	float angle = gimbal->base_info.pitch_mec_angle_target;
	if(angle > GIMBAL_MAX_MEC_ANGEL)
	{
		angle = GIMBAL_MAX_MEC_ANGEL;
	}
	if(angle < GIMBAL_MIN_MEC_ANGEL)
	{
		angle = GIMBAL_MIN_MEC_ANGEL;
	}
	gimbal->base_info.pitch_mec_angle_target = angle;
}



/**
  * @brief  云台陀螺仪yaw目标角度检查 陀螺仪目标角度是累加的需要限制
  * @param  
  * @retval 
  */
void Gimbal_Yaw_Angle_Check(gimbal_t *gimbal)
{
	float angle = gimbal->base_info.yaw_imu_angle_target;//-180°~180°
	while (abs(angle) > 180)
	{
		angle -= 360 * sgn(angle);
	}
	gimbal->base_info.yaw_imu_angle_target = angle;
	
}
/**
  * @brief  云台 yaw pitch can 更新
  * @param  
  * @retval 
  */
void Gimbal_Can_Update(gimbal_t *gimbal)
{
	//过零点处理
	if(gimbal->gimbal_y->rx_info.angle > (YAW_MOTOR_ANGLE_MIDDLE + 4096)) 
	{
		gimbal->base_info.yaw_motor_angle =  8192 + YAW_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_y->rx_info.angle;
	}
	else
	{
		gimbal->base_info.yaw_motor_angle =  YAW_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_y->rx_info.angle;
	}
	
	gimbal->base_info.yaw_motor_speed = - gimbal->gimbal_y->rx_info.speed;  
	
	gimbal->base_info.pitch_motor_angle =  PITCH_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_p->rx_info.angle;
	gimbal->base_info.pitch_motor_speed =  - gimbal->gimbal_p->rx_info.speed;  
	

}

/**
 * @brief 陀螺仪模式遥控器更新
 * 
 * @param gimbal 
 * @param ctrl_mode 0:遥控器模式 1：键盘模式
 */
void Gimbal_Gyro_Update(gimbal_t *gimbal,uint8_t ctrl_mode)
{
	if(ctrl_mode == 0)//遥控器模式
	{

		gimbal->base_info.yaw_imu_angle_target +=  rc.base_info->ch0 / 2000.f; //最快增量330dps
		gimbal->base_info.pitch_imu_angle_target  +=  rc.base_info->ch1 / 10000.f; 
	}
	else if(ctrl_mode == 1)//键盘模式
	{
		gimbal->base_info.yaw_imu_angle_target +=  rc.info->mouse_x_K / 500.f; 
		gimbal->base_info.pitch_imu_angle_target  -= rc.info->mouse_y_K / 1000.f; 

	}
	Gimbal_Yaw_Angle_Check(gimbal);
	Gimbal_Pitch_Gyro_Angle_Limit(gimbal);
	
	//机械pitch实时更新
	gimbal->base_info.pitch_mec_angle_target = gimbal->base_info.pitch_motor_angle;
	Gimbal_Pitch_Mec_Angle_Limit(gimbal);
}

/**
  * @Name    
  * @brief  机械模式更新 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Mec_Update(gimbal_t *gimbal,uint8_t ctrl_mode)
{
	gimbal->base_info.yaw_mec_angle_target = 0 ;
	if (ctrl_mode == 0)
	{
		gimbal->base_info.pitch_mec_angle_target  +=  rc.base_info->ch1 / 500.f; 
	}
	else if(ctrl_mode == 1)
	{
		gimbal->base_info.pitch_mec_angle_target  -= rc.info->mouse_y_K / 3000.f; 
	}
	 
	
	
	gimbal->base_info.yaw_imu_angle_target = gimbal->base_info.yaw_imu_angle;
	Gimbal_Pitch_Mec_Angle_Limit(gimbal);
	
	gimbal->base_info.pitch_imu_angle_target = gimbal->base_info.pitch_imu_angle;
	gimbal->base_info.yaw_imu_angle_target = gimbal->base_info.yaw_imu_angle;
	Gimbal_Pitch_Gyro_Angle_Limit(gimbal);
}
/**
  * @Name    
  * @brief 陀螺仪模式 PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Gyro_Pid_Calculating(gimbal_t *gimbal)
{
	//yaw pid
	float yaw_meas_out = gimbal->base_info.yaw_imu_angle;				//外环
	float yaw_meas_in = gimbal->base_info.yaw_imu_speed	;			//内环
	float yaw_target = gimbal->base_info.yaw_imu_angle_target;
	
	gimbal->base_info.output_gimbal_y = -gimbal->gimbal_y->c_pid2( &gimbal->gimbal_y->pid.gyro_out ,  \
																																&gimbal->gimbal_y->pid.gyro_in  ,	 \
																																yaw_meas_out ,yaw_meas_in ,yaw_target ,3);
	
	//pitch pid
	float pitch_meas_out = gimbal->base_info.pitch_imu_angle;				//外环
	float pitch_meas_in = gimbal->base_info.pitch_imu_speed	;			//内环
	float pitch_target = gimbal->base_info.pitch_imu_angle_target;
	
	gimbal->base_info.output_gimbal_p = -gimbal->gimbal_p->c_pid2( &gimbal->gimbal_p->pid.gyro_out,	\
																																&gimbal->gimbal_p->pid.gyro_in,		\
																																pitch_meas_out,pitch_meas_in,pitch_target,1);
}

/**
  * @Name    
  * @brief 机械模式 PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Mec_Pid_Calculating(gimbal_t *gimbal)
{
	//yaw pid
	float yaw_meas_out = gimbal->base_info.yaw_motor_angle / 4096.f * 180.f;	 //外环 转为角度
	float yaw_meas_in = gimbal->base_info.yaw_motor_speed	;			//内环
	float yaw_target = gimbal->base_info.yaw_mec_angle_target;
	
	gimbal->base_info.output_gimbal_y = -gimbal->gimbal_y->c_pid2( &gimbal->gimbal_y->pid.mec_out ,  \
																																&gimbal->gimbal_y->pid.mec_in  ,	 \
																																yaw_meas_out ,yaw_meas_in ,yaw_target ,3);
	
	//pitch pid
	float pitch_meas_out = gimbal->base_info.pitch_motor_angle ;				//外环
	float pitch_meas_in = gimbal->base_info.pitch_motor_speed	;			//内环
	float pitch_target = gimbal->base_info.pitch_mec_angle_target;
	
	gimbal->base_info.output_gimbal_p = -gimbal->gimbal_p->c_pid2( &gimbal->gimbal_p->pid.mec_out,	\
																																&gimbal->gimbal_p->pid.mec_in,		\
																																pitch_meas_out,pitch_meas_in,pitch_target,1);
}
/**
  * @Name    
  * @brief  陀螺仪模式 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Work_Gyro(gimbal_t *gimbal)
{
	if(car.gim_180_run_state != CMD_RUNING)//换头时遥控器不能改变目标值
	{
		Gimbal_Gyro_Update(gimbal,car.car_ctrl_mode);
	}
	Gimbal_Gyro_Pid_Calculating(gimbal);
	
	//电机输出
	gimbal->gimbal_p->base_info.motor_out = gimbal->base_info.output_gimbal_p;
	gimbal->gimbal_y->base_info.motor_out = gimbal->base_info.output_gimbal_y;
}

/**
  * @Name    
  * @brief  机械模式 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Work_Mec(gimbal_t *gimbal)
{
	Gimbal_Mec_Update(gimbal,car.car_ctrl_mode);
	Gimbal_Mec_Pid_Calculating(gimbal);
	
	//电机输出
	gimbal->gimbal_p->base_info.motor_out = gimbal->base_info.output_gimbal_p;
	gimbal->gimbal_y->base_info.motor_out = gimbal->base_info.output_gimbal_y;
}

/**
  * @Name    Gimbal_Work
  * @brief   总控
  * @param   
  * @retval 
  * @author  
  * @Date    
**/

void Gimbal_Work(gimbal_t *gimbal)
{
	Gimbal_Can_Update(gimbal);
	
	//命令执行
	if(CAR_R90_CMD  ==  true && car.car_r90_run_flag == CMD_RUN_OK) 
	{
		gimbal->base_info.yaw_imu_angle_target += 90;
		car.car_r90_run_flag = CMD_RUN_NO; //执行完成，等待按键复位
		
	}
	if(CAR_L90_CMD  ==  true && car.car_l90_run_flag == CMD_RUN_OK) 
	{
		gimbal->base_info.yaw_imu_angle_target -= 90;
		car.car_l90_run_flag = CMD_RUN_NO; //执行完成，等待按键复位
	}

  //云台转180
	int16_t *angel_last = &gimbal->base_info.cmd_yaw_angle_last;
	if(GIM_180_CMD  ==  true )
	{
		if(car.gim_180_run_flag == CMD_RUN_OK)
		{		
			car.gim_180_run_state = CMD_RUNING;
			*angel_last = gimbal->base_info.yaw_imu_angle;
			gimbal->base_info.yaw_imu_angle_target = *angel_last - sgn(*angel_last)*180;
			car.gim_180_run_flag = CMD_RUN_NO;
		}
	}
  if(abs(gimbal->base_info.yaw_imu_angle- (*angel_last - sgn(*angel_last)*180)) < 1)
 	{
  	car.gim_180_run_state = CMD_FINISH;
 	}
	
	//模式判断
	switch(car.car_move_mode)
	{
		//离线
		case(offline_CAR):
			gimbal->base_info.output_gimbal_y = 0;
			gimbal->base_info.output_gimbal_p = 0;
			gimbal->gimbal_reset_state = DEV_RESET_NO;
			break;
		//初始化
		case(init_CAR):
			gimbal->base_info.pitch_mec_angle_target = 0;
      gimbal->base_info.yaw_mec_angle_target = 0;
			//判断初始化是否完成
			if(abs(gimbal->base_info.yaw_motor_angle) <= 13 && abs(gimbal->base_info.pitch_motor_angle) <= 40   \
      && abs(gimbal->base_info.pitch_motor_speed) <= 3 && abs(gimbal->base_info.pitch_motor_speed) <= 3 )
			{
				gimbal->gimbal_reset_state = DEV_RESET_OK;
			}
			else
			{
				//机械模式进行初始化
        Gimbal_Work_Mec(gimbal);
			}
		break;
		//机械模式
		case(mec_CAR):
			Gimbal_Work_Mec(gimbal);
			break;
		//陀螺仪模式
		case(gyro_CAR):
			Gimbal_Work_Gyro(gimbal);
			break;
		//小陀螺模式
		case(cycle_CAR):
			Gimbal_Work_Gyro(gimbal);
			break;
		default:
			break;
			
	}

}







