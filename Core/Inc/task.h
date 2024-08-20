#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "define.h"

extern init_setting load_value;
extern uint8_t center_start;
extern uint8_t CAN_RxData[4];
extern uint8_t CAN_RxData_Error[8];
extern char info[9];
typedef struct 
{
    uint32_t StdId; 
    uint32_t ExtId; 
    uint8_t IDE;    
    uint8_t RTR;    
    uint8_t DLC;    
    uint8_t Data[8];
}CanTxMsg;


typedef struct 
{
    uint32_t StdId; 
    uint32_t ExtId; 
    uint8_t IDE;    
    uint8_t RTR;    
    uint8_t DLC;    
    uint8_t Data[8];
    uint8_t FMI;
}CanRxMsg;

extern uint8_t RTK_state;
extern uint8_t Line_auto_condition;
extern uint32_t ms_cnt;



extern uint32_t Bcon_dly;

extern void can1_Trans8b(uint8_t Command);
extern void INIT_Check();
extern void Timer_Event();

extern void Event_10ms();
extern void Event_30ms();
extern void Event_200ms();
extern void Event_500ms();	


extern void Var_Init();
extern void bsp_can_init();
extern void Bcon_Control();
extern void Port_ReadACT();
extern uint8_t NotEmpty_WL();

extern void init_set_read();
extern void init_set_save();
extern void init_set();

void R_LED_On(void);
void R_LED_Off(void);
void R_LED_Toggle(void);
void G_LED_On(void);
void G_LED_Off(void);
void G_LED_Toggle(void);
void B_LED_On(void);
void B_LED_Off(void);
void B_LED_Toggle(void);
extern void bcon_On(uint8_t bcon_no);

extern void bcon_Off(uint8_t bcon_no);

#endif
