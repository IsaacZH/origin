/**
 * @file car.c
 * @author Isaac
 * @brief 整车标志位的更新
 * @version 0.1
 * @date 2023-10-25
 * 
 * @copyright Isaac (c) 2023
 * @note 左拨杆下进入键盘模式，遥控器模式下的命令不要与这个冲突。
 * 首次初始化完成会先进入陀螺仪模式
 */

/* Includes ------------------------------------------------------------------*/
#include "car.h"
#include "gimbal.h"

/* Private function prototypes -----------------------------------------------*/
void Car_Commond_Init(car_t *car);
void Commond_Update(car_t *car);
void Car_Mode_Update(car_t *car);
void RC_S1_Status_Check(car_t *car);
void RC_S2_Status_Check(car_t *car);
void Car_Init_Judge(car_t *car);
void RC_Wheel_Status_Check(car_t *car);
void	Car_Ctrl_Mode_Update(car_t *car);
void KEY_Shift_Status_Check(car_t *car);
void KEY_Ctrl_Status_Check(car_t *car);
void KEY_F_Status_Check(car_t *car);
void KEY_E_Status_Check(car_t *car);
void KEY_Q_Status_Check(car_t *car);
void KEY_V_Status_Check(car_t *car);
void KEY_ML_Status_Check(car_t *car);
/* Private variables ---------------------------------------------------------*/
car_t car=
{
	.car_reset_state = DEV_RESET_NO,

};


//命令
bool CAR_L90_CMD   		  =  false;  //左转90命令 
bool GIM_180_CMD   		  =  false;  //云台掉头命令
bool CAR_R90_CMD  		  =  false;  //右转90命令 
bool SHOOTING_FIRE_CMD  =  false;  //发射打弹命令
bool SHOOTING_FIRING_CMD = false;  //发射连发命令

