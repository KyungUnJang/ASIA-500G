#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "define.h"

extern uint8_t DRV_SPDTBNo;
extern uint8_t SPD_offset;
extern uint8_t is_DriveKey();
extern void DM_Stop();
extern void DRV_Control();
extern uint8_t is_StrringKey();
extern void SM_Stop();
extern void STR_Control();
extern void PUMP_Control();
extern void FAN_Control();
extern void pn_PumpOn();
extern void all_MotorStop();
extern void str_MotorLimitStop();
extern void Init_DrvPortSet();
extern void Motor_Control();
extern uint8_t Auto_run_state;
#if 0
#define DRV_CHECK_SQC 0x00
#define DRV_STOP_SQC  0x01
#define DRV_WAIT_SQC  0x02
#define DRV_GO_SQC    0x04
#define DRV_BACK_SQC  0x05



#define STR_CHECK_SQC 0x00
#define STR_STOP_SQC  0x01
#define STR_WAIT_SQC  0x02
#define STR_LEFT_SQC    0x04
#define STR_RIGHT_SQC  0x05



//#define PUMP_NULL		0
#define PUMP_START		0	/* PUMP ON Low */
#define PUMP_SRTH		2	/* PUMP ON High */
#define PUMP_UP1L		3
#define PUMP_UP1H		4
#define PUMP_UP2L		5
#define PUMP_UP2H		6
#define PUMP_UP3L		7
#define PUMP_UP3H		8
#define PUMP_UP4L		9
#define PUMP_UP4H		10
#define PUMP_UP5L		11
#define PUMP_UP5H		12
//#define PUMP_Stay		13
#define PUMP_STOP		13
#define PUMP_STOP1H		14
#define PUMP_Stay		15


extern uint8_t DRV_DAC_SET;
extern uint8_t FAN_dly;
extern uint16_t DRV_dly;
extern uint16_t STR_dly;
extern uint16_t Pump_dly;
extern uint8_t Pump_power_lv_SET;
extern uint8_t EMG_dly;
extern uint8_t  ALL_STOP_SQC;

extern void Motor_Control();
extern uint8_t Auto_run_state;
#endif 



#endif 