#ifndef __JUDGE_PROTOCOL_H
#define __JUDGE_PROTOCOL_H

#include "config.h"
#include "main.h"
#include "stdbool.h"
#define JUDGE_FRAME_HEADER	0xA5  //此为帧头的SOF,帧头分为 SOF,length,Seq,CRC8
//***********裁判系统的分类信息***************/
enum
{ 
	ID_game_state       						= 0x0001,//比赛状态数据，1Hz
	ID_game_result 	   							= 0x0002,//比赛结果数据，比赛结束发送
	ID_game_robot_HP       					= 0x0003,//比赛机器人血量数据，1Hz发送
//	ID_dart_status									= 0x0004,//飞镖发射状态，飞镖发射时发送
	ID_ICRA_buff_debuff_zone_status = 0x0005,//人工智能挑战赛加成与惩罚区状态，1Hz
	ID_event_data  									= 0x0101,//场地事件数据，事件 ——改变后—— 发送
	ID_supply_projectile_action   	= 0x0102,//场地补给站动作标识数据，动作 ——改变后—— 发送
//	ID_supply_projectile_booking    = 0x0103,//请求补给站补弹数据 ——参赛队发送——（RM 对抗赛尚未开）10Hz
	ID_referee_warning					 		= 0x0104,//裁判警告数据，——警告后——发送
	ID_dart_remaining_time					= 0x0105,//飞镖发射口倒计时，1Hz
	ID_game_robot_state    					= 0x0201,//机器人状态数据，10Hz
	ID_power_heat_data    					= 0x0202,//实时功率热量数据，50Hz
	ID_game_robot_pos        				= 0x0203,//机器人位置数据，10Hz
	ID_buff_musk										= 0x0204,//机器人增益数据，1Hz
	ID_aerial_robot_energy					= 0x0205,//空中机器人能量状态数据，10Hz，只有——空中机器人主控——发送
	ID_robot_hurt										= 0x0206,//伤害状态数据，伤害发生后发送
 	ID_shoot_data										= 0x0207,//实时射击数据，子弹发射后发送
	ID_bullet_remaining							= 0x0208,//弹丸剩余发送数，仅——空中机器人，哨兵机器人——以及ICRA机器人发送，1Hz
	ID_rfid_status									= 0x0209,//机器人RFID状态，1Hz
	
	ID_dart_client_directive        = 0x020A,//飞镖机器人客户端指令书, 10Hz
	
	ID_robot_interactive_header_data			= 0x0301,//机器人交互数据，——发送方触发——发送 10Hz
	ID_controller_interactive_header_data = 0x0302,//自定义控制器交互数据接口，通过——客户端触发——发送 30Hz
	ID_map_interactive_header_data        = 0x0303,//客户端小地图交互数据，——触发发送——
	ID_keyboard_information               = 0x0304,//键盘、鼠标信息，通过——图传串口——发送
  ID_smallmap                           = 0x0305,//客户端小地图接收信息 
};
//命令码枚举CMD_ID
//**************裁判系统各个信息的长度（单位/字节）**************************/
enum judge_data_length_t {
	/* Std */
	LEN_FRAME_HEAD 	                 = 5,	// 帧头长度
	LEN_CMD_ID 		                   = 2,	// 命令码长度
	LEN_FRAME_TAIL 	                 = 2,	// 帧尾CRC16
	/* Ext */
	// 0x000x
	LEN_GAME_STATUS 				         = 11,
	LEN_GAME_RESULT 				         = 1,
	LEN_GAME_ROBOT_HP 			         = 32,
//	LEN_DART_STATUS					         = 3,
	LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS = 11,//0x0005
	// 0x010x
	LEN_EVENT_DATA					         = 4,
	LEN_SUPPLY_PROJECTILE_ACTION	   = 4,//！！！！！！！！！！！！！！！！！
//	LEN_SUPPLY_PROJECTILE_BOOKING	   = 2,//对抗赛未开启
	LEN_REFEREE_WARNING				       = 2,
	LEN_DART_REMAINING_TIME		     	 = 1,//0x0105
	// 0x020x
	LEN_GAME_ROBOT_STATUS			       = 27,//15!!!!!!!!!!!!!!!!!!!!!!!!!!!
	LEN_POWER_HEAT_DATA 			       = 16,//！！！！！！！！！！
	LEN_GAME_ROBOT_POS				       = 16,
	LEN_BUFF_MASK		 				         = 1,
	LEN_AERIAL_ROBOT_ENERGY 	     	 = 1,//！！！！！
	LEN_ROBOT_HURT				         	 = 1,
	LEN_SHOOT_DATA					         = 7,//！！！！
	LEN_BULLET_REMAINING	 		       = 6,//！！！！
	LEN_RFID_STATUS					         = 4,
	LEN_DART_CLIENT_DIRECTIVE        = 6,//0x020A