/**
  * @Name    
  * @brief  命令初始化
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Car_Commond_Init(car_t *car)
{
	bool CAR_R90_CMD  =  false;  //右转90命令 
	bool CAR_L90_CMD  =  false;  //左转90命令 
	bool GIM_180_CMD  =  false;  //云台掉头命令
	bool SHOOTING_FIRE_CMD  =  false;  //发射打弹命令
	bool SHOOTING_FIRING_CMD = false;  //发射连发命令
	//可执行初始化
	car->car_l90_run_flag = CMD_RUN_NO ;
	car->car_r90_run_flag = CMD_RUN_NO ;
	car->gim_180_run_flag = CMD_RUN_NO ;
	car->shooting_fire_run_flag = CMD_RUN_NO ; 
	
	car->gim_180_run_state = CMD_FINISH ;
	car->shooting_firing_run_state = CMD_FINISH;
	car->shooting_fire_run_state = CMD_FINISH;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @name     
  * @brief  控制任务
  * @param   
  * @retval 
  * @author  
  * @date
**/
void Car_Ctrl(car_t *car) 
{
	/*控制模式更新*/
	Car_Ctrl_Mode_Update(car);
	
	/* 整车模式更新 */
  Car_Mode_Update(car);
	
	/* 整车命令更新 */
  Commond_Update(car);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @Name    
  * @brief  初始化判断 
  * @param   
  * @retval 
  * @author      
**/
void Car_Init_Judge(car_t *car)
{

	if(gimbal.gimbal_reset_state == DEV_RESET_OK)
	{
		car->car_reset_state = DEV_RESET_OK;
	}
}
/**
  * @Name    
  * @brief  整车命令更新 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Commond_Update(car_t *car)
{
	if(car->car_ctrl_mode == RC_CTRL_MODE)
	{
		RC_Wheel_Status_Check(car);
		RC_S1_Status_Check(car);
	}
	else if (car->car_ctrl_mode == KEY_CTRL_MODE)
	{
		KEY_E_Status_Check(car);
		KEY_Q_Status_Check(car);
		KEY_V_Status_Check(car);
		KEY_ML_Status_Check(car);
	}
}

/**
  * @Name    
  * @brief  整车模式更新 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Car_Mode_Update(car_t *car)
{
	Car_Init_Judge(car);
	
	if(!RC_ONLINE)
	{
		car->car_move_mode = offline_CAR;
		car->car_reset_state = DEV_RESET_NO;
		Car_Commond_Init(car);
	}
	else if(car->car_reset_state == DEV_RESET_NO)
	{
		car->car_move_mode = init_CAR;
	}
	else if (car->car_move_mode == init_CAR)
	{
		car->car_move_mode = gyro_CAR;//进入默认模式（陀螺仪模式）
	}
	else 
	{
		if (car->car_ctrl_mode == RC_CTRL_MODE)//遥控器模式
		{
			RC_S2_Status_Check(car);//右拨杆控制移动模式
		}
		else//键盘模式 
		{
			KEY_Shift_Status_Check(car);//退出，进入陀螺仪模式
			KEY_Ctrl_Status_Check(car);//机械模式切换
			KEY_F_Status_Check(car);//小陀螺模式切换
		}
		
	}
}

/**
 * @brief 控制模式更新
 * 
 * @param car 
 */
void	Car_Ctrl_Mode_Update(car_t *car)
{
	//左拨杆下：键盘模式  其余都是遥控器模式
	switch(rc.base_info->s1.value)
	{
	case 0x02:	//左拨杆下
    car->car_ctrl_mode = KEY_CTRL_MODE;
		break;
	default://左拨杆不是下
		car->car_ctrl_mode = RC_CTRL_MODE;
		break;
	}

//从遥控器模式转到键盘模式，进入陀螺仪模式
	switch (rc.base_info->s1.status)
	{
	case down_R://左拨杆向下拨
		car->car_move_mode = gyro_CAR;
		break;
	
	default:
		break;
	}
}


/**
  * @Name    
  * @brief  遥控器右拨杆状态检查
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_S2_Status_Check(car_t *car)
{
	switch(rc.base_info->s2.value)
	{
	case 0x01:  //右拨杆上
		car->car_move_mode  =   mec_CAR ;
		break;
	case 0x03:	//右拨杆中
		car->car_move_mode  =   gyro_CAR;
		break;
	case 0x02:	//右拨杆下
		car->car_move_mode  =   cycle_CAR;
		break;
	default:
		break;		
	}
}
/**
  * @Name    
  * @brief  遥控器拨轮状态检查
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_Wheel_Status_Check(car_t *car)
{
	switch(rc.base_info->thumbwheel.status)
	{
	case down_R:  //拨轮上
		CAR_R90_CMD   =  true;
		break;
	case up_R:	//拨轮下
		CAR_L90_CMD   =  true;
		break;
	case keep_R:
		CAR_R90_CMD   =  false;
		CAR_L90_CMD   =  false;
		//拨轮按键复位，左右转90命令可执行
		car->car_r90_run_flag = CMD_RUN_OK;  
		car->car_l90_run_flag = CMD_RUN_OK;  		
	default:
		break;		
	}
}
/**
  * @Name    
  * @brief  遥控器左拨杆状态检查 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_S1_Status_Check(car_t *car)
{

	switch(rc.base_info->s1.value)
	{
	case 0x01:  //左拨杆上
		SHOOTING_FIRE_CMD = true;
		break;  
	case 0x03:	//左拨杆中
		SHOOTING_FIRE_CMD = false;
	//拨杆位置归中，发射可执行
		car->shooting_fire_run_flag = CMD_RUN_OK; 
		break;
	case 0x02:	//左拨杆下
		SHOOTING_FIRE_CMD = false;
		break;
	default:
		break;
	}

}

/*-----------------------------按键部分------------------------------------*/

/**
 * @brief shift按键状态检查
 * 
 * @param car 
 */
void KEY_Shift_Status_Check(car_t *car)
{
	switch (rc.base_info->Shift.status)
	{
	case relax_K://放松
		break;
	case down_K://按下
		car->car_move_mode = gyro_CAR;
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief ctrl按键状态检查
 * 
 * @param car 
 */
void KEY_Ctrl_Status_Check(car_t *car)
{
	switch (rc.base_info->Ctrl.status)
	{
	case relax_K://放松
		break;
	case down_K://按下
		if (car->car_move_mode == mec_CAR)//现在是机械模式，退回陀螺仪模式
		{
			car->car_move_mode = gyro_CAR;
		}
		else//现在不是机械模式，进入机械模式
		{
			car->car_move_mode = mec_CAR;
		}
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief F按键状态检查
 * 
 * @param car 
 */
void KEY_F_Status_Check(car_t *car)
{
	switch (rc.base_info->F.status)
	{
	case relax_K://放松
		break;
	case down_K://按下
		if (car->car_move_mode == cycle_CAR)//现在是小陀螺模式，切换回陀螺仪模式
		{
			car->car_move_mode = gyro_CAR;
		}
		else//现在不是小陀螺，进入小陀螺
		{
			car->car_move_mode = cycle_CAR;
		}
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief E按键状态检查 CAR_R90_CMD
 * 
 * @param car 
 */
void KEY_E_Status_Check(car_t *car)
{
	switch(rc.base_info->E.status)
	{
	case relax_K://放松
		CAR_R90_CMD   =  false;
		car->car_r90_run_flag = CMD_RUN_OK;
		break;
	case down_K://按下
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		CAR_R90_CMD   =  true;
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief Q按键状态检查 CAR_L90_CMD
 * 
 * @param car 
 */
void KEY_Q_Status_Check(car_t *car)
{
	switch(rc.base_info->Q.status)
	{
	case relax_K://放松
		CAR_L90_CMD   =  false;
		car->car_l90_run_flag = CMD_RUN_OK;
		break;
	case down_K://按下
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		CAR_L90_CMD   =  true;
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief V按键状态检查 GIM_180_CMD
 * 
 * @param car 
 * @note 在机械模式下按换头会自动进入陀螺仪模式
 */
void KEY_V_Status_Check(car_t *car)
{
	switch(rc.base_info->V.status)
	{
	case relax_K://放松
		GIM_180_CMD   =  false;
		car->gim_180_run_flag = CMD_RUN_OK;
		break;
	case down_K://按下
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		GIM_180_CMD   =  true;
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		break;
	case long_press_K://长按
		break;
	default:
		break;
	}
}

/**
 * @brief 鼠标左键状态检查 SHOOTING_FIRE_CMD SHOOTING_FIRING_CMD
 * 
 * @param car 
 */
void KEY_ML_Status_Check(car_t *car)
{
	switch(rc.base_info->mouse_btn_l.status)
	{
	case relax_K://放松
		SHOOTING_FIRE_CMD = false;
		SHOOTING_FIRING_CMD = false;
		car->shooting_fire_run_flag = CMD_RUN_OK;
		break;
	case down_K://按下
		break;
	case up_K://抬起
		break;
	case short_press_K://短按
		SHOOTING_FIRE_CMD = true;
		break;
	case long_press_K://长按
		SHOOTING_FIRING_CMD = true;
		break;
	default:
		break;
	}
}