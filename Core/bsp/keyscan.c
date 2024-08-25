#include "keyscan.h"
#include "define.h"
#include "hw_config.h"
#include "motor.h"
#include "hw_config.h"
#include "task.h"

extern LocalKeyType Local_Key;
uint8_t isAutoCondition();

void GPIO_Configuration(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

    

    /* PORT A Input Define_ADC :  PA 0, 1, 2, 3  */
    GPIO_InitStruct.Pin  = A_in_ADC1_0 | A_in_ADC1_1 | A_in_ADC1_2 | A_in_ADC1_3;   
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);                                       
	#if 0
	/* PORT A Output Define_DAC : PA 4, 5 */
	GPIO_InitStruct.Pin  = GPIO_DAC1 | GPIO_DAC2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
	#endif 
	/* PORT A Output Define : PA 6(Buzzer) */
	GPIO_InitStruct.Pin   = A_BUZZER; 
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    #if 0 
	// UART 1 configuration 
    /* Configure USAR1_Tx(PA9) as alternate function push-pull */
    GPIO_InitStruct.Pin   = GPIO_UART1_Tx;
    GPIO_InitStruct.Mode  = GPIO_Mode_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USAR1_Rx(PA10) as input floating */
    GPIO_InitStruct.Pin  = GPIO_UART1_Rx;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	// CAN 
	/* Configure CAN1_Rx(PA11) */
    GPIO_InitStruct.Pin = GPIO_CAN1_Rx;		
    GPIO_InitStruct.Mode = GPIO_Mbode_IPU;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* Configure CAN1_Tx(PA12) */
    GPIO_InitStruct.Pin = GPIO_CAN1_Tx;
    GPIO_InitStruct.Mode = GPIO_Mode_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
	#endif 
	
	GPIO_InitStruct.Pin   = B_BEACON_G | B_BEACON_B | GPIO_I2C_WP | GPIO_I2C_SCL | GPIO_I2C_SDA;	   
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = B_LED_Y | B_LED_W;	 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);										 
	#if 0
	GPIO_InitStruct.Pin   = GPIO_UART3_Tx;
	GPIO_InitStruct.Mode  = GPIO_Mode_AF_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = GPIO_UART3_Rx;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	#endif
	GPIO_InitStruct.Pin  = B_PMPON | B_PMPUP | B_PMPOFF | B_PMPDN;	 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
	HAL_GPIO_WritePin(GPIOB, B_PMPON, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, B_PMPOFF, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, B_PMPDN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin  = C_STRright_LMT | C_STRleft_LMT | C_STRcenter;   
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = C_CRASH_1;   
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = C_WLS_IN1 | C_WLS_IN2;   
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = C_BEACON_R;	 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	GPIO_InitStruct.Pin  = C_OPT_RDY | C_OPT_VISION;	 
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);										 
	GPIO_InitStruct.Pin  = IR_FORWARD | IR_AUTODRIVE | IR_FANONUP | IR_FANOFF;	 
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);										 
    GPIO_InitStruct.Pin  = IR_BACKWARD | IR_LEFT | IR_RIGHT | IR_PUMP | IR_Main;   
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                       
	#if 0 
	GPIO_InitStruct.Pin   = GIO_UART2_Tx;
	GPIO_InitStruct.Mode  = GPIO_Mode_AF_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	GPIO_InitStruct.Pin  = GIO_UART2_Rx;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* PORT D Output Define  PWM */
	GPIO_InitStruct.Pin   = PWM_RDY | PWM_FAN; 
	GPIO_InitStruct.Mode = GPIO_Mode_AF_PP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);	
	#endif 
	GPIO_InitStruct.Pin   = E_LED_R | E_LED_G; 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
	GPIO_InitStruct.Pin   = E_strM_BREAK | E_strM_CWCCW; 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
	GPIO_InitStruct.Pin   = E_drvM_BREAK | E_drvM_CWCCW | E_SOLv_1; 
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
    GPIO_InitStruct.Pin  = E_KEY_EMERGENCY | E_KEY_FORWARD | E_KEY_BACKWORD | E_KEY_LEFT | E_KEY_RIGHT;   
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);                                       
	/* PORT E Output Define :  PE 0, 1, 8, 9, 11, 12 */
    GPIO_InitStruct.Pin  = E_OPT_Pumpspeed | E_OPT_Autospeed;   
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);                                       
	R_LED_Off();
	G_LED_On();
	B_LED_Off();
	#if _ATBD_PST
	f_commCAN = TRUE;		/* PST BD --> CAN */
	#endif 
}

/*
 * (C) COPYRIGHT 2009 CRZ
 *
 * File Name : task.c
 * Author    : 
 * Version   : 
 * Date      : 
 */
#include "hw_config.h"

extern LocalKeyType Local_Key;
//extern uint8_t SW_I2C_WriteControl_8Bit(uint8_t pageaddr, uint8_t regaddr, uint8_t data);


