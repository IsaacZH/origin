/**
 * @file        judge_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       judge_protocol
 */

#include "judge_protocol.h"
#include "vision_protocol.h"
#include "drv_uart.h"
#include "drv_can.h"
#include "string.h"
#include "usart.h"
#include "crc.h"
#include "can.h"

void Determine_ID(void)//判断自己是哪个队伍
{
	if(judge.game_robot_status.robot_id < 10)//本机器人的ID，红方
	{ 
		judge.ids.teammate_hero 		 	= 1;
		judge.ids.teammate_engineer  = 2;
		judge.ids.teammate_infantry3 = 3;
		judge.ids.teammate_infantry4 = 4;
		judge.ids.teammate_infantry5 = 5;
		judge.ids.teammate_plane		 	= 6;
		judge.ids.teammate_sentry		= 7;
		
		judge.ids.client_hero 		 	= 0x0101;
		judge.ids.client_engineer  = 0x0102;
		judge.ids.client_infantry3 = 0x0103;
		judge.ids.client_infantry4 = 0x0104;
		judge.ids.client_infantry5 = 0x0105;
		judge.ids.client_plane			= 0x0106;
		
		if     (judge.game_robot_status.robot_id == hero_red)//不断刷新放置在比赛中更改颜色
			judge.self_client = judge.ids.client_hero;
		else if(judge.game_robot_status.robot_id == engineer_red)
			judge.self_client = judge.ids.client_engineer;
		else if(judge.game_robot_status.robot_id == infantry3_red)
			judge.self_client = judge.ids.client_infantry3;
		else if(judge.game_robot_status.robot_id == infantry4_red)
			judge.self_client = judge.ids.client_infantry4;
		else if(judge.game_robot_status.robot_id == infantry5_red)
			judge.self_client = judge.ids.client_infantry5;
		else if(judge.game_robot_status.robot_id == plane_red)
			judge.self_client = judge.ids.client_plane;
		
	}
	else //蓝方
	{
		judge.ids.teammate_hero 		 	= 101;
		judge.ids.teammate_engineer  = 102;
		judge.ids.teammate_infantry3 = 103;
		judge.ids.teammate_infantry4 = 104;
		judge.ids.teammate_infantry5 = 105;
		judge.ids.teammate_plane		 	= 106;
		judge.ids.teammate_sentry		= 107;
		
		judge.ids.client_hero 		 	= 0x0165;
		judge.ids.client_engineer  = 0x0166;
		judge.ids.client_infantry3 = 0x0167;
		judge.ids.client_infantry4 = 0x0168;
		judge.ids.client_infantry5 = 0x0169;
		judge.ids.client_plane			= 0x016A;
		
		if     (judge.game_robot_status.robot_id == hero_blue)
			judge.self_client = judge.ids.client_hero;
		else if(judge.game_robot_status.robot_id == engineer_blue)
			judge.self_client = judge.ids.client_engineer;
		else if(judge.game_robot_status.robot_id == infantry3_blue)
			judge.self_client = judge.ids.client_infantry3;
		else if(judge.game_robot_status.robot_id == infantry4_blue)
			judge.self_client = judge.ids.client_infantry4;
		else if(judge.game_robot_status.robot_id == infantry5_blue)
			judge.self_client = judge.ids.client_infantry5;
		else if(judge.game_robot_status.robot_id == plane_blue)
			judge.self_client = judge.ids.client_plane;
		
	}

}

judge_t judge = {
	.power_heat_data_offline_max_cnt = 30,
};
uint16_t frame_length;
void judge_update(judge_t *judge_sen,uint8_t *rxBuf)
{
	if(Verify_CRC8_Check_Sum(rxBuf, LEN_FRAME_HEAD) == true)
	{
		memcpy(&judge_sen->fream_header, rxBuf, LEN_FRAME_HEAD);//5个字节
		frame_length = LEN_FRAME_HEAD + LEN_CMD_ID + judge_sen->fream_header.data_length + LEN_FRAME_TAIL;
		if(judge_sen->fream_header.sof == JUDGE_FRAME_HEADER)
		{
			if(Verify_CRC16_Check_Sum(rxBuf, frame_length) == true) 
			{
				uint32_t cmd_id = rxBuf[5] | (rxBuf[6]<<8);
				judge_sen->power_heat_data_offline_cnt = 0;		
				switch(cmd_id)
				{
					case ID_game_state:					
						memcpy(&judge_sen->ext_game_status,rxBuf+7, judge_sen->fream_header.data_length);					
					break;
					case ID_game_robot_state:
						//judge.game_robot_status.robot_id机器人id
						memcpy(&judge_sen->game_robot_status,rxBuf+7, judge_sen->fream_header.data_length);		
						Determine_ID();					
					break;
					case ID_controller_interactive_header_data:
						memcpy(&judge_sen->custom_info,rxBuf+7, judge_sen->fream_header.data_length);
					break;
				}
			}
		}
		if(rxBuf[frame_length] == JUDGE_FRAME_HEADER)
			judge_update(judge_sen,&rxBuf[frame_length]);
	}
}

//检查裁判系统失联
void check_judge_offline(judge_t *judge_sen)
{
	judge_sen->power_heat_data_offline_cnt++;
	if(judge_sen->power_heat_data_offline_cnt > judge_sen->power_heat_data_offline_max_cnt)
	{
		judge_sen->power_heat_data_offline_cnt = judge_sen->power_heat_data_offline_max_cnt;
		judge_sen->power_heat_data_state = DEV_OFFLINE;
	}
	else if(judge_sen->power_heat_data_state == DEV_OFFLINE)
		judge_sen->power_heat_data_state = DEV_ONLINE;
	
}	

