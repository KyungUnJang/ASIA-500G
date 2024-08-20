#include "hw_config.h"
#include "task.h"
#include "indicate.h"
#include "bsp_adc.h"
#include "cli.h"
#include "keyscan.h"
#include "motor.h"
#include "debug.h"
#include "at24_eep.h"
#include "i2c.h"
uint32_t ms_cnt = 0;

uint8_t CAN_RxData[4];
uint8_t CAN_RxData_Error[8];
uint32_t can_Init_time;
CanTxMsg TxMessage;
uint32_t TxMailbox;
uint8_t RTK_state=0;
uint8_t Line_auto_condition = 0;
extern CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef txHeader; //CAN Bus Receive Header
CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure
float Idle_bat_limit = 0;
float DisCharge_bat_limit = 0;

init_setting load_value;



// 모터타입 , 센서타입 , 리모트타입, 배터리타입, IDLE_SPD, LOW SPD, HIGH SPD 
char info[9]={1,2,3,4,5,6,7,8,9};

/*
0 -> NPN
1 -> PNP



*/

void init_set_read()
{
	at24_HAL_ReadBytes(&hi2c1, 0xA0, 0, info, 9);
	
	load_value.Motor_type   = (uint8_t)info[0];
	load_value.Sensor_type  = (uint8_t)info[1];
	load_value.Remote_type  = (uint8_t)info[2];
	load_value.Battery_type = (uint8_t)info[3];
	load_value.IDLE_SPD     = (uint8_t)info[4];
	load_value.ATD_LOW_SPD  = (uint8_t)info[5];
	load_value.ATD_HIGH_SPD = (uint8_t)info[6];
	load_value.Spray_type   = (uint8_t)info[7];
	load_value.rtk_type     = (uint8_t)info[8];
	
	if (load_value.rtk_type == 255 )
	{
		info[0] = 2; // DMCS  
		info[1] = 2; // NPN  확인필요
		info[2] = 1; // 1이 RF Master 
		info[3] = 2; // 롱기스트
		info[4] = 15;     
		info[5] = 103;  
		info[6] = 115; 
		info[7] = 1; // 1이 분무기타입 
		info[8] = 1; // 1이 RTK type

		at24_HAL_WriteBytes(&hi2c1, 0xA0, 0, &info, 9);	
		HAL_Delay(1000);
		at24_HAL_ReadBytes(&hi2c1, 0xA0, 0, info, 9);
		load_value.Motor_type   = (uint8_t)info[0];
		load_value.Sensor_type  = (uint8_t)info[1];
		load_value.Remote_type  = (uint8_t)info[2];
		load_value.Battery_type = (uint8_t)info[3];
		load_value.IDLE_SPD     = (uint8_t)info[4];
		load_value.ATD_LOW_SPD  = (uint8_t)info[5];
		load_value.ATD_HIGH_SPD = (uint8_t)info[6];
		load_value.Spray_type   = (uint8_t)info[7];
		load_value.rtk_type     = (uint8_t)info[8];
	}
	
}

void init_set_save()
{
	
	info[0] = (char)load_value.Motor_type;   
	info[1] = (char)load_value.Sensor_type;  
	info[2] = (char)load_value.Remote_type; 
	info[3] = (char)load_value.Battery_type;
	info[4] = (char)load_value.IDLE_SPD;     
	info[5] = (char)load_value.ATD_LOW_SPD;  
	info[6] = (char)load_value.ATD_HIGH_SPD;
	info[7] = (char)load_value.Spray_type; 
	info[8] = (char)load_value.rtk_type; 
	
	at24_HAL_WriteBytes(&hi2c1, 0xA0, 0, &info, 9);
}