	// 0x030x
	//LEN_robot_interactive_header_data      = n,
	//LEN_controller_interactive_header_data = n,
	LEN_MAP_INTERACTIVE_HEADERDATA           = 15,
	LEN_KEYBOARD_INFORMATION                 = 12,//0x0304

		LEN_SENTRY_INFO                          = 1,
};//表2-4
/*******************裁判系统信息内容排序**************************/



typedef struct __packed
{
	uint8_t  sof;
	uint16_t data_length;
	uint8_t  seq;
	uint8_t  crc8;
} std_frame_header_t;//LEN_FRAME_HEAD


typedef struct __packed
{
 uint8_t game_type : 4;
 uint8_t game_progress : 4;
 uint16_t stage_remain_time;
uint64_t SyncTimeStamp;
} ext_game_status_t;

//LEN_CMD_ID
//LEN_FRAME_TAIL
/* ID: 0x0005 	Byte:	11	人工智能挑战赛加成与惩罚区状态 */
typedef struct __packed
{
	uint8_t F1_zone_status:1;            //激活状态
	uint8_t F1_zone_buff_debuff_status:3;//红方回血区
	uint8_t F2_zone_status:1;
	uint8_t F2_zone_buff_debuff_status:3;//红方弹药补给区
	uint8_t F3_zone_status:1;
	uint8_t F3_zone_buff_debuff_status:3;//蓝方回血区
	uint8_t F4_zone_status:1;
	uint8_t F4_zone_buff_debuff_status:3;//蓝方弹药补给区
	uint8_t F5_zone_status:1;
	uint8_t F5_zone_buff_debuff_status:3;//禁止射击区
	uint8_t F6_zone_status:1;
	uint8_t F6_zone_buff_debuff_status:3;//禁止移动区
	uint16_t red1_bullet_left;           //红方 1 号剩余弹量
	uint16_t red2_bullet_left;           //红方 2 号剩余弹量
	uint16_t blue1_bullet_left;          //蓝方 1 号剩余弹量
	uint16_t blue2_bullet_left;          //蓝方 2 号剩余弹量
} ext_ICRA_buff_debuff_zone_status_t;//LEN_ICRA_BUFF_DEBUFF_ZONE_STATUS  表3-5
/* ID: 0x0102  Byte:  4    补给站动作标识数据 */
typedef struct __packed 
{ 
	uint8_t supply_projectile_id;  //补给站口 ID
	uint8_t supply_robot_id;       //补弹机器人 ID
	uint8_t supply_projectile_step;//出弹口开闭状态
	uint8_t supply_projectile_num; //补弹数量
} ext_supply_projectile_action_t;//LEN_SUPPLY_PROJECTILE_ACTION  表3-7
/* ID: 0X0201  Byte: 27    机器人状态数据 */
typedef struct __packed 
{ 
	uint8_t robot_id;                       //本机器人 ID
	uint8_t robot_level;                    //机器人等级
	uint16_t remain_HP;                     //机器人剩余血量
	uint16_t max_HP;                        //机器人上限血量
	uint16_t shooter_id1_17mm_cooling_rate; //机器人 1 号 17mm 枪口每秒冷却值
	uint16_t shooter_id1_17mm_cooling_limit;//机器人 1 号 17mm 枪口热量上限
	uint16_t shooter_id1_17mm_speed_limit;  //机器人 1 号 17mm 枪口上限速度 单位 m/s
	uint16_t shooter_id2_17mm_cooling_rate; //机器人 2 号 17mm 枪口每秒冷却值
	uint16_t shooter_id2_17mm_cooling_limit;//机器人 2 号 17mm 枪口热量上限
	uint16_t shooter_id2_17mm_speed_limit;  //机器人 2 号 17mm 枪口上限速度 单位 m/s
	uint16_t shooter_id1_42mm_cooling_rate; //机器人 42mm 枪口每秒冷却值
	uint16_t shooter_id1_42mm_cooling_limit;//机器人 42mm 枪口热量上限
	uint16_t shooter_id1_42mm_speed_limit;  //机器人 42mm 枪口上限速度 单位 m/s
	uint16_t chassis_power_limit;           //机器人底盘功率限制上限
	//主控电源输出情况
	uint8_t mains_power_gimbal_output  : 1; //gimbal 口输出： 1 为有 24V 输出，0 为无 24v 输出
	uint8_t mains_power_chassis_output : 1; //chassis 口输出：1 为有 24V 输出，0 为无 24v 输出
	uint8_t mains_power_shooter_output : 1; //shooter 口输出：1 为有 24V 输出，0 为无 24v 输出
} ext_game_robot_status_t; //LEN_GAME_ROBOT_STATUS  表3-10
/* ID: 0X0202  Byte: 16    实时功率热量数据 */
typedef struct __packed 
{ 
	uint16_t chassis_volt;   				// 底盘输出电压，单位：mV
	uint16_t chassis_current;				// 底盘输出电流，单位：mA
	float chassis_power;   					// 底盘瞬时功率，单位：W
	uint16_t chassis_power_buffer;	// 底盘功率缓冲，单位：60J焦耳缓冲能量(飞坡根据规则增加至250J)
	uint16_t shooter_id1_17mm_cooling_heat; //1 号 17mm 枪口热量
	uint16_t shooter_id2_17mm_cooling_heat; //2 号 17mm 枪口热量
	uint16_t shooter_id1_42mm_cooling_heat;	//42mm 枪口热量
} ext_power_heat_data_t; //LEN_POWER_HEAT_DATA   表3-11
/* ID: 0x0205  Byte:  1    空中机器人能量状态数据 */
typedef struct __packed 
{ 
	uint8_t attack_time;//可攻击时间 单位 s。30s 递减至 0
} ext_aerial_robot_energy_t; //LEN_AERIAL_ROBOT_ENERGY  表3-14
/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef struct __packed 
{ 
	uint8_t armor_id : 4; 	// 装甲伤害时代表装甲ID
	uint8_t hurt_type : 4; 	// 0x0装甲伤害 0x1模块掉线 0x2超射速 0x3超热量 0x4超功率 0x5撞击
} ext_robot_hurt_t; //LEN_ROBOT_HURT  表3-15
/* ID: 0x0207  Byte:  7    实时射击数据 */
typedef struct __packed 
{ 
	uint8_t bullet_type; 	// 子弹类型(1-17mm, 2-42mm)
	uint8_t shooter_id;   // 发射机构 ID（1：1 号 17mm 发射机构，2：2 号 17mm 发射机构，3：42mm 发射机构）
	uint8_t bullet_freq;  // 子弹射频(Hz)
	float bullet_speed;		// 子弹射速(m/s)
} ext_shoot_data_t; //LEN_SHOOT_DATA  表3-16
/* ID: 0x0208  Byte:  6    子弹剩余发射数数据 */
typedef struct __packed
{
	uint16_t bullet_remaining_num_17mm;//17mm 子弹剩余发射数目
	uint16_t bullet_remaining_num_42mm;//42mm 子弹剩余发射数目
	uint16_t coin_remaining_num;       //剩余金币数量
} ext_bullet_remaining_t; //LEN_BULLET_REMAINING   表3-17
/* ID: 0x0209  Byte:  4 	机器人RFID状态 */
typedef struct __packed
{
	uint32_t rfid_status;
} ext_rfid_status_t;//LEN_RFID_STATUS  表3-18
/*ID：0x020A   Byte：6  飞镖机器人客户端指令数据*/
	/*******************************************************************************/
