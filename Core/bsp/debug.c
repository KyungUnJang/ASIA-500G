
/* includes */
#include "hw_config.h"
#include "debug.h"
#include "task.h"
#include "motor.h"
#include "keyscan.h"
uint32_t RTK_check_dly = 0;
uint8_t RTK_check_flag = 0;
/* functions */
/* DATA [0] */
/*
1	#define ERR_DRIVE_DONE            0   // 주행완료 
2	#define ERR_FLASH_READ            1   //메로리 로드 실패 
4	#define ERR_OUT_ROAD            2   //경로 이탈 
8	#define ERR_MEMORY_OVER         3   //메모리 용량 초과 
16	#define ERR_REMOCON            4   //리모컨 인식 실패 
32	#define ERR_collision               5   // 충돌 감지 
64	#define ERR_LOW_BAT            6   //저전압 경보 
128	#define ERR_IMU_COMM_ERR         7   //IMU 통신 에러
*/

/* DATA [1] */
/*
1	#define ERR_HEADING_ERR         8   //헤딩 체크 에러 
2	#define ERR_START_DIS_ERR         9   // 출발 거리 에러 
4	#define ERR_FIND_COURSE_ERR      10   //경로 찾기 에러 
8	#define ERR_COURSE_CHG_ERR      11   //경로 변경 에러  
16	#define ERR_CENTER_SWITCH_ERR   12   // 센터 스위치 안맞음 에러 
32	#define ERR_IMU_ACK_ERR         13   //IMU SENSOR 에러 
64	#define ERR_RTK_MCU_CAN         14		// RTK CAN 에러
128	#define ERR_IMU_MCU_CAN         15		// IMU CAN 에러


/* DATA [2] */
/*
8   #define ERR_CAR_MCU_CAN         16 		// 크래블 <-> 아세아 CAN 에러
16  #define ERR_FIND_COURSE_OK      17		// 경로찾기 OK

*/

void CAN_ER_Decode() 
{
	uCommStatusFlag &= ~f_canERIn;
	uKeyStatusFlag &= ~ frk_AutoDrv;
	uSysStatusFlag &= ~f_AutoRun;
	if( CAN_RxData_Error[1] & 0x01 )
	{	
		cliPrintf("%sHeading Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x01;
	}
	else if ( CAN_RxData_Error[1] & 0x02 )
	{
		cliPrintf("%sDistance Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x02;
	}
	else if ( CAN_RxData_Error[1] & 0x04 )
	{
		cliPrintf("%sLocation Find Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x04;
	}
	else if ( CAN_RxData_Error[1] & 0x08 )
	{
		
		cliPrintf("%sLocation change Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x08;
	}
	else if ( CAN_RxData_Error[1] & 0x10 )
	{

		cliPrintf("%sCenter SW Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x10;	
	}
	else if ( CAN_RxData_Error[1] & 0x20 )
	{

		cliPrintf("%sIMU CAL Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_NotCondition;
		Bcon_dly = 2700;
		CAN_RxData_Error[1] &= ~ 0x20;	
	}
	else if ( CAN_RxData_Error[1] & 0x40 )
	{

		cliPrintf("%sRTK CAN Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		
		CAN_RxData_Error[1] &= ~ 0x40;	
	}
	else if ( CAN_RxData_Error[1] & 0x80 )
	{

		cliPrintf("%sIMU CAN Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		
		CAN_RxData_Error[1] &= ~ 0x80;	
	}
	else if ( CAN_RxData_Error[2] & 0x01 )
	{

		cliPrintf("%sCAR CAN Error\r\n",C_RED);
		cliPrintf("%s",C_NRML);
		
		CAN_RxData_Error[2] &= ~ 0x01;	
	}
	else if ( CAN_RxData_Error[2] & 0x02 )
	{

		cliPrintf("%sLocation FIND OK \r\n",C_RED);
		cliPrintf("%s",C_NRML);
		uSysStatusFlag |= a_Auto_Ready;
		Bcon_dly = 1200;
		CAN_RxData_Error[2] &= ~ 0x02;	
	}

}
void CAN_DT_Decode() 
{
	
	uCommStatusFlag &= ~f_canDTIn;
	if (is_NotUrgency() == 0) 
	{
		uSysStatusFlag &= ~f_TraceLine;
		uSysStatusFlag &= ~f_smartConnected;
		smt_Command = 0;
		return;
	}
	//
	if (CAN_RxData[0] & 0x40 ) // rtk state 1 detect
	{
		RTK_state = 1;
	}
	else // 원래 0 
	{
		RTK_state = 0;
	}
	#if 0
	if (CAN_RxData[0]& 0x20) // 자율주행 조건 판단 
	{
		uSysStatusFlag |= a_NotCondition;
		//uSysStatusFlag &= ~ frk_AutoDrv;
		uSysStatusFlag &= ~f_AutoRun;
		//uSysStatusFlag &= ~f_TraceLine; /* ������ ���� �� */
		//uSysStatusFlag &= ~f_smartConnected;
		//Drive_REQ(KEY_AUTORUN);
		//Bcon_dly = 3000;		
	}
	else 	
	{
		if(!(not_ready_flag & F_DRV_REQ) )
		{
			uSysStatusFlag &= ~ a_NotCondition;
		}
	}
	
	#endif 

	if (CAN_RxData[0] & 0x80) 
	{
		uSysStatusFlag |= f_TraceLine; /* ������ ���� �� */
		smt_Command = (CAN_RxData[1] << 8) | CAN_RxData[0];
		smt_Command &= 0x008D; /* 0000 0000 1000 1101 Heart Bit & Backward Bit Clear */
		uSysStatusFlag |= f_smartConnected;
		//
		if (uSysStatusFlag & f_OPT_AutodrvSPD) 
		{
			smt_DRVSpeed = 3;
		}
		else 
		{
			smt_DRVSpeed = 2;
		}
		
	} 
	else 
	{
		if (uSysStatusFlag & f_TraceLine) 
		{
			uSysStatusFlag &= ~f_TraceLine; /* ������ ���� �� */
			uSysStatusFlag &= ~f_smartConnected;
			smt_Command = 0;
			//
			if (uSysStatusFlag & f_AutoRun)
				all_MotorStop();
		}
	}

}


void Comm_Check() {
	if (f_commCAN) 
	{
		if (uCommStatusFlag & f_canDTIn) 
		{
			CAN_DT_Decode();
		}
		else if (uCommStatusFlag & f_canERIn)
		{
			CAN_ER_Decode();
		}
		else 
		{
			if (uSysStatusFlag & f_AutoRun) 
			{
				if (canDTIntime == 0) 
				{
					uSysStatusFlag |= f_ADRVnoCANData;
					all_MotorStop();
				} 
				else
				{
					uSysStatusFlag &= ~f_ADRVnoCANData;
				}
			}
		}
	}
}
//