/* functions */
void DRV_Start()
{
  if((uSysStatusFlag & f_DMotorACT) == 0)
  {
	DMotorSeq = DMOTOR_START; 		  /* Local & RFR Forward */
	uSysStatusFlag |= f_DMotorACT;
	DMotorWaitime = 10;
  }
}
//
void STR_Start()
{
  if((uSysStatusFlag & f_SMotorACT) == 0)
  {

	SMotorSeq = SMOTOR_START;
	uSysStatusFlag |= f_SMotorACT;
	SMotorWaitime = 10;
  }
}
//
//
//

#if 0
#define KEY_NONE		0
#define KEY_FORWARD		1
#define KEY_BACKWORD	2
#define KEY_LEFT		4
#define KEY_RIGHT		8
//
#endif
void LK_FlagClear()
{
  uKeyStatusFlag = uKeyStatusFlag & 0xFFF0;		
}
//
/*
#define frk_Emergency		(1 << 5)
#define frk_Forward			(1 << 6)
#define frk_Backward		(1 << 7)
#define frk_Left			(1 << 8)
#define frk_Right			(1 << 9)
#define frk_AutoDrv			(1 << 10)
#define frk_PumpFan			(1 << 11)
#define frk_FSpdUp			(1 << 12)
#define frk_FSpdDn			(1 << 13)
*/
#if 0
void RK_FlagClear()
{
  uKeyStatusFlag = uKeyStatusFlag & 0x000F;		/* 1100 0000 0001 1111 */
}
#endif
//
uint8_t is_NotUrgency()
{
  if(((uSysStatusFlag & f_Crash) == 0) && ((uSysStatusFlag & f_LowBat) == 0))
  {
  	if((uSysStatusFlag & f_RFR_Emergency) == 0)
		return 1;
	else
	{
		if(LKAUTOSeq >= 4)
		{
			return 1;
		}
		else
		{
			if(LKAUTOSeq < 3)
			{
				if(u16_DebugFlag == 1)
					cliPrintf("\n URGENCY - RFR_Not MAIN");
			}
			return 0;
		}
	}
  }
  else
  {
	if(u16_DebugFlag == 1)
	{
		cliPrintf("\n URGENCY - ");
		if(uSysStatusFlag & f_Crash)
			cliPrintf("CRASH ");
		//
		if(uSysStatusFlag & f_LowBat)
			cliPrintf("Low Bat. ");
		//
		if(uSysStatusFlag & f_RFR_Emergency)
			cliPrintf("RFR_Not MAIN ");
	}
	return 0;
  }
}
#if 0
#define f_LowBat			(1 << 13)	/* 1 =  Battery V. Low 35V 이하 */
#define f_Crash				(1 << 14)	/* 1 = 충돌 센서 감지  */
#define f_RFR_Emergency		(1 << 15)	/* 1 =  Battery V. Low 35V 이하 */
충돌 : 후진 Key 만 동작 한다.
구동과 조향 Key 이외는 위 3가지 경우에는 동작 하지 않는다.
#endif
void Drive_REQ(uint8_t Control)
{
  if(uSysStatusFlag & f_Crash)		/* 충돌 중에는 후진 Key 만 먹는다. */
  {
	if(Control != KEY_BACKWORD)
	{
		Control = KEY_NONE;	
	}
  }
  //
  switch(Control)
  {
	case KEY_FORWARD:
		uSysStatusFlag |= f_DMForward;
		DRV_Start();
		if(u16_DebugFlag == 3)
			cliPrintf("\n DRV_ForStart");

		break;
		//
	case KEY_BACKWORD:
		uSysStatusFlag &= ~f_DMForward;			
		DRV_Start();
		if(u16_DebugFlag == 3)
			cliPrintf("\n DRV_BackStart");

		break;
		//
	case KEY_LEFT:
		if((uSysStatusFlag & f_LimitLeft) == 0)
		{
			uSysStatusFlag |= f_SMLeft;
			STR_Start();
			if(u16_DebugFlag == 3)
				cliPrintf("\n STR_LeftStart");
		}				
		break;
		//
	case KEY_RIGHT:
		if((uSysStatusFlag & f_LimitRight) == 0)
		{
			uSysStatusFlag &= ~f_SMLeft;
			STR_Start();
			if(u16_DebugFlag == 3)
				cliPrintf("\n STR_RightStart");
		}
		break;
		//
	case KEY_PUMP:		/* PUMP 온 후 1초 뒤 FAN 온 */
		if(is_NotUrgency())
		{
			uSysStatusFlag ^= f_PumpOn;
			if(u16_DebugFlag == 1)
				cliPrintf("\n uSysStatusFlag[%0x]",uSysStatusFlag);
			
			if(uSysStatusFlag & f_PumpOn)
			{
				if(u16_DebugFlag == 1)
					cliPrintf("\n PUMP ON SEQ");

				pn_PumpOn();
			}
			else
			{
				PumpSeq = PUMP_STOP;
				PumpWaittime = 10;
				if(u16_DebugFlag == 1)
					cliPrintf("\n PUMP STOP SEQ");
			}
		}
		break;
		//
	case KEY_FANONUP:	/* FAN ON-1,2,3, 1,2,3, ........ uSysStatusFlag f_FanOn*/
		if(is_NotUrgency())
		{
			if(NotEmpty_WL())
			{
				if((uSysStatusFlag & f_FanACT) == 0)
				{
#if _fan4Level				
					if(FAN_Speed >= 6)
						FAN_Speed = 1;
#else
					if(FAN_Speed >= 4)
						FAN_Speed = 1;
#endif
					//
					if((FAN_Speed == 0) && (LKAUTOSeq == 0))
					//if((FAN_Speed == 0) )
					{
						FAN_Speed = 1;					
					}
					//
					if(FAN_Speed != 0)
					{
						FanSeq = FAN_START;
						uSysStatusFlag |= f_FanACT;
						uSysStatusFlag |= f_FanOn;
						FanWaittime = 10;
						FanTKPWM = FAN_PWM[FAN_Speed];
					}
					else
					{
						FanSeq = FAN_STOP;					
					}
					if(u16_DebugFlag == 1)
						cliPrintf("\n FAN_Start SPEED[%0d],PWM[%0d]",FAN_Speed,FanTKPWM);
				}
				else
				{
					++FAN_Speed;
#if _fan4Level
					if(FAN_Speed >= 6)
						FAN_Speed = 1;
#else
					if(FAN_Speed >= 4)
						FAN_Speed = 1;
#endif					
					//
					FanTKPWM = FAN_PWM[FAN_Speed];
					//
					FanSeq = FAN_SPEED;
					FanWaittime = 10;
					if(u16_DebugFlag == 1)
						cliPrintf("\n FAN_UP SPEED[%0d],PWM[%0d]",FAN_Speed,FanTKPWM);
				}
			}	
			else
			{
				if(u16_DebugFlag == 1)
					cliPrintf("\n !! WL_Empty !!");
			}
		}
		break;
		//
	case KEY_FANOFF:
		if(uSysStatusFlag & f_FanACT)
		{
			uSysStatusFlag &= ~f_FanOn;
			FanSeq = FAN_STOP;
			FanWaittime = 10;
			FAN_Speed = 0;
			if(u16_DebugFlag == 1)
				cliPrintf("\n FAN STOP SEQ. LOGIC 0");
		}
		break;
		//		
	case KEY_AUTORUN:
		if(is_NotUrgency())
		{
			CAN_RX_FND_Error[0] &= ~0x15;
			CAN_RX_FND_Error[1] &= ~0x3F;
			CAN_RX_FND_Error[2] &= ~0x02;
			// 조건을 먼저 판단해야 된다. All Motor Stop 상태에서
			// All Motor Stop 이 아닌 경우 는 ? (일단 걍 둔다.)
			// Start는 Start는 조향, 구동 모터가 정지된 상태에서 입력 받는다. 
			uSysStatusFlag ^= f_AutoRun;
			
			if(uSysStatusFlag & f_WTEmptBuzReq)			// During Buzzer  기대자율떄문이네
				uSysStatusFlag &= ~f_WTEmptBuzReq;		// Stop Buszzer		

			if(u16_DebugFlag == 1)
				cliPrintf("\n KEY_AUTO_RUN[%0x]",(uSysStatusFlag & f_AutoRun));
			if(uSysStatusFlag & f_AutoRun)
			{
				if(u16_DebugFlag == 1)
					cliPrintf("_[%0x]",uSysStatusFlag & f_AutoRun);
			}
			else
			{
				// Status 를 백업해야 한다.  Pump & FAN(Speed)
				if(u16_DebugFlag == 1)
				{
					cliPrintf("_[%0x]",uSysStatusFlag & f_AutoRun);
					cliPrintf(" - Driving AUTO, KEY IN - All Motor Stop");
				
				}
				
				all_MotorStop();
			}
		}			
		break;
		//
	default:
		break;
  }
  //}
}
//
// uSysStatusFlag |= f_smartConnected
// uSysStatusFlag |= f_TraceLine
#if 0
	자율주행 중에는 주행,조향 Key 동작을 안한다.
