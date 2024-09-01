/**
  ******************************************************************************
  * @file           : RP_drv
  * @brief          : 
  * @note           : start 2022-12-12 
  ******************************************************************************
  */

#include "config_can.h"
#include "control.h"
#include "config.h"
#include "device.h"

/**
 * @brief can2收
 * 
 * @param canId CAN ID
 * @param rxBuf 数据帧
 */
void CAN2_RX(uint32_t canId, uint8_t *rxBuf)
{
	test_motor.rx(&test_motor,rxBuf,8);
	test_motor.id.rx_id = canId;
	test_motor.id.drive_type = M_CAN2;
	// control.target_speed = 824;
}

/**
  * @Name    CAN1_RX
  * @brief   can1收
  * @param   canId：canid  
	* @param   rxBuf：数据帧
  * @retval 
  * @author  
  * @Date    
**/
void CAN1_RX(uint32_t canId, uint8_t *rxBuf)
{
	test_motor.rx(&test_motor,rxBuf,8);
	test_motor.id.rx_id = canId;
	test_motor.id.drive_type = M_CAN1; 
	// control.target_speed = -824;

}




