#include "indicate.h"
#include "task.h"
#include "hw_config.h"
/*
 */

LocalKeyType Local_Key;
RFRKeyType RFR_Key;

//
bool f_commCAN = FALSE;			/* Init. UART */

//
uint16_t uTimerFlag = 0;
uint32_t uSysStatusFlag = 0;
uint32_t uKeyStatusFlag = 0;
uint16_t uCommStatusFlag = 0;

uint32_t uMsBasecnt = 0;
uint32_t u32_SysRunTime = 0;

uint16_t u16_DebugFlag = 0;
uint8_t u8_TestFlag = 0;

uint8_t u1_rx_data[100];
uint8_t u1_rx_point = 0;
uint8_t u2_rx_data[100];
uint8_t u2_rx_point = 0;
uint8_t u3_rx_data[u3Buf_Size];
uint8_t u3_rx_point = 0;

uint8_t u8_SMTDlmitErCnt = 0;
uint8_t u8_SMTCsumErCnt = 0;

uint8_t u1Data_frame[40];
uint8_t u2Data_frame[40];
uint8_t u3Data_frame[40];
uint8_t TransCnt_U1;
uint8_t TransCnt_U2;
uint8_t TransCnt_U3;


uint16_t ADC10dma_Value[10];	
uint16_t ADC11dma_Value[10];	
uint8_t ADC10_Cnt = 0;
uint16_t ADC_Battery = 0;
uint16_t ADC_RFRVolume = 0;

float V_Battery = 50;
float V_RFRVolume = 0;

uint32_t u32can_TrnCnt = 0;


uint8_t t_200msCnt = 0;
uint8_t t_1sCnt = 0;

uint8_t DMotorSeq = 0;
uint8_t SMotorSeq = 0;
uint8_t PumpSeq = 0;
uint8_t FanSeq = 0;

uint16_t DMotorWaitime = 0;
uint16_t SMotorWaitime = 0;
uint16_t PumpWaittime = 0;
uint16_t FanWaittime = 0;

uint8_t DMotorTKPWM = 0;
uint8_t SMotorTKPWM = 0;
uint8_t FanTKPWM = 0;

uint8_t DRV_PWMDuty = 0;
uint8_t STR_PWMDuty = 0;
uint16_t FAN_PWMDuty = 0;

uint16_t RK_PermitTime = 0;
uint8_t DRV_Speed = 1;
uint8_t smt_DRVSpeed = 1;

uint8_t FAN_Speed = 0;

uint8_t WLInCnt = 0;


uint8_t bcon_Req = 0;
uint8_t bcon_Out = 0;



uint16_t smt_Command = 0;

uint8_t LowBat_Cnt = 0;
uint16_t u2DTInTime = 0;
uint16_t u16_RFMNChkCnt = 0;

uint16_t u16_TestCnt = 0;

uint8_t smt_DbgCommand = 0;
uint8_t can_DbgCommand = 0;

uint8_t WLEmptyCnt1 = 0;		
uint8_t WLEmptyCnt2 = 0;		
uint8_t WLFullCnt1 = 0;			
uint8_t WLFullCnt2 = 0;			

uint8_t u8_pumpspdCnt = 0;
uint8_t u8_AutospdCnt = 0;

uint8_t u2_ErCnt = 0;
uint8_t m8_Buff = 0;


uint32_t CAN_RcvCount = 0;

uint32_t Received_canID = 0;
uint8_t	 Received_canIDType = 0;
uint8_t	 Received_canDLC = 0;

uint8_t u3TransReq_cnt = 0;
uint16_t canDTIntime = 0;	

uint8_t crash_Cnt = 0;		
uint8_t LKAUTOSeq = 0;
uint16_t LKSeqDlyTime = 0;