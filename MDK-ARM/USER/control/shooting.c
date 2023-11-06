/*
* shooting
*	
*	2023.9.11
*/

/* Includes ------------------------------*/
#include "shooting.h"
#include "device.h"
#include "car.h"
#include "math_support.h"
/* Exported variables --------------------*/



shooting_t shooting = 
{
	.frictionLF = &shooting_motor[FRICTION_LF],
	.frictionRF = &shooting_motor[FRICTION_RF],
	.dail       = &shooting_motor[DAIL],
	.limit      = &shooting_motor[LIMIT],
	
	
	.work = Shooting_Work,
	
	.shooting_reset_state = DEV_RESET_NO,
	.load_state = load_NO,
};

/* Function  body --------------------------------------------------------*/

/**
 * @brief 发射PID计算
 * 
 * @param shooting 
 */
void Shooting_Pid_Calculating(shooting_t *shooting)
{
	//拨盘PID计算
	shooting->base_info.output_dail = shooting->dail->c_speed(shooting->dail,     \
																					shooting->base_info.dail_info.target_speed);
	//拨盘PID计算
	shooting->base_info.output_limit = shooting->limit->c_speed(shooting->limit,  \
																					shooting->base_info.limit_info.target_speed);

}

/**
 * @brief 发射机构初始化 
 * 				
 * @param shooting 
 * @retval 装弹完成-> 初始化成功 装弹完成
 */
void Shooting_Work_Init(shooting_t *shooting)
{
	while (Shooting_Work_Reloading(shooting) == 1)//补弹完成
	{
		shooting->shooting_reset_state = DEV_RESET_OK;//初始化成功
		shooting->load_state = load_OK;								//装弹完成
	}
	
}

/**
 * @brief 补弹
 * 
 * @param shooting 
 * @return uint8_t 补弹完成：1 未完成：0
 */
uint8_t Shooting_Work_Reloading(shooting_t *shooting)
{
	int32_t position = shooting->dail->rx_info.angle_sum;//拨盘现在位置
	int16_t speed    = shooting->dail->rx_info.speed;    //拨盘现在速度 
	dail_info_t *info = &shooting->base_info.dail_info;
	uint8_t res = 0;
switch (info->work_state)
{
case DAIL_SLEEP://唤醒拨盘，开始补弹
	info->work_state = DAIL_RELOAD;
	break;
case DAIL_RELOAD://正转补弹
	if (motor_class_stucking_flag(shooting->dail) != 1)//正转没有堵转 
	{
		if (info->work_times >= DAIL_MAX_WORK_TIMES)//工作超时,没有弹->供弹完成，退出
		{
			/* break */
			info->work_times = 0;
			info->target_speed = 0;
			info->f_stucking_cnt = 0;
			info->work_state = DAIL_SLEEP;
			res = 1;
		}
		else//没有堵转且工作没有超时->有弹
		{
			info->target_speed = DAIL_FEED_SPEED ;
			info->work_times++;
		}
	}
	else//正转堵转了
	{
		info->f_stucking_cnt++;
		if (info->f_stucking_cnt < DAIL_MAX_F_STUCK_CNT)//堵转次数未满足->反转
		{
			/*正转下的堵转才会改变反转目标位置*/
			info->target_position = position + DAIL_REVERT_PISITION;//计算反转目标位置
			info->work_state = DAIL_REVERT;//进入反转模式
		}
		else//正向堵转满足次数->供弹成功，退出
		{
			/* break */
			info->work_times = 0;
			info->target_speed = 0;
			info->f_stucking_cnt = 0;
			info->work_state = DAIL_SLEEP;
			res = 1;
		}
	}

	break;
case DAIL_REVERT:
	if (motor_class_stucking_flag(shooting->dail) == 1)//反转堵转了->正转补弹
	{
		info->work_state = DAIL_RELOAD;
	}
	else if(position <= info->target_position)//(反转为负)没堵转但到位了->正转补弹
	{
		info->work_state = DAIL_RELOAD;
	}
	else//没堵转也没到位
	{
		info->target_speed = DAIL_STUCK_SPEED;//保持反转速度
	}
	
	break;
default:
	break;
}

	return res;

}

/**
 * @brief 发射打弹模式
 * 
 * @param shooting 
 * @return uint8_t 未到位：0 到位：1
 */
uint8_t Shooting_Work_Fire(shooting_t *shooting)
{ 
	uint8_t res = 0;
	int32_t position = shooting->limit->rx_info.angle_sum;//限位现在位置
	limit_info_t *info = &shooting->base_info.limit_info;

	switch (info->work_state)
	{
	case LIMIT_SLEEP:
		info->target_position = position + LIMIT_POSIT_VALUE;//设置目标位置
		info->work_state = LIMIT_RUNING;//开始运行
		break;
	case LIMIT_RUNING:
		if(position <= info->target_position)//（方向为负）到位
		{
			/*break*/
			info->target_speed = 0;//目标速度为0
			info->work_state = LIMIT_SLEEP;//进入休眠
			res = 1;//返回1
		}
		else//未到位
		{
			info->target_speed = LIMIT_SPEED;//保持速度
		}
		break;
	default:
		break;
	}
	return res;
}

/**
 * @brief 总控
 * 
 * @param shooting 
 */
void Shooting_Work(shooting_t *shooting)
{
	/*命令响应*/
	if(SHOOTING_FIRE_CMD == true &&						//打弹命令
		shooting->load_state == load_OK && 		//装弹完成
		car.shooting_fire_run_flag == CMD_RUN_OK ||//可以运行
		car.shooting_fire_run_state == CMD_RUNING)//命令正在执行
	{
		if (car.shooting_fire_run_state != CMD_RUNING)//首次进来，切换命令状态
		{
			car.shooting_fire_run_state = CMD_RUNING;
		}
		
		while (Shooting_Work_Fire(shooting) == 1)//命令执行完成
		{
			shooting->load_state = load_NO;//需要装弹
			car.shooting_fire_run_state = CMD_FINISH;//命令执行完成
			car.shooting_fire_run_flag = CMD_RUN_NO;//等待按键复位
		}	
	}
	
//连发命令
	if(SHOOTING_FIRING_CMD == true)
	{
		shooting->base_info.limit_info.target_speed = LIMIT_SPEED;
		Shooting_Work_Reloading(shooting);
		car.shooting_firing_run_state = CMD_RUNING;
	}
	else if(car.shooting_firing_run_state == CMD_RUNING)
	{
		shooting->base_info.limit_info.target_speed = 0;
		car.shooting_firing_run_state = CMD_FINISH;
		shooting->load_state = load_NO;
	}

//常规控制部分
	if(!RC_ONLINE)//关控
	{
		shooting->base_info.output_dail = 0 ;
		shooting->base_info.output_limit = 0 ;
		shooting->shooting_reset_state = DEV_RESET_NO;//需要初始化
	}
	else if(shooting->shooting_reset_state != DEV_RESET_OK)//发射初始化
	{
		Shooting_Work_Init(shooting);
	}
	else if(shooting->load_state == load_NO)//检查是否需要装弹
	{
		while (Shooting_Work_Reloading(shooting) == 1)
		{
			shooting->load_state = load_OK;
		}
	}

	if(RC_ONLINE)//开控
	{
		Shooting_Pid_Calculating(shooting); 
		shooting->dail->base_info.motor_out = shooting->base_info.output_dail;
		shooting->limit->base_info.motor_out = shooting->base_info.output_limit;

	}
}