void can1_Trans8b(uint8_t Command) 
{
  //uint8_t ii;
  #if 1 
  ++u32can_TrnCnt;
  switch(Command)
  {
	case 0x01:
	{
		TxMessage.Data[0] = (uint8_t)(uSysStatusFlag & 0x000000FF); 
		TxMessage.Data[1] = (uint8_t)(uSysStatusFlag >> 8 & 0x000000FF);
		TxMessage.Data[0] &= ~0x40;
		txHeader.StdId = 0x737;
		if(uSysStatusFlag & f_RFR_Emergency)
		{
			if(LKAUTOSeq >= 6)
				TxMessage.Data[1] &= 0xBF;			/* 1011 1111 RFR_Emergency �� �����ؼ� ����. */				
		}
		if(can_DbgCommand == 1)
		{
			printf("\n TRS_ID[%0x] DLC[%0d] DT0[%0x] DT1[%0x]",TxMessage.StdId,TxMessage.DLC,TxMessage.Data[0],TxMessage.Data[1]);
		}	
	    HAL_CAN_AddTxMessage(&hcan, &txHeader, &TxMessage.Data, &TxMailbox);
	}
	break;
		

	case 0x02:
	{
		TxMessage.Data[0] = (uint8_t)(uSysStatusFlag & 0x000000FF); 
		TxMessage.Data[0] |= 0x40;
		TxMessage.Data[1] = (uint8_t)(uSysStatusFlag >> 8 & 0x000000FF);
		txHeader.StdId = 0x737;

		HAL_CAN_AddTxMessage(&hcan, &txHeader, &TxMessage.Data, &TxMailbox);
	}
	break;

	case 0x03:
	{	
		TxMessage.Data[0] = CAN_RxData_Error[0];
		TxMessage.Data[1] = CAN_RxData_Error[1];
		TxMessage.Data[2] = CAN_RxData_Error[2];	
		TxMessage.Data[3] = CAN_RxData_Error[3];
		TxMessage.Data[4] = CAN_RxData_Error[4];	
		TxMessage.Data[5] = CAN_RxData_Error[5];
		TxMessage.Data[6] = CAN_RxData_Error[6];	
		TxMessage.Data[7] = CAN_RxData_Error[7];
		txHeader.StdId = 0x501;
		HAL_CAN_AddTxMessage(&hcan, &txHeader, &TxMessage.Data, &TxMailbox);
	}

	default:
		break;
	
  }
  #endif
}


void Event_10ms();
void Event_30ms();
void Event_200ms();
void Event_500ms();	
void Battery_Check();
void Bcon_Out();
void WL_CheckInit();
void WL_Check();
void Bcon_Control();
uint8_t NotEmpty_WL();
uint8_t count;
uint8_t center_start = 0;

void Event_30ms()
{
  uTimerFlag &= ~TIM_30ms;
  get_adc_value();
  if(RK_PermitTime)	--RK_PermitTime;
  if(f_commCAN)
  {
	if ( center_start == 0 )
	{
		can1_Trans8b(0x01);	//  CAN
	}
	else 
	{
		can1_Trans8b(0x02);
		count++;
		if ( count  > 3)
		{
			center_start =0;
		}
	}
  }
  Bcon_Control();
}

void Event_10ms()
{
  uTimerFlag &= ~TIM_10ms;  
}

void Event_200ms()
{
	uTimerFlag &= ~TIM_200ms;
	G_LED_Toggle(); 	  	/* Green led로 FIX */
	WL_Check();
	Battery_Check();
	#if 0 
	++t_200msCnt;
	if(t_200msCnt >= 5)
	t_200msCnt = 0;
	//
	switch(t_200msCnt)
	{
	case 1:
		//Debug_1s_PRT();
		break;
		//
	case 2:
		Battery_Check();
		break;
	case 3:
		break;	
	case 4:
		break;
	default:
		break;
	}
  #endif 
}
//
void Event_500ms()
{
  uTimerFlag &= ~TIM_500ms;
  uTimerFlag ^= TIM_1SecMaker;
  if(uTimerFlag & TIM_1SecMaker)
  {
	//uTimerFlag |= TIM_1Sec;	
	++u32_SysRunTime;
  }
}
void Timer_Event()
{
    if(uTimerFlag & TIM_10ms)	Event_10ms();
    if(uTimerFlag & TIM_30ms)	Event_30ms();		
    if(uTimerFlag & TIM_200ms)	Event_200ms();
    if(uTimerFlag & TIM_500ms)	Event_500ms();	
}

void bsp_can_init()
{
    TxMessage.Data[0] = 0x11; 
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0x44;
    txHeader.DLC = 8;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.StdId = 0x737;
    
    txHeader.TransmitGlobalTime = DISABLE;

    
	
  	sFilterConfig.FilterFIFOAssignment=CAN_FILTER_FIFO0; //set fifo assignment
	sFilterConfig.FilterIdHigh=0x737; //the ID that the filter looks for (switch this for the other microcontroller)
	sFilterConfig.FilterIdLow=0;
	sFilterConfig.FilterMaskIdHigh=0;
	sFilterConfig.FilterMaskIdLow=0;
	sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT; //set filter scale
	sFilterConfig.FilterActivation=ENABLE;
	
	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig); //configure CAN filter
	HAL_CAN_Start(&hcan); //start CAN
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING); //enable interrupts

}

extern LocalKeyType Local_Key;


void R_LED_On(void)
{
  	HAL_GPIO_WritePin(GPIOE, E_LED_R, GPIO_PIN_RESET);
}
void R_LED_Off(void)
{
  	HAL_GPIO_WritePin(GPIOE, E_LED_R, GPIO_PIN_SET);
}
void R_LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOE, E_LED_R);
}
void G_LED_On(void)
{
  	HAL_GPIO_WritePin(GPIOE, E_LED_G, GPIO_PIN_RESET);
}
void G_LED_Off(void)
{
	HAL_GPIO_WritePin(GPIOE, E_LED_G, GPIO_PIN_SET);
}
void G_LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOE, E_LED_G);
}
void B_LED_On(void)
{
	HAL_GPIO_WritePin(GPIOB, B_LED_Y, GPIO_PIN_RESET);
}
void B_LED_Off(void)
{
	HAL_GPIO_WritePin(GPIOB, B_LED_Y, GPIO_PIN_SET);
}
void B_LED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOB, B_LED_Y);
}
void bcon_On(uint8_t bcon_no)
{
  if((bcon_no & 0x01) == 0x01)
	HAL_GPIO_WritePin(GPIOC, C_BEACON_R, GPIO_PIN_SET);
  else
	HAL_GPIO_WritePin(GPIOC, C_BEACON_R, GPIO_PIN_RESET);
  if((bcon_no & 0x02) == 0x02)
	HAL_GPIO_WritePin(GPIOB, B_BEACON_G, GPIO_PIN_SET);
  else
	HAL_GPIO_WritePin(GPIOB, B_BEACON_G, GPIO_PIN_RESET);
  if((bcon_no & 0x04) == 0x04)
	HAL_GPIO_WritePin(GPIOB, B_BEACON_B, GPIO_PIN_SET);
  else
	HAL_GPIO_WritePin(GPIOB, B_BEACON_B, GPIO_PIN_RESET);
  if((bcon_no & 0x08) == 0x08)
	HAL_GPIO_WritePin(GPIOA, A_BUZZER, GPIO_PIN_SET);
  else
	HAL_GPIO_WritePin(GPIOA, A_BUZZER, GPIO_PIN_RESET);
}

void bcon_Off(uint8_t bcon_no)
{
  if((bcon_no & 0x01) == 0x01)
	HAL_GPIO_WritePin(GPIOC, C_BEACON_R, GPIO_PIN_RESET);
  if((bcon_no & 0x02) == 0x02)
	HAL_GPIO_WritePin(GPIOB, B_BEACON_G, GPIO_PIN_RESET);
  if((bcon_no & 0x04) == 0x04)
	HAL_GPIO_WritePin(GPIOB, B_BEACON_B, GPIO_PIN_RESET);
  if((bcon_no & 0x08) == 0x08)
	HAL_GPIO_WritePin(GPIOA, A_BUZZER, GPIO_PIN_RESET);
}

//
void Battery_Check()
{
  if(u16_DebugFlag == 2)
  {
    cliPrintf("\n Battery : ADC[%0d] Voltage[%.1f]",ADC_Battery,V_Battery);
    cliPrintf("\n RFR_Volume : ADC[%0d] Voltage[%.1f] DRV_SPD[%0d]",ADC_RFRVolume,V_RFRVolume,DRV_Speed);
  }
  if(V_Battery < DisCharge_bat_limit)
  {							/* All 모터 Stop이 되면, 다시 전원이 상승해 해제 된다. */
	++LowBat_Cnt;
	if(LowBat_Cnt >= 5)
	{
		LowBat_Cnt = 5;
#if _LOWBAT_MASK  		
		if((uSysStatusFlag & f_LowBat) == 0)
		{
			uSysStatusFlag |= f_LowBat;
			if(LKAUTOSeq)
			{
				LKAUTOSeq = 0;
			} 
			all_MotorStop();			
			if(u16_DebugFlag == 2)
			{
				cliPrintf("  ! LOW V. ![%0x]",uSysStatusFlag & f_LowBat);
				u16_DebugFlag = 0;
			}
		}
#endif		
	}
  }
  else
  {
	//uSysStatusFlag &= ~f_LowBat;
#if _BAT_P
	cliPrintf("\n 11_FLAG[%0x]",uSysStatusFlag & f_LowBat);
#endif
	
	if(uSysStatusFlag & f_LowBat)
	{
		if(V_Battery > Idle_bat_limit)
		{
			uSysStatusFlag &= ~f_LowBat;
#if _BAT_P
			cliPrintf("\n - RESET LOW BAT");
#endif
		}
		else
		{
#if _BAT_P
			cliPrintf("\n FLAG[%0x]",uSysStatusFlag & f_LowBat);
#endif
		}
	}
	else
	{
		LowBat_Cnt = 0;
#if _BAT_P
		cliPrintf("  ! STABLE !");
#endif
	}
  }
/* RFR Volume Check */
#if 0
  cliPrintf("\n RFR_Volume : ADC[%0d] Voltage[%.1f] DRV_SPD[%0d]",ADC_RFRVolume,V_RFRVolume,DRV_Speed);
#endif
}
//
uint32_t PC_Data;
uint32_t Temp_Data;


void Port_ReadACT() // limit SW ?
{

	#if 1 
	if(load_value.Sensor_type == 1)
	{
		PC_Data = GPIOC->IDR & 0x001F ;	  
	}	
	else 
	{
		Temp_Data = GPIOC->IDR & 0x001F ;	  
		PC_Data = Temp_Data^0x001B; 
	}	

	#endif 

	if(PC_Data & 0x0001)
	{
		if((uSysStatusFlag & f_LimitRight) == 0)
		{
			str_MotorLimitStop();
			uSysStatusFlag &= ~f_LimitLeft;
			uSysStatusFlag |= f_LimitRight;
		}
	}
	//
	if(PC_Data & 0x0002)
	{
		if((uSysStatusFlag & f_LimitLeft) == 0)
		{
			str_MotorLimitStop();
			uSysStatusFlag &= ~f_LimitRight;
			uSysStatusFlag |= f_LimitLeft;
		}
	}

	if((PC_Data & 0x0003) == 0)
	{
		uSysStatusFlag &= ~f_LimitRight;
		uSysStatusFlag &= ~f_LimitLeft;
	}

	if(PC_Data & 0x0010)	/* Center Check  CENTER는 기구 형상이 반대임 */
	{
		if(uSysStatusFlag & f_LimitCenter) 
		{
			uSysStatusFlag &= ~f_LimitCenter;
		}
	}
	else
	{
		if((uSysStatusFlag & f_LimitCenter) == 0)
		{
			uSysStatusFlag |= f_LimitCenter;
		}
	}

	if(PC_Data & 0x0004)				/* 0000 0100*/ //범퍼 	
	{
		if((uSysStatusFlag & f_Crash) == 0)
		{
			--crash_Cnt;			/* 22012	2 */
			if(crash_Cnt == 0)
			{
				uSysStatusFlag |= f_Crash;
				if(LKAUTOSeq) 
					LKAUTOSeq = 0;
				
				all_MotorStop();
			}
		}
	}
	else
	{
		uSysStatusFlag &= ~f_Crash;
		crash_Cnt = 50;				/* 220122 */
	}
}


#if 0
	Low Battery : Magenta + Buzzer
	RFR_Main 	: Red + Buzzer
	Crash		: Blue + Buzzer
	ALLWLEmpty	: Cyan + Buzzer
#endif

uint32_t Bcon_dly;
uint8_t toggle_flag = 0;


