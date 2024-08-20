#include "hw_def.h"
#include "hw_config.h"

extern void CAN_DT_Decode();
extern void CAN_ER_Decode();
extern void Comm_Check();
extern void can1_Trans8b(uint8_t Command);

extern uint32_t RTK_check_dly;
extern uint8_t RTK_check_flag;