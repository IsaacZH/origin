/*
* chassis
*	
*	2023.9.11
* 底盘
*/

/* Includes ------------------------------*/
#include "chassis.h"
#include "gimbal.h"
#include "device.h"
#include "rp_math.h"
#include "car.h"

/* Private function prototypes -----------------------------------------------*/

void Chassis_Speed_Calculating(chassis_t *chassis);//麦轮底盘电机目标速度解算
void Chassis_Pid_Calculating(chassis_t *chassis);//底盘PID计算
void Chassis_Speed_Update(chassis_t *chassis);//计算底盘当前速度

void Chassis_Gyro_Update(chassis_t *chassis,uint8_t ctrl_mode, uint8_t move_mode);//底盘陀螺仪模式更新
void Chassis_Mec_Update(chassis_t *chassis,uint8_t ctrl_mode);//底盘机械模式更新 

void Chassis_Work_Mec(chassis_t *chassis);//机械模式
void Chassis_Work_Gyro(chassis_t *chassis);//陀螺仪模式

/* Exported variables --------------------*/
chassis_t chassis = 
{
	.chassisLF = &chassis_motor[CHASSIS_LF],
	.chassisLB = &chassis_motor[CHASSIS_LB],
	.chassisRF = &chassis_motor[CHASSIS_RF],
	.chassisRB = &chassis_motor[CHASSIS_RB],
	
	.work = Chassis_Work,
	
};

/* Function  body --------------------------------------------------------*/



/**
  * @brief  底盘当前速度速度更新
  * @param  
  * @retval 
**/
void Chassis_Speed_Update(chassis_t *chassis)
{
	//获取当前电机速度
	int16_t LF_speed = chassis->chassisLF->rx_info.speed;
	int16_t LB_speed = chassis->chassisLB->rx_info.speed;
	int16_t RF_speed = chassis->chassisRF->rx_info.speed;
	int16_t RB_speed = chassis->chassisRB->rx_info.speed;
	

	
	//计算速度
	chassis->base_info.Front_speed = (LF_speed - RF_speed + LB_speed - RB_speed) / 4;
	chassis->base_info.Right_speed = (LF_speed + RF_speed - LB_speed - RB_speed) / 4;
	chassis->base_info.Cycle_speed = (LF_speed + RF_speed + LB_speed + RB_speed) / 4;
	

}

/**
 * @brief 底盘机械模式更新
 * 
 * @param chassis 
 * @param ctrl_move 0:遥控器模式 1：键盘模式
 */
void Chassis_Mec_Update(chassis_t *chassis,uint8_t ctrl_mode)
{
	if(ctrl_mode == 0)//遥控器模式
	{
		chassis->base_info.target_front_speed = (float) rc.base_info->ch3 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed = (float) rc.base_info->ch2 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_cycle_speed = (float) rc.base_info->ch0 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
	}
	else if (ctrl_mode == 1)//键盘模式
	{
		chassis->base_info.target_front_speed = 0;
		chassis->base_info.target_right_speed = 0;
		chassis->base_info.target_front_speed += (float)rc.base_info->W.cnt / (float)KEY_W_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_front_speed -= (float)rc.base_info->S.cnt / (float)KEY_S_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed += (float)rc.base_info->D.cnt / (float)KEY_D_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed -= (float)rc.base_info->A.cnt / (float)KEY_A_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_cycle_speed =  rc.info->mouse_x_K;
	}
}

/**
 * @brief 底盘陀螺仪模式更新
 * 
 * @param chassis 
 * @param ctrl_move 0:遥控器模式 1：键盘模式
 * @param move_mode 0:陀螺仪模式 1：小陀螺模式
 * @note 通过“底盘机械模式更新”更新目标值，再把目标值映射到底盘
 */
void Chassis_Gyro_Update(chassis_t *chassis,uint8_t ctrl_mode, uint8_t move_mode)
{
	Chassis_Mec_Update(chassis,ctrl_mode);
	float front = chassis->base_info.target_front_speed;
	float right = chassis->base_info.target_right_speed;

	//计算陀螺仪坐标和底盘坐标角度差
	int16_t yaw_angle_err;
	float yaw_angle_err_rad; 
	yaw_angle_err  = gimbal.base_info.yaw_motor_angle;							//yaw轴   相对底盘   角度(-4096~4096)(顺时针为正)
	yaw_angle_err_rad = (double)yaw_angle_err / 4096.f * 3.14159;   //yaw轴角度转弧度制（一圈2π）

	//front和right值计算
	chassis->base_info.target_front_speed =  front * cos(yaw_angle_err_rad) - right * sin(yaw_angle_err_rad);
	chassis->base_info.target_right_speed =  right * cos(yaw_angle_err_rad) + front * sin(yaw_angle_err_rad);


	if(move_mode == 0)
	{
		//cycle值计算
		if(abs(yaw_angle_err) > 2048)//头可以朝后
		{
			yaw_angle_err -= 4096 * sgn(yaw_angle_err);
		}
		if(abs(yaw_angle_err) >= 100) //大于4.4°斜率变大 
		{
			chassis->base_info.target_cycle_speed = yaw_angle_err * 6.f - 300.f * sgn(yaw_angle_err);
		}
		else 
		{
			chassis->base_info.target_cycle_speed = yaw_angle_err * 3.f;
		}
	}
	else if (move_mode == 1)
	{
		chassis->base_info.target_cycle_speed = 3000;//小陀螺速度
	}
	
}