void Bcon_Control()
{
  if(uSysStatusFlag & f_LowBat)
  {
	  bcon_Req = bcon_R | bcon_B | bcon_BZ;
  }
  else if(uSysStatusFlag & f_RFR_Emergency)
  {
    if(LKAUTOSeq  >= 4)
    {
      bcon_Req = bcon_R;
      if(LKAUTOSeq >= 7)
      {
        bcon_Req = bcon_G;
      }
      if(uSysStatusFlag & f_ALLWLEmpty)
        {
        bcon_Req = bcon_B | bcon_G;						/* Cyan */
        if(uSysStatusFlag & f_WTEmptBuzReq)
          bcon_Req = bcon_B | bcon_G | bcon_BZ;		/* Cyan + Buzzer */	
        }		
    }
	else
		bcon_Req = bcon_R | bcon_BZ;
  }
  else if(uSysStatusFlag & f_Crash)
  {
	bcon_Req = bcon_B | bcon_BZ;
  }
  else if(uSysStatusFlag & f_ALLWLEmpty)
  {
	
	if(uSysStatusFlag & f_WTEmptBuzReq)
	{
		bcon_Req = bcon_B | bcon_G | bcon_BZ;		/* Cyan + Buzzer */	
	}
	if ( load_value.rtk_type == 1) 
	{
		if ( RTK_state == 1 ) /* 물없고 RTK OK */ 
		{
			if ( !Bcon_dly )
			{
				uSysStatusFlag &= ~a_NotCondition;
				uSysStatusFlag &= ~a_Auto_Ready;
			}
			if (uSysStatusFlag & a_NotCondition )
			{
				bcon_Req = bcon_R| bcon_G | bcon_BZ;
			}
			else if( uSysStatusFlag & a_Auto_Ready)
			{
				bcon_Req = bcon_G | bcon_BZ;
			}
			
			else 
			{
				bcon_Req = bcon_B | bcon_G;
			}
			
		}
		else /* 물없고 RTK NONE */ 
		{

			if ( not_ready_flag & F_DRV_REQ)
			{
				
				bcon_Req = bcon_R| bcon_G | bcon_BZ;
				if (!Bcon_dly)
				{
					not_ready_flag &= ~F_DRV_REQ;
					uSysStatusFlag &= ~a_NotCondition;
				}
			}
			else
			{	
				if ( !Bcon_dly  )
				{
					
					if ( toggle_flag == 0)
					{
						bcon_Req = bcon_B | bcon_G;						/* 물없을때, 일반상태 */ 
						Bcon_dly = 1000;
						toggle_flag++;
					}
					else
					{
						bcon_Req = bcon_R| bcon_G; // 노랑 
						Bcon_dly = 1000;
						toggle_flag = 0;
					}
					

				}
			}
			
		}
	}
	else 
	{
		bcon_Req = bcon_B | bcon_G;
	}
  }
  else
  {	/* Normal Status */
	if ( load_value.rtk_type == 1)
	{
		if ( RTK_state == 1 ) /* 물있고 RTK OK */ 
		{
			if ( !Bcon_dly )
			{
				uSysStatusFlag &= ~a_NotCondition;
				uSysStatusFlag &= ~a_Auto_Ready;
			}
			if (uSysStatusFlag & a_NotCondition )
			{
				bcon_Req = bcon_R| bcon_G | bcon_BZ;
			}
			else if( uSysStatusFlag & a_Auto_Ready)
			{
				bcon_Req = bcon_G | bcon_BZ;
			}
			else 
			{
				bcon_Req = bcon_G;
			}

			// 여기서 자율항상 시작 물있고 다 정상일때.
		}
		else /* 물있고 RTK NONE */ 
		{
			if ( not_ready_flag & F_DRV_REQ)
			{
				
				bcon_Req = bcon_R| bcon_G | bcon_BZ;
				if (!Bcon_dly)
				{
					not_ready_flag &= ~F_DRV_REQ;
					uSysStatusFlag &= ~a_NotCondition;
				}
			}
			else 
			{
				if ( !Bcon_dly )
				{
					if ( toggle_flag == 0)
					{
						bcon_Req = bcon_G;						/* 물없을때, 일반상태 */ 
						Bcon_dly = 1000;
						toggle_flag++;
					}
					else
					{
						bcon_Req = bcon_R| bcon_G; // 노랑 
						Bcon_dly = 1000;
						toggle_flag = 0;
					}
				
				}
			}
		}
	}
    else
	{
		bcon_Req = bcon_G;	
	}
  }
  Bcon_Out(); 
}
//
void Bcon_Out()
{
  if(bcon_Req != bcon_Out)
  {
	bcon_On(bcon_Req);
	bcon_Out = bcon_Req;
  }
}
//
#if 1	
// 분사중이고, 자율주행중 수위 가 없으면, 
uint8_t Real_AutoRunning()
{
  if((uSysStatusFlag & f_AutoRun) == 0)
  {
  	return 0;
  }
  if((uSysStatusFlag & f_TraceLine) == 0)
  {
  	return 0;
  }
  if((smt_Command & 0x0001) == 0)
  {
  	return 0;
  }
  if((uSysStatusFlag & f_DMotorACT) == 0)
  {
  	return 0;
  }
  return 1;
}
#endif