#endif
void Lkey_Action()
{
#if 0
  printf("\n L_KEY[%0d]",Local_Key.lK_Last);	
#endif
  if((Local_Key.lK_Last & KEY_FORWARD)  == KEY_FORWARD)
  {
	if(isAutoCondition() == 0)	
	{
		uKeyStatusFlag |= flk_Forward;
		uKeyStatusFlag &= ~flk_Backward;
		Drive_REQ(KEY_FORWARD);
	}
  }
  else if((Local_Key.lK_Last & KEY_BACKWORD)  == KEY_BACKWORD)
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag &= ~flk_Forward;
		uKeyStatusFlag |= flk_Backward;
		Drive_REQ(KEY_BACKWORD);
	}
  }
  else
  {
	uKeyStatusFlag &= ~flk_Forward;
	uKeyStatusFlag &= ~flk_Backward;
  }
  //
  if((Local_Key.lK_Last & KEY_LEFT) == KEY_LEFT)
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag |= flk_Left;
		uKeyStatusFlag &= ~flk_Right;
		Drive_REQ(KEY_LEFT);
	}
  }
  else if((Local_Key.lK_Last & KEY_RIGHT) == KEY_RIGHT)
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag &= ~flk_Left;
		uKeyStatusFlag |= flk_Right;
		Drive_REQ(KEY_RIGHT);
	}
  }
  else
  {
	uKeyStatusFlag &= ~flk_Left;
	uKeyStatusFlag &= ~flk_Right;
  }
}
//
void Comm_Action()
{
  if((smt_Command & KEY_FORWARD)  == KEY_FORWARD)
  {
	uKeyStatusFlag |= smt_Forward;
	uKeyStatusFlag &= ~smt_Backward;
	Drive_REQ(KEY_FORWARD);
  }
  else if((smt_Command & KEY_BACKWORD)	== KEY_BACKWORD)
  {
	uKeyStatusFlag &= ~smt_Forward;
	uKeyStatusFlag |= smt_Backward;
	Drive_REQ(KEY_BACKWORD);
  }
  else
  {
	uKeyStatusFlag &= ~smt_Forward;
	uKeyStatusFlag &= ~smt_Backward;
  }
  //
  #if 0 
  if((smt_Command & KEY_LEFT) == KEY_LEFT)
  {
	uKeyStatusFlag |= smt_Left;
	uKeyStatusFlag &= ~smt_Right;
	Drive_REQ(KEY_LEFT);	
  }
  else if((smt_Command & KEY_RIGHT) == KEY_RIGHT)
  {
	uKeyStatusFlag &= ~smt_Left;
	uKeyStatusFlag |= smt_Right;
	Drive_REQ(KEY_RIGHT);   
  }
  else
  {
	uKeyStatusFlag &= ~smt_Left;
	uKeyStatusFlag &= ~smt_Right;
  }
  #endif 
}
uint8_t not_ready_flag = 0;
//
void Rkey_Action()	/* 무선리모컨  */
{
  if(u16_DebugFlag == 1)
  {
	cliPrintf("\n R_KEY[%0x] >> B_ADC[%0d] B-VOLT[%.1f] -- [R_VOL_ADC[%0d] Voltage[%.1f] DRV_SPD[%0d]",RFR_Key.rK_Last,ADC_Battery, V_Battery,ADC_RFRVolume,V_RFRVolume,DRV_Speed);
	cliPrintf("\n Limit_C[%0x] L[%0x] R[%0x]",uSysStatusFlag & f_LimitCenter,uSysStatusFlag & f_LimitLeft,uSysStatusFlag & f_LimitRight);
  }
  //
  if((RFR_Key.rK_Last & RFR_FORWARD)  == RFR_FORWARD)			/* 0x20 */
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag |= frk_Forward;
		uKeyStatusFlag &= ~frk_Backward;
		Drive_REQ(KEY_FORWARD);
	}
  }
  else if((RFR_Key.rK_Last & RFR_BACKWORD)  == RFR_BACKWORD)	/* 0x01 */
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag |= frk_Backward;
		uKeyStatusFlag &= ~frk_Forward;
		Drive_REQ(KEY_BACKWORD); 
	}
  }
  else
  {
	uKeyStatusFlag &= ~frk_Forward;
	uKeyStatusFlag &= ~frk_Backward;
  }
  //
  if((RFR_Key.rK_Last & RFR_LEFT)  == RFR_LEFT)				/* 0x02 */
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag |= frk_Left;
		uKeyStatusFlag &= ~frk_Right;
		Drive_REQ(KEY_LEFT); 
	}
  }
  else if((RFR_Key.rK_Last & RFR_RIGHT)  == RFR_RIGHT)		/* 0x04 */
  {
	if(isAutoCondition() == 0)
	{
		uKeyStatusFlag |= frk_Right;
		uKeyStatusFlag &= ~frk_Left;
		Drive_REQ(KEY_RIGHT); 
	}
  }
  else
  {
	uKeyStatusFlag &= ~frk_Left;
	uKeyStatusFlag &= ~frk_Right;
  }
  //  
  if((RFR_Key.rK_Last & RFR_FANOFF) == RFR_FANOFF)			/* 0x100 --> 0x80 */
  {
	if(u16_DebugFlag == 1)
		cliPrintf("\n FAN_OFF");
	
	uKeyStatusFlag |= frk_FanOff;
	Drive_REQ(KEY_FANOFF);  
  }
  else if((RFR_Key.rK_Last & RFR_FANONUP) == RFR_FANONUP)
  {
	if(u16_DebugFlag == 1)
    	cliPrintf("\n FAN_ON_UP");

	uKeyStatusFlag |= frk_FanOnUp;
	Drive_REQ(KEY_FANONUP);  
  }
  else
  {
	uKeyStatusFlag &= ~frk_FanOnUp;
	uKeyStatusFlag &= ~frk_FanOff;
  }
  // 
  if((RFR_Key.rK_Last & RFR_PUMP) == RFR_PUMP)
  {
	  if(u16_DebugFlag == 1)
		  cliPrintf("\n RFR_PUMP");
	  
		uKeyStatusFlag |= frk_Pump;
		Drive_REQ(KEY_PUMP);  			/*  */
  }
  else
 	uKeyStatusFlag &= ~frk_Pump; 	
  //
  if((RFR_Key.rK_Last & RFR_ATDRV) == RFR_ATDRV)
  {
		

	if (RTK_state == 1)
	{
		if ( !(uSysStatusFlag & a_NotCondition))
		{
			
			if (DRV_Speed  == 1 )
			{
				center_start = 3;
			}  				/* 0x40 */
			else 
			{
				uKeyStatusFlag |= frk_AutoDrv;
				
				Drive_REQ(KEY_AUTORUN);
			}
		}
	}	
	else 
	{	
		uSysStatusFlag |= a_NotCondition;
		uKeyStatusFlag &= ~ frk_AutoDrv;
		uSysStatusFlag &= ~f_AutoRun;
		not_ready_flag |= F_DRV_REQ;
		Bcon_dly = 2700;
	}
		
  }

}
//
/* 동시 KEY 처리 */
uint8_t Key_IdentChk(uint16_t In_LocalKey)
{
  if(((In_LocalKey & KEY_FORWARD) == KEY_FORWARD) && ((In_LocalKey & KEY_BACKWORD) == KEY_BACKWORD))
	return 0;
  //
  if(((In_LocalKey & KEY_LEFT) == KEY_LEFT) && ((In_LocalKey & KEY_RIGHT) == KEY_RIGHT))
	  return 0;
  //	  
  return 1;
}
//
/* */
#if 0
	동시 입력 Check (전진/ 후진, 좌향/ 우향)
