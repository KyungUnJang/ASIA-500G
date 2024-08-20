
#ifndef _KEYSCAN_H_
#define _KEYSCAN_H_


#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include "define.h"


// #include "stm32f1xx_hal_gpio.h"
extern void GPIO_Configuration(void); 
extern void User_Interface();
extern uint8_t isAutoCondition();
extern void Drive_REQ(uint8_t Control);
extern void Comm_Action();
extern uint8_t not_ready_flag;
#endif