//
void WLS_Check1()
{
#if _WLS_MASK  		
  if(HAL_GPIO_ReadPin(GPIOC,C_WLS_IN1) == 0)	/* 견인 물통 */
  {
	++WLEmptyCnt1;
	if(WLEmptyCnt1 >= 20)  /* 200m x 20 = 4000ms */
	{
		if((uSysStatusFlag & f_WLEmpty1) == 0)
		{
			uSysStatusFlag |= f_WLEmpty1;
#if _WLS_CHK
			cliPrintf("\n WL_TOW_EMPTY");
#endif
		}
		WLEmptyCnt1 = 20;
		WLFullCnt1 = 0;
	}
  }
  else
  {
	++WLFullCnt1;
	if(WLFullCnt1 >= 20)
	{
		uSysStatusFlag &= ~f_WLEmpty1;
#if _WLS_CHK
		cliPrintf("\n WL_TOW_FULL");
#endif
		WLEmptyCnt1 = 0;
		WLFullCnt1 = 20;
	}
  }
#else
  uSysStatusFlag &= ~f_WLEmpty1;
  WLEmptyCnt1 = 0;
  WLFullCnt1 = 20;
#endif
}
//
void WLS_Check2()
{
#if _WLS_MASK  		
  if(HAL_GPIO_ReadPin(GPIOC,C_WLS_IN2) == 0)	/* 본체 물통 */
  {
	++WLEmptyCnt2;
	if(WLEmptyCnt2 >= 20)  /* 200m x 20 = 4000ms */
	{
		if((uSysStatusFlag & f_WLEmpty2) == 0)
		{
			uSysStatusFlag |= f_WLEmpty2;
#if _WLS_CHK
			cliPrintf("\n WL_BB_EMPTY");
#endif
		}
		WLEmptyCnt2 = 20;
		WLFullCnt2 = 0;
	}
  }
  else
  {
	++WLFullCnt2;
	if(WLFullCnt2 >= 20)
	{
		uSysStatusFlag &= ~f_WLEmpty2;
#if _WLS_CHK
		cliPrintf("\n WL_BB_FULL");
#endif
		WLEmptyCnt2 = 0;
		WLFullCnt2 = 20;
	}
  }
#else
  uSysStatusFlag &= ~f_WLEmpty2;
  WLEmptyCnt2 = 0;
  WLFullCnt2 = 20;
#endif
}
//
void WL_CheckInit()
{
  WLS_Check2(); 		/* 본체 */
  WLS_Check1();		  /* 견인 */
}

//#define E_SOLv_1			GPIO_Pin_12		// Sol. Velve 
void WL_Check()
{
  WLS_Check2();   /* 본체 */
  WLS_Check1();   /* 견인 */
  //
  if(NotEmpty_WL())
  { /* Sol V. Control */
	if((uSysStatusFlag & f_WLEmpty1) == 0)			/* 견인 물통 Full */
	{
		HAL_GPIO_WritePin(GPIOE, E_SOLv_1, GPIO_PIN_RESET);
    //GPIO_ResetBits(GPIOE,E_SOLv_1);				/* 견인쪽 (세워진 방향)*/
		//GPIO_ResetBits(GPIOE,E_SOLv_2);			
	}
	else
	{
    HAL_GPIO_WritePin(GPIOE, E_SOLv_1, GPIO_PIN_SET);
		//GPIO_SetBits(GPIOE,E_SOLv_1);				/* 본체쪽 (눕혀진 방향)*/
		//GPIO_SetBits(GPIOE,E_SOLv_2);	
	}
	//	
	uSysStatusFlag &= ~f_ALLWLEmpty;
	uSysStatusFlag &= ~f_WTEmptBuzReq;
  }
  else		/* Water Empty */
  {
	uSysStatusFlag |= f_ALLWLEmpty;
#if _PUMP_P
	cliPrintf("\n !!! PUMP & FAN STOP Seq !!!");
#endif
	if(uSysStatusFlag & f_PumpACT)
	{
		if(Real_AutoRunning())		/* 분사중이고 자율주행 중이면 All Motor Stop */ 	
		{
			uSysStatusFlag |= f_ADRVnoWaterStop;
			uSysStatusFlag |= f_WTEmptBuzReq;
			if(LKAUTOSeq) 
				LKAUTOSeq = 0;
			all_MotorStop();
		}
		else
		{
			uSysStatusFlag &= ~f_ADRVnoWaterStop;		
			PumpSeq = PUMP_STOP;
			PumpWaittime = 10;
		}
	}
	else
	{
		uSysStatusFlag &= ~f_ADRVnoWaterStop;		
	}
	//
	if(uSysStatusFlag & f_FanACT)
	{
		if(FanSeq != FAN_STOP)
		{
			FanSeq = FAN_STOP;
			FanWaittime = 2000;		/* Pump Off 후 4초 뒤 Fan Off 한다. */
		}
	}
  }
}