#endif
uint8_t Comm_IdentChk(uint16_t In_LocalKey)
{
  if(((In_LocalKey & KEY_FORWARD) == KEY_FORWARD) && ((In_LocalKey & KEY_BACKWORD) == KEY_BACKWORD))
	return 0;
  //
  if(((In_LocalKey & KEY_LEFT) == KEY_LEFT) && ((In_LocalKey & KEY_RIGHT) == KEY_RIGHT))
	return 0;
  //		
  return 1;
}

/*
	79	PC11	IN		RFR_IN_EMERGENCY	10
	80	PC12	IN		RFR_IN_FORWARD		20
	7	PC13	IN		RFR_AUTODRIVE		40
	8	PC14	IN		RFR_FANSPD+ 		80
	9	PC15	IN		RFRFANSPD-		    100

	81	PD0 	IN		RFR_IN_BACKWARD 	1
	82	PD1 	IN		RFR_IN_LEFT 		2
	83	PD2 	IN		RFR_IN_RIGHT		4
	84	PD3 	IN		RFR_IN_PUMP+FAN 	8

	전,후진은 연속 들어온다. ON/Off Toggle로 누르고 잇지 않아도 들어온다.
	좌,우는 연속으로 들어오는데, 누르고 잇어야 들어온다.
	이하 Key 는 연속 처리 하면 안 된다. 
*/
uint8_t RFR_IdentChk(uint16_t In_RFRKey)
{
  if((In_RFRKey & RFR_EMERGENCY) == 0)			/* Main Off 면 무조건..*/
  {
	if(u16_DebugFlag == 1)
		cliPrintf("\n KEY_Rtn_1");
	return KEY_NONE;
  }
  //
  // 동시 Key 만 막는다...
  if(((In_RFRKey & RFR_FORWARD) == RFR_FORWARD) && ((In_RFRKey & RFR_BACKWORD) == RFR_BACKWORD))
  	{
		if(u16_DebugFlag == 1)
	  		cliPrintf("\n KEY_Rtn_2");
	  	return KEY_NONE;	  
  	}
  //
  
  if(((In_RFRKey & RFR_LEFT) == RFR_LEFT) && ((In_RFRKey & RFR_RIGHT) == RFR_RIGHT))
  	{
		if(u16_DebugFlag == 1)
	  		cliPrintf("\n KEY_Rtn_3");
	  	return KEY_NONE;	  
  	}
  //
  if(((In_RFRKey & RFR_PUMP) == RFR_PUMP) || ((In_RFRKey & RFR_FANONUP) == RFR_FANONUP) || ((In_RFRKey & RFR_FANOFF) == RFR_FANOFF) || ((In_RFRKey & RFR_ATDRV) == RFR_ATDRV))
  {	/* 단독 KEY, 연속 Key 처리를 막는다. */

	if(uKeyStatusFlag & fRK_Repeat)
	{
		 return KEY_NONE;
	}
	else
		return 1;	
  }
  else
	  return 1;   
}