/**
  * @Name    Chassis_Speed_Calculating
  * @brief   底盘电机目标速度计算
  * @param   
  * @retval
  * @author  HWX CCB Isaac 
  * @Date    2022-12-3
**/
void Chassis_Speed_Calculating(chassis_t *chassis)
{
	int16_t front = chassis->base_info.target_front_speed;
	int16_t right = chassis->base_info.target_right_speed;
	int16_t cycle = chassis->base_info.target_cycle_speed;
	
		
	int16_t speed_sum;
	float K;
	
	speed_sum = abs(front) + abs(right) + abs(cycle);
	
	if(speed_sum > CHASSIS_MAX_SPEED)
	{
		K = (float)CHASSIS_MAX_SPEED / (float)speed_sum;
	}
	else 
	{
		K = 1;
	}

	front *= K;
	right *= K;
	cycle *= K;
	
	chassis->base_info.target_chassisLF   =   front + right + cycle; 
	chassis->base_info.target_chassisLB   =   front - right + cycle;
	chassis->base_info.target_chassisRF   = - front + right + cycle; 
	chassis->base_info.target_chassisRB   = - front - right + cycle; 
	
}

/**
  * @Name    
  * @brief  PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Pid_Calculating(chassis_t *chassis)
{

	chassis->base_info.output_chassisLF 	= chassis->chassisLF->c_speed(chassis->chassisLF, chassis->base_info.target_chassisLF);
	chassis->base_info.output_chassisLB 		= chassis->chassisLB->c_speed(chassis->chassisLB, chassis->base_info.target_chassisLB);	
	chassis->base_info.output_chassisRF 	= chassis->chassisRF->c_speed(chassis->chassisRF, chassis->base_info.target_chassisRF);
	chassis->base_info.output_chassisRB 		= chassis->chassisRB->c_speed(chassis->chassisRB, chassis->base_info.target_chassisRB);
}

/**
  * @Name    
  * @brief   底盘机械模式
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Mec(chassis_t *chassis)
{
	Chassis_Speed_Update(chassis);
	Chassis_Mec_Update(chassis,car.car_ctrl_mode);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}

/**
  * @Name    
  * @brief   底盘陀螺仪模式
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Gyro(chassis_t *chassis)
{
	
	Chassis_Speed_Update(chassis);
	Chassis_Gyro_Update(chassis,car.car_ctrl_mode,0);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}	

/**
  * @Name    
  * @brief   底盘小陀螺模式
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Cycle(chassis_t *chassis)
{
	
	Chassis_Speed_Update(chassis);
	Chassis_Gyro_Update(chassis,car.car_ctrl_mode,1);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}	

/**
  * @Name    Chassis_Work
  * @brief   总控
  * @param   
  * @retval 
  * @author  
  * @Date    
**/

void Chassis_Work(chassis_t *chassis)
{

  
  switch(car.car_move_mode)
	{

		case(offline_CAR):	
			chassis->base_info.output_chassisLF = 0;
			chassis->base_info.output_chassisLB = 0;
			chassis->base_info.output_chassisRF = 0;
			chassis->base_info.output_chassisRB = 0;
			break;
		case(init_CAR):
      chassis->base_info.target_cycle_speed = 0;
      chassis->base_info.target_front_speed = 0;
      chassis->base_info.target_right_speed = 0;
		break;
		case(mec_CAR):
			Chassis_Work_Mec(chassis);
			break;
		case(gyro_CAR):
			//命令响应
  			if(car.gim_180_run_state == CMD_RUNING)
  			{
  			  Chassis_Work_Mec(chassis);
  			}
				else
				{
					Chassis_Work_Gyro(chassis);
				}
			break;
		case(cycle_CAR):
			Chassis_Work_Cycle(chassis);
			break;
		default:
			break;
	}


}

