#ifndef __BSP_INDICATE_H
#define __BSP_INDICATE_H

#include "define.h"
extern void dbg_led_toggle();
extern void buzz_ctrl(uint8_t ctrl);
extern void Lamp_green_ctrl(uint8_t ctrl);
extern void Lamp_blue_ctrl(uint8_t ctrl);
extern void Lamp_red_ctrl(uint8_t ctrl);
extern void DBG_1_ctrl();
extern void DBG_2_ctrl();
extern void DBG_3_ctrl();
extern void DBG_4_ctrl();
extern void Water_PUMP_ctrl(uint8_t type, uint8_t ctrl);
extern void sol_ctrl(uint8_t ctrl);
extern void M1_DRV_ctrl(uint8_t ctrl);
extern void M1_DRV_Break_ctrl(uint8_t ctrl);
extern void M2_STR_ctrl(uint8_t ctrl);
extern void M2_STR_Break_ctrl(uint8_t ctrl);
#endif 