//
/*
97	PE0 	OUT 		LED R
98	PE1 	OUT 		LED G
1	PE2 	IN		KEY_EMERGENCY	3.3V 10K FU Ready

2	PE3 	IN		KEY_FORWARD 	(내부 F/U 사용 ?)	1
3	PE4 	IN		KEY_BACKWORD					2
4	PE5 	IN		KEY_LEFT						4
5	PE6 	IN		KEY_RIGHT						8
5	PE7 	IN		KEY_RIGHT						8
5	PE8 	IN		KEY_RIGHT						8

*/
void LKEY_Check()		/* Local 4 Key */
{
	  uint16_t L_KeyBuff;
	
	  L_KeyBuff = (GPIOE->IDR >> 3) & 0x000F;
	  if(L_KeyBuff != 0 && Key_IdentChk(L_KeyBuff)) 	/* 동시 Key 처리 --> 막는다. */
	  {
		Local_Key.lK_Data = L_KeyBuff;
		if(Local_Key.lK_Data == Local_Key.lK_Back)
		{
			if(Local_Key.lK_ChatCnt == 0)
			{
				uKeyStatusFlag |= fLK_Ok;
				uKeyStatusFlag &= ~fLK_Repeat;
				if(Local_Key.lK_Data == Local_Key.lK_Last)
					uKeyStatusFlag |= fLK_Repeat;
				Local_Key.lK_Last = Local_Key.lK_Data;
				//	
				Local_Key.lK_ChatCnt = 30;		/* 매 30ms 마다 */
				RK_PermitTime = 3000/30;		/* 3초 뒤 R Key Check */
				
				if(uSysStatusFlag & f_AutoRun)		/* 자율 주행중, 유효한 Key 가 들어오면 */
				{									/* 자울 주행을 중지한다. */
					all_MotorStop();
				}
				else					
					Lkey_Action();					
			}
		} 
		else 
		{
			Local_Key.lK_Back = Local_Key.lK_Data;
			Local_Key.lK_ChatCnt = 60;
			uKeyStatusFlag &= ~fLK_Ok;
			uKeyStatusFlag &= ~fLK_Repeat;
		}
	  }
	  else
	  {
		//LK_FlagClear();
		uKeyStatusFlag &= ~fLK_Ok;
		uKeyStatusFlag &= ~fLK_Repeat;
		Local_Key.lK_Data = 0;
		Local_Key.lK_Back = 0;
		Local_Key.lK_Last = 0;
		Local_Key.lK_ChatCnt = 60;
	  }
}