//
void Option_Check()
{
  if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Pumpspeed) == 1)
  { /* 접점위치, 1이 눌러진 상태 */
#if 0  
	cliPrintf("\n Pump_Speed");
#endif
	++u8_pumpspdCnt;
	if(u8_pumpspdCnt >= 40)
	{
		if((uSysStatusFlag & f_OPT_Pumpspeed) == 0)
			uSysStatusFlag |= f_OPT_Pumpspeed;		/* Pump 5 */
		
		u8_pumpspdCnt = 40;
		//cliPrintf("\n PUMP PRESS HIGH-5   ");
		
	}
  }
  else
  {
	if(u8_pumpspdCnt)
		--u8_pumpspdCnt;
	//
	if(u8_pumpspdCnt == 0)
	{
		if((uSysStatusFlag & f_OPT_Pumpspeed) == f_OPT_Pumpspeed)
			uSysStatusFlag &= ~f_OPT_Pumpspeed; 
		//
		//cliPrintf("\n PUMP PRESS LOW-3   ");
		
	}
  }
  //
  if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
  { /* 접점위치 */
	++u8_AutospdCnt;
	if(u8_AutospdCnt >= 40)
	{
		if((uSysStatusFlag & f_OPT_AutodrvSPD) == 0)
			uSysStatusFlag |= f_OPT_AutodrvSPD;
  
		u8_AutospdCnt = 40;
		//cliPrintf("HIGH_SPLAY_SPEED");
	}
  }
  else
  {
	if(u8_AutospdCnt)
		--u8_AutospdCnt;
	//
	if(u8_AutospdCnt == 0)
	{
		if((uSysStatusFlag & f_OPT_AutodrvSPD) == f_OPT_AutodrvSPD)
			uSysStatusFlag &= ~f_OPT_AutodrvSPD; 
		
		//cliPrintf("LOW_SPLAY_SPEED");
	}
  }
}
//
void INIT_Check()
{
  uint8_t	iii,jjj;

  for(iii = 0; iii < 30; iii++)
	Option_Check();

  if(u16_DebugFlag == 1)
  {
    if(uSysStatusFlag & f_OPT_AutodrvSPD)
    cliPrintf("\n AUTO DRV SPEED : HIGH(3)");
    else
    cliPrintf("\n AUTO DRV SPEED : LOW(2)");
    
    if(uSysStatusFlag & f_OPT_Pumpspeed)
      cliPrintf("   PUMP_HIGH(5_LEVEL)");
    else
    cliPrintf("   PUMP_LOW(3_LEVEL)");
    //
    if(f_commCAN)
      cliPrintf("  --> CAN");
    else
    cliPrintf("  --> UART");

  #if _PT_NPN
    cliPrintf("  NPN");	
  #else
    cliPrintf("  PNP");	
  #endif
  }
  
  
  for(iii = 0; iii < 5; iii++)
  {
	bcon_Req = 0;
	Bcon_Out();
	R_LED_Off();
	G_LED_On();
	for(jjj = 0; jjj < 5; jjj++)
	{
    	HAL_Delay(80);
		WL_CheckInit(); 		/* */
	}
	Battery_Check();
	bcon_Req = bcon_R;
	Bcon_Out();
	R_LED_On();
	G_LED_Off();
	for(jjj = 0; jjj < 5; jjj++)
	{
		HAL_Delay(80);
		WL_CheckInit(); 
	}

  }
  
  // Init Sol_V 정열
  if((uSysStatusFlag & f_WLEmpty1) == 0)
  {	
    /* 견인 물통 Full */
	  HAL_GPIO_WritePin(GPIOE, E_SOLv_1, GPIO_PIN_RESET);
  }
  else if((uSysStatusFlag & f_WLEmpty2) == 0)
  {	
    /* 본체 물통 Full */	
	  HAL_GPIO_WritePin(GPIOE, E_SOLv_1, GPIO_PIN_SET);
  }
  else
  {	
    /* All WT Empty */
	  HAL_GPIO_WritePin(GPIOE, E_SOLv_1, GPIO_PIN_RESET);
  }
  // Sys 안정화 Delay 5초 
  for(m8_Buff = 0; m8_Buff < 4; m8_Buff++)
  {
    bcon_Req = 0;
    Bcon_Out();
    R_LED_Off();
    G_LED_On();
    HAL_Delay(250);
        
    bcon_Req = bcon_R;
    Bcon_Out();
    R_LED_On();
    G_LED_Off();
    HAL_Delay(250);
  }
  //  
  bcon_Req = 0;
  Bcon_Out();
  R_LED_Off();
  G_LED_On();	/* Run Indicator */
  
}
//
/*
	본체와 견인물통이 한족이라도 물이 잇으면, 1을 리턴 함.
*/
uint8_t NotEmpty_WL()
{
  if((uSysStatusFlag & f_WLEmpty1) && (uSysStatusFlag & f_WLEmpty2))
	  return 0;
  else
	  return 1;
}
//
void Var_Init()
{
  uSysStatusFlag = 0;
  
  Local_Key.lK_ChatCnt = 0;
  RFR_Key.rK_ChatCnt = 0;
  RK_PermitTime = 0;
  	
  smt_Command = 0;
  u16_DebugFlag = 0;
  u8_TestFlag = 0;
  uTimerFlag = 0;
	  
  WLEmptyCnt1 = 0;		  /* 견인 Empty Count 4초 */
  WLEmptyCnt2 = 0;		  /* 본체 Empty Count 4초 */
  WLFullCnt1 = 0;		  /* 견인 Full Count  2초 */
  WLFullCnt2 = 0;		  /* 본체 Full Count  2초 */
  	
  ADC10_Cnt = 0;
  LowBat_Cnt = 0;
  	  
  DMotorSeq = DMOTOR_STOP;
  SMotorSeq = SMOTOR_STOP;
  FanSeq = FAN_STOP;
  PumpSeq = PUMP_STOP;
	  
  FAN_Speed = 1;
  DRV_Speed = 1;
  	  
  DMotorTKPWM = DRV_ADC[DRV_SPDTBNo][DRV_Speed];
  SMotorTKPWM = STR_ADC[2];
  FanTKPWM = FAN_PWM[FAN_Speed];
  	
  u8_pumpspdCnt = 20;
  u8_AutospdCnt = 20; 
  
  	
  u8_SMTDlmitErCnt = 0; 
  u8_SMTCsumErCnt = 0;
  u2_ErCnt = 0;
	
  u16_TestCnt = 0;
  smt_DbgCommand = 0;

  LKAUTOSeq = 0;

  uSysStatusFlag |= f_Pump1st;
  
  bcon_Req = 0;
  bcon_Out = 0;
  bcon_Req = bcon_R;
  Bcon_Out();
}
void init_set()
{
	if ( load_value.Motor_type == 1)
	{
		DRV_SPDTBNo = 0; // Curtis type
	}
	else 
	{
		DRV_SPDTBNo = 1; // DMCS type
	}
	

	if ( load_value.Battery_type == 1 )
	{
		
		Idle_bat_limit = 48;   // Atlas
		DisCharge_bat_limit = 44.5; 
	}
	else  
	{
		Idle_bat_limit = 47;   // Longist
		DisCharge_bat_limit = 44.5;
	}

	if ( load_value.Remote_type == 1)  // RF MASTER
	{

	}
	else 
	{

	}

	



	

}
/////////////////////////////////////////////////////////////////////////////////
