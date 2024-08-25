
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* includes */
#include "main.h"
#include "stdio.h"
#include "math.h"
//
#include "define.h"		/* Define */


/* global variables */
extern bool f_commCAN;			/* Init. UART */



extern float V_Battery;
extern float V_RFRVolume;

extern uint32_t uSysStatusFlag;	/* f_CHGConnect */
extern uint32_t uKeyStatusFlag;
extern uint32_t uMsBasecnt;
extern uint32_t u32_SysRunTime;
extern uint32_t u32can_TrnCnt;
extern uint32_t CAN_RcvCount;
extern uint32_t Received_canID;

extern uint16_t uTimerFlag;
extern uint16_t	uCommStatusFlag; 
extern uint16_t u16_DebugFlag;
extern uint16_t ADC10dma_Value[10];
extern uint16_t ADC11dma_Value[10];
extern uint16_t ADC_Battery;
extern uint16_t ADC_RFRVolume;
extern uint16_t DMotorWaitime;
extern uint16_t SMotorWaitime;
extern uint16_t PumpWaittime;
extern uint16_t FanWaittime;
extern uint16_t smt_Command;
extern uint16_t u2DTInTime;
extern uint16_t u16_RFMNChkCnt;
extern uint16_t canDTIntime;
extern uint16_t u16_TestCnt;
extern uint16_t RK_PermitTime;		/* 100ms */


  
  


extern uint8_t u8_TestFlag;


extern uint8_t u1_rx_data[100];
extern uint8_t u1_rx_point;
extern uint8_t u2_rx_data[100];
extern uint8_t u2_rx_point;
extern uint8_t u3_rx_data[u3Buf_Size];
extern uint8_t u3_rx_point;

extern uint8_t u8_SMTDlmitErCnt;
extern uint8_t u8_SMTCsumErCnt;

extern uint8_t u1Data_frame[40];
extern uint8_t u2Data_frame[40];
extern uint8_t u3Data_frame[40];

extern uint8_t TransCnt_U1;
extern uint8_t TransCnt_U2;
extern uint8_t TransCnt_U3;
extern uint8_t ADC10_Cnt;





//

extern uint8_t t_200msCnt;
extern uint8_t t_1sCnt;


extern uint8_t DMotorSeq; 
extern uint8_t SMotorSeq;
extern uint8_t PumpSeq;
extern uint8_t FanSeq;



extern uint8_t DMotorTKPWM;
extern uint8_t SMotorTKPWM;
extern uint8_t FanTKPWM;


extern uint8_t DRV_PWMDuty;
extern uint8_t STR_PWMDuty;
extern uint16_t FAN_PWMDuty;
extern uint8_t DRV_Speed;			/* 1.2.3.4.5  */ 
extern uint8_t smt_DRVSpeed;

extern uint8_t FAN_Speed;			/* FAN Speed 1,2,3 */
extern uint8_t WLInCnt;
extern uint8_t bcon_Req;
extern uint8_t bcon_Out;
extern uint8_t LowBat_Cnt;

extern uint8_t smt_DbgCommand;
extern uint8_t can_DbgCommand;


extern uint8_t WLEmptyCnt1;			/*  Empty Count 4 */
extern uint8_t WLEmptyCnt2;			/*  Empty Count 4 */
extern uint8_t WLFullCnt1;			/*  Full Count  2 */
extern uint8_t WLFullCnt2;			/*  Full Count  2 */

extern uint8_t u8_pumpspdCnt;
extern uint8_t u8_AutospdCnt;

extern uint8_t u2_ErCnt;
extern uint8_t m8_Buff;



extern uint8_t	 Received_canIDType;
extern uint8_t	 Received_canDLC;

extern uint8_t u3TransReq_cnt;
extern uint8_t crash_Cnt;
extern uint8_t ADRkeyIn_Index;

extern uint8_t LKAUTOSeq;
extern uint16_t LKSeqDlyTime;


extern const unsigned char DRV_ADC[][6];
extern const unsigned char STR_ADC[3];
#if _fan4Level
extern const unsigned char FAN_PWM[6];
#else
extern const unsigned char FAN_PWM[4];
#endif

typedef struct
{
  uint16_t u16_SOCx10;
  uint16_t u16_TTLVoltage;
  uint16_t u16_Current;
  uint8_t  u8_Warning;
  uint8_t  u8_ChgStatus;
}BMS555OutputType;

typedef struct
{
  uint8_t   Major;
  uint8_t   Minor;
  uint8_t   Patch;
  uint8_t   Build;
  uint8_t   ReleaseYear;
  uint8_t   ReleaseMonth;
  uint8_t   ReleaseDay;
  uint8_t   Manufactory;
}BMS632OutputType;

//extern BMS632OutputType BMSFWVER;
//extern BMS555OutputType BMSext555;

typedef struct
{
	uint16_t 	lK_Data;
	uint16_t 	lK_Last;
	uint16_t	lK_Back;
	uint16_t 	lK_ChatCnt;
	uint8_t		lkey_Ident;
}LocalKeyType;



typedef struct
{
	uint16_t 	rK_Data;
	uint16_t 	rK_Last;
	uint16_t	rK_Back;
	uint16_t 	rK_ChatCnt;
	uint8_t		rkey_Ident;
}RFRKeyType;



extern LocalKeyType Local_Key;		//  Local_Key.Seq = 0;
extern RFRKeyType RFR_Key;

#endif  /*__HW_CONFIG_H */

