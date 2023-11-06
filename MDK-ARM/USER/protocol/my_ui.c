#include "my_ui.h"
#include "my_judge.h"
#include "string.h"
#include "stdbool.h"
#include "arm_math.h"
#include "stdio.h"
#include "judge_protocol.h"
#include "vision_protocol.h"
#include "device.h"


#define send_frequency 10  //发送频率

/**
  * @brief  实时任务
  * @param  更新周期（ms）
  * @retval 
  */
void tick_task(uint16_t time)
{
	uint16_t cnt_max = 1000 / time / send_frequency;
	uint8_t task_num_max = 1;
	static uint16_t cnt = 0;
	static uint8_t task_num = 0;
	client_info_update();
	
	if(cnt >= cnt_max)
	{
		switch(task_num)
		{
			case 0:
				UI_send_char_1();
				break;
			default:
				break;
		}
		task_num++;
		task_num %= task_num_max;
		cnt = 0;
	}
	else 
	{
		cnt++;
	}
}

graphic_data_struct_t vision_yaw_buff;
ext_client_custom_character_t vision_yaw_character;

char vision_yaw_info[30] = {"yaw:"};


/*********************char:vision_yaw****************************/
void UI_send_char_1(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  if(cnt == 0)//add num
  {
    operate_tpye = ADD;  
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	vision_yaw_buff = draw_char("gc0",  
	                        operate_tpye,  
                          1,  
                          CYAN_BLUE,  
                          18,  
                          strlen(vision_yaw_info),  
                          2,  
                          Client_mid_position_x - 750,  
                          Client_mid_position_y + 125);  
	
	vision_yaw_character.grapic_data_struct = vision_yaw_buff;
	memcpy(&vision_yaw_character.data,&vision_yaw_info,sizeof(vision_yaw_info));
		
	client_send_char(vision_yaw_character);

	
	cnt++;
  cnt %= 2;
}