//
#if 0
#define GPO_RFRmain		
	79	PC11	IN		RFR_IN_EMERGENCY
	80	PC12	IN		RFR_IN_FORWARD
	7	PC13	IN		RFR_AUTODRIVE
	8	PC14	IN		RFR_FANSPD+ 
	9	PC15	IN		RFRFANSPD-

	81	PD0 	IN		RFR_IN_BACKWARD
	82	PD1 	IN		RFR_IN_LEFT
	83	PD2 	IN		RFR_IN_RIGHT
	84	PD3 	IN		RFR_IN_PUMP+FAN
	//
	RFR_IN_BACKWARD		0x01
	RFR_IN_LEFT			0x02
	RFR_IN_RIGHT		0x04
	RFR_IN_PUMP+FAN		0x08

	RFR_IN_EMERGENCY	0x10
	RFR_IN_FORWARD		0x20
	RFR_AUTODRIVE		0x40
	RFR_FANSPD+ 		0x80
	RFRFANSPD-			0x0100
#endif
#if 0
uint8_t DRVSTR_Key()
{
  if(RFR_Key.rK_Data == 0x101)			// For 
  	return 1;
  else if(RFR_Key.rK_Data == 0x110)		// Back
  	return 1;	
  else if(RFR_Key.rK_Data == 0x120)		// Left
  	return 1;
  else if(RFR_Key.rK_Data == 0x140)		// Right 
  	return 1;
  else
  	return 0;	/* FANON 104, FAN)FF 108, AUTO  102, OUMP 180 */
}
#endif
void RKEY_Check()
{
	  uint16_t PC_rKey, PD_rKey, R_KeyBuff;
	
	  PC_rKey = (GPIOC->IDR & 0xF000) >> 12;
	  PD_rKey = (GPIOD->IDR & 0x001F) << 4;
	  R_KeyBuff = PC_rKey | PD_rKey;
	  R_KeyBuff &= 0x01FF;
#if 0	
	  printf("\n RFR_Port_Read[%0x]",R_KeyBuff);
#endif	
	  if(R_KeyBuff & 0x00FF)		/* RFR_MAIN 제외  */
	  {
		RFR_Key.rK_Data = R_KeyBuff;
			
		if(RFR_Key.rK_Data == RFR_Key.rK_Back)
		{
			if(RFR_Key.rK_ChatCnt == 0)
			{
				/* uKeyStatusFlag fLK_Ok */
				uKeyStatusFlag |= fRK_Ok;
				uKeyStatusFlag &= ~fRK_Repeat;
				if(RFR_Key.rK_Data == RFR_Key.rK_Last)
					uKeyStatusFlag |= fRK_Repeat;
				//		
				RFR_Key.rK_Last = RFR_Key.rK_Data;
				RFR_Key.rK_Back = RFR_Key.rK_Data;
				RFR_Key.rK_ChatCnt = 10;			/* 연속키 처리 타임  매 10ms 마다 */
				if(u16_DebugFlag == 1)
					cliPrintf(" R_KEY[%0x]\r\n",RFR_Key.rK_Back);
				if(((RFR_Key.rK_Last & RFR_PUMP) == RFR_PUMP) || ((RFR_Key.rK_Last & RFR_FANONUP) == RFR_FANONUP) || ((RFR_Key.rK_Last & RFR_ATDRV) == RFR_ATDRV) || ((RFR_Key.rK_Last & RFR_FANOFF) == RFR_FANOFF)) 
				{
					if((uKeyStatusFlag & fRK_Repeat) == 0)
						Rkey_Action();
					//
				}
				else
				{
					Rkey_Action();
				}
			}
		} 
		else 
		{
			RFR_Key.rK_Back = RFR_Key.rK_Data;
			RFR_Key.rK_ChatCnt = 100;
			uKeyStatusFlag &= ~fRK_Ok;
			uKeyStatusFlag &= ~fRK_Repeat;
		}
	  }
	  else
	  {
		uKeyStatusFlag &= ~fRK_Ok;
		uKeyStatusFlag &= ~fRK_Repeat;
		RFR_Key.rK_Data = 0;
		RFR_Key.rK_Back = 0;
		RFR_Key.rK_Last = 0;
		RFR_Key.rK_ChatCnt = 100;
	  }
}