/*
	机器人 ID：
	1，英雄(红)；
	2，工程(红)；
	3/4/5，步兵(红)；
	6，空中(红)；
	7，哨兵(红)；
	9，雷达（红）
	101，英雄(蓝)；
	102，工程(蓝)；
	103/104/105，步兵(蓝)；
	106，空中(蓝)；
	107，哨兵(蓝)；
	109，雷达（蓝）

	客户端 ID： 
	0x0101 为英雄操作手客户端(红) ；
	0x0102 为工程操作手客户端( 红 )；
	0x0103/0x0104/0x0105 为步兵操作手客户端(红)；
	0x0106 为空中操作手客户端((红)； 

	0x0165，英雄操作手客户端(蓝)；
	0x0166，工程操作手客户端(蓝)；
	0x0167/0x0168/0x0169，步兵操作手客户端(蓝)；
	0x016A，空中操作手客户端(蓝)。 
*/
enum judge_robot_ID{
	hero_red       = 1,
	engineer_red   = 2,
	infantry3_red  = 3,
	infantry4_red  = 4,
	infantry5_red  = 5,
	plane_red      = 6,
	
	hero_blue      = 101,
	engineer_blue  = 102,
	infantry3_blue = 103,
	infantry4_blue = 104,
	infantry5_blue = 105,
	plane_blue     = 106,
};
typedef struct{
	uint16_t teammate_hero;
	uint16_t teammate_engineer;
	uint16_t teammate_infantry3;
	uint16_t teammate_infantry4;
	uint16_t teammate_infantry5;
	uint16_t teammate_plane;
	uint16_t teammate_sentry;
	
	uint16_t client_hero;
	uint16_t client_engineer;
	uint16_t client_infantry3;
	uint16_t client_infantry4;
	uint16_t client_infantry5;
	uint16_t client_plane;
} ext_interact_id_t;