//
#if 0
	b[0] 	: Forward
	b[1]	: Backward
	b[2]	: Left
	b[3]	: Right
	..
	b[7]	: 주행 Line 감지 정보( 1 : 주행Line 감지)
	..
#endif
uint8_t isAutoCondition()
{
  //if((uSysStatusFlag & f_AutoRun) && (uSysStatusFlag & f_smartConnected) && (uSysStatusFlag  & f_TraceLine) && (smt_Command & 0x0080))
  if(uSysStatusFlag & f_AutoRun)
	return 1;
  else		
	return 0;
}

void SMART_check()
{
  uint16_t S_Comm;
  
  if(isAutoCondition())
  {
	S_Comm = smt_Command & 0x000F;
	if(S_Comm != 0 && Comm_IdentChk(S_Comm) )	/* For/Back , Left/Right 동시 Comm. Check */
	{
		Comm_Action();
	}

  }

}
//
#if 1
void LKAUTO_CHKSeq()
{
  switch(LKAUTOSeq)
  {
  	case 1:		/* 조건 Check 수위 有, 자율 Line 신호 有 */ 
		if((uSysStatusFlag & f_ALLWLEmpty) == 0)
		{	/* 수위 有 */
			if(uSysStatusFlag & f_TraceLine)
			{	/* 자율 Line 신호 有*/
				LKAUTOSeq = 2;
				//printf("\n SEQ_2");
			}
		}
		break;
		//
	case 2:
		if(uSysStatusFlag & f_LKATKInitStatus)
		{ /* Init E_OPT_Autospeed Port High Status */
			if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 0)
			{
				HAL_Delay(100);
				if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 0)
					LKAUTOSeq = 3;
				//
				
				//printf("\n SEQ_3");
				//printf(" -- INIT_REVERSE[%0d]",LKAUTOSeq);
			}
		}
		else
		{ /* Init E_OPT_Autospeed Port Low Status Status */
			if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
			{
				HAL_Delay(100);
				if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
					LKAUTOSeq = 3;
				//
				//printf("\n SEQ_3");
				//printf(" -- INIT_REVERSE[%0d]",LKAUTOSeq);
			}
		}
		break;
		//
	case 3:
		if(uSysStatusFlag & f_LKATKInitStatus)
		{
			if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
			{
				HAL_Delay(100);
				if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
				{
					LKAUTOSeq = 4;		/* SEQ. Start */
					LKSeqDlyTime = 2000;

					//printf("\n SEQ_4");
					//printf(" -- INIT_ORIGIN[%0d]",LKAUTOSeq);
				}
			}
		}
		else
		{
			if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 0)
			{
				HAL_Delay(100);
				if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 0)
				{
					LKAUTOSeq = 4;		/* SEQ. Start */	
					LKSeqDlyTime = 2000;
					
					//printf("\n SEQ_4");
					//printf(" -- INIT_ORIGIN[%0d]",LKAUTOSeq);
				}
			}
		}
		break;
		//
	case 4:		/* Delay 2 Sec. */	
		if(LKSeqDlyTime == 0)
		{
			Drive_REQ(KEY_FANONUP);
			LKAUTOSeq = 5;
			LKSeqDlyTime = 3000;
			//printf("\n SEQ_FAN ONUP[%0d]",LKAUTOSeq);
		}
		break;
		//
	case 5:
		if(LKSeqDlyTime == 0)
		{
			Drive_REQ(KEY_PUMP);
			LKAUTOSeq = 6;
			LKSeqDlyTime = 3000;
			//printf("\n SEQ_PUMP[%0d]",LKAUTOSeq);
		}
		break;
		//
	case 6:
		if(LKSeqDlyTime == 0)
		{
			Drive_REQ(KEY_AUTORUN);
			LKAUTOSeq = 7;
			LKSeqDlyTime = 3000;
			//printf("\n SEQ_AUTORUN[%0d]",LKAUTOSeq);
		}
		break;
		//
	case 7:
		if(LKSeqDlyTime == 0)
		{
			LKAUTOSeq = 8;
			//printf("\n SEQ_END AUTO RUN[%0d]",LKAUTOSeq);
		}
		break;
		//
	case 8:
		break;
		//
	default:
		break;
  }
}
#endif
//
void RFR_MainCheck()
{
#if 1
  if(uSysStatusFlag & f_AutoRun)
  {	/* 자율 주행시 MN check 안함 */
	uKeyStatusFlag |= frk_Emergency;
	uSysStatusFlag &= ~f_RFR_Emergency;
  }
  else
  {
#endif  
	if(HAL_GPIO_ReadPin(GPIOD,IR_Main) == 1)
	{
		//u16_RFMNChkCnt = 1000;	  /* 200ms */
		u16_RFMNChkCnt = 2000;	  /* 200ms */
		
	 	uKeyStatusFlag |= frk_Emergency;
		uSysStatusFlag &= ~f_RFR_Emergency;
#if 1
		if(LKAUTOSeq >= 4)
		{
			all_MotorStop();
			if(u16_DebugFlag == 1)
				cliPrintf("\n LK_AUTO_SEQ STOP.");
		}
		LKAUTOSeq = 0; // 경운수정 24/2/1
#endif
	}
	else
	{
		if(u16_RFMNChkCnt == 0)
		{
			if((uSysStatusFlag & f_RFR_Emergency) == 0)
			{	/* 비상 정지 */
				uKeyStatusFlag &= ~frk_Emergency;
				uSysStatusFlag |= f_RFR_Emergency;
#if 0				
				all_MotorStop();
#else
				if(LKAUTOSeq == 0)
				{
					all_MotorStop();
					
					if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
					{
						HAL_Delay(100);
						if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 1)
						{
							uSysStatusFlag |= f_LKATKInitStatus;
							LKAUTOSeq = 1;
							if(u16_DebugFlag == 1)
								cliPrintf("\n SEQ_1");
						}
					}
					else
					{
						HAL_Delay(100);
						if(HAL_GPIO_ReadPin(GPIOE,E_OPT_Autospeed) == 0)
						{
							uSysStatusFlag &= ~f_LKATKInitStatus;
							LKAUTOSeq = 1;
							if(u16_DebugFlag == 1)
								cliPrintf("\n SEQ_1");
						}
					}
				}
#endif
			}
		
			uKeyStatusFlag	&= ~frk_AutoDrv;
			uKeyStatusFlag &= ~fRK_Ok;
			uKeyStatusFlag &= ~fRK_Repeat;
		}
  	}
  }
}

//
void User_Interface()
{
  Port_ReadACT();
  //
  LKEY_Check();		/* Low BAT. 상태에서도 Local Key 는 동작 한다. */
  #if 0
  RFR_MainCheck();
  RKEY_Check();
  #endif 
  
  if((uSysStatusFlag & f_LowBat) == 0)
  {
	if((uKeyStatusFlag & fLK_Ok) == 0)	/* Local Key 가 릴리지 되고 */
	{
		if(RK_PermitTime == 0)			/* 3초 이후부터 RFR을 입력 받는다.*/ 
		{
			#if 1 
			RFR_MainCheck();
			if((uSysStatusFlag & f_RFR_Emergency) == 0)
			{
				RKEY_Check();
				SMART_check();
			}
			else
			{
				LKAUTO_CHKSeq();
				SMART_check();
			}
			#endif 
		}
	}
  }
}
//