typedef struct
{
	float x_speed;
	float y_speed;
	float z_speed;
	float pitch_speed;
	float yaw_speed;
	float roll_speed;
	
	uint8_t key5;
	uint8_t key4;
	uint8_t key1;
	uint8_t key3;
	uint8_t key2;
	
}custom_controler_info_t;

typedef struct {
	std_frame_header_t							fream_header;				// 帧头信息

	ext_game_status_t								ext_game_status;	//0x001
	ext_supply_projectile_action_t	supply_projectile_action;		// 0x0102
	
	ext_game_robot_status_t					game_robot_status;	// 0x0201
	ext_power_heat_data_t						power_heat_data;		// 0x0202
	ext_aerial_robot_energy_t				aerial_robot_energy;// 0x0205
	ext_robot_hurt_t								robot_hurt;					// 0x0206
	ext_shoot_data_t								shoot_data;					// 0x0207
	ext_bullet_remaining_t					bullet_remaining;		// 0x0208	
	ext_rfid_status_t								rfid_status;				// 0x0209	
	
	custom_controler_info_t					custom_info;				//0x0302
	
	ext_interact_id_t								ids;								//与本机交互的机器人id
	
	uint16_t                        self_client;        //本机客户端
	
	bool	 		data_valid;	// 数据有效性

	Dev_Work_State_e power_heat_data_state;
	
	uint8_t		power_heat_data_offline_cnt;
	uint8_t		power_heat_data_offline_max_cnt;
	
} judge_t;

extern judge_t judge;
void check_judge_offline(judge_t *judge_sen);
void judge_update(judge_t *judge_sen,uint8_t *rxBuf);
//void judge_send(void);


#endif
