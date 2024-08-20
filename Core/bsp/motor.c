#include "motor.h"
#include "keyscan.h"
#include "cli.h"
#include "indicate.h"
#include "bsp_adc.h"
#include "task.h"
#include "dac.h"
#include "hw_config.h"

extern DAC_HandleTypeDef hdac;

uint8_t DRV_SPDTBNo = 0;
#if _CURTIS
const unsigned char DRV_ADC[][6]= 
{
  /* CURTIS 주행 드라이버   */
  0,90,115,130,140,155,			/* 평소 주행속도 */
  /* DMCS 주행 드라이버   */
  1,90,105,120,130,145,			/* 평소 주행속도  */
};
#else
const unsigned char DRV_ADC[][6]= {
	0,93,100,108,116,124,
	1,94,109,124,139,152,
	2,94,100,124,139,152,
	3,98,110,124,139,152,
	4,102,114,124,139,152,
	5,105,118,124,139,152,
	9,105,115,130,135,145,
};
#endif
//
const unsigned char STR_ADC[3] = {
	250,250,250,
};
//
#if _fan4Level
const unsigned char FAN_PWM[6] = {
	//0,76,82,88,94, 	/* 2.2/ 2.4/ 2.6/ 2.8V about */
	0,70,76,82,88,94,	/* 2.2/ 2.4/ 2.6/ 2.8V about */
};
#else
const unsigned char FAN_PWM[4] = {
	0,82,88,94, 		/* 2.4/ 2.6/ 2.8V about */
};
#endif

/* functions  */
// KEY_FORWARD 	PE3
// KEY_REVERSE		PE4
// RFR_FORWARD		PC12
// RFR_BACKWORD	PD0
uint8_t is_DriveKey()
{
  if(((Local_Key.lK_Last & KEY_FORWARD) == KEY_FORWARD) || ((Local_Key.lK_Last & KEY_BACKWORD) == KEY_BACKWORD))
	return 1;
  else
  {
    if(((RFR_Key.rK_Last & RFR_FORWARD) == RFR_FORWARD) || ((RFR_Key.rK_Last & RFR_BACKWORD) == RFR_BACKWORD))
      return 1;
    else
    {
      if(uSysStatusFlag & f_AutoRun)
      {
        if(((smt_Command & KEY_FORWARD)  == KEY_FORWARD) || ((smt_Command & KEY_BACKWORD)  == KEY_BACKWORD))
          return 1;
        else
          return 0;
      }
      else
        return 0;
    }
  }
}
//
void DM_Stop()
{
  if(DMotorSeq != DMOTOR_STOP)
  {
    DMotorSeq = DMOTOR_STOP;
    DMotorTKPWM = 0;
    DMotorWaitime = 10;
  }
}

void DRV_Control()
{
  if((uSysStatusFlag & f_DMotorACT) && (is_DriveKey() == 0))
  {
  	DM_Stop();
  }
  switch(DMotorSeq)
  {
	case DMOTOR_START:
		if(DMotorWaitime == 0)
		{
			if(uSysStatusFlag & f_DMForward)
      {
        HAL_GPIO_WritePin(GPIOE, E_drvM_CWCCW, GPIO_PIN_SET);
      }
			else
      {
        HAL_GPIO_WritePin(GPIOE, E_drvM_CWCCW, GPIO_PIN_RESET);
      }
			DMotorWaitime = 2;
			DMotorSeq = DMOTOR_BRKOFF;
		}
		break;
		//
	case DMOTOR_BRKOFF:		/* Brake Switch ON=+24V,OFF=OPEN */
		if(DMotorWaitime == 0)
		{
      HAL_GPIO_WritePin(GPIOE, E_drvM_BREAK, GPIO_PIN_RESET);
			uSysStatusFlag &= ~f_DMBreak;
			DMotorWaitime = 2;
			DMotorSeq = DMOTOR_ACCEL;
		}
		break;
		
	case DMOTOR_ACCEL:
		if(DMotorWaitime == 0)
		{
			DMotorTKPWM = DRV_ADC[DRV_SPDTBNo][DRV_Speed]+load_value.IDLE_SPD;
			//
			if((uSysStatusFlag & f_LimitCenter) == 0)
			{
				if(DRV_Speed >= 4)
					DMotorTKPWM = DRV_ADC[DRV_SPDTBNo][DRV_Speed-1]+load_value.IDLE_SPD;			// 센터가 아닐때 속도 낮추는부분		
			}
			if((uSysStatusFlag & f_DMForward) == 0)		/* Backward */
			{
				if(DRV_Speed >= 3)						/* 3,4,5, 단 스피드는 3단으로한다 */
					DRV_Speed = 3;
				DMotorTKPWM = DRV_ADC[DRV_SPDTBNo][DRV_Speed]+load_value.IDLE_SPD; 
			}
#if _SMT_DMSpeed			
			if(isAutoCondition())
			{
        if ( smt_DRVSpeed == 3) //3일때 고속
        {
          if ( DRV_SPDTBNo == 1) // DMCS type
          {
            DMotorTKPWM = load_value.ATD_HIGH_SPD+12;
          }
          else  // Curtis type
          {
            DMotorTKPWM = load_value.ATD_HIGH_SPD;
          }
        }
        else // 2일때 저속  
        {
          if ( DRV_SPDTBNo == 1) // DMCS type
          {
            DMotorTKPWM = load_value.ATD_LOW_SPD+12;
          }
          else  // Curtis type
          {
            DMotorTKPWM = load_value.ATD_LOW_SPD;
          }
          
        }
				 
			}
      #endif
			if(uKeyStatusFlag & fLK_Ok)	/* 달려있는 키로 구동할 경우 */
			{
				DMotorTKPWM = 105;	
			}
			DMotorWaitime = 2;
			DMotorSeq = DMOTOR_ACCELACT;
		}
		break;
		//
	case DMOTOR_ACCELACT:
		if(DMotorWaitime == 0)
		{
			if(DMotorTKPWM == DRV_PWMDuty)
			{
				DMotorSeq = DMOTOR_ACCEL;		/* RFR �� ���? �ӵ� ���� ó���� �ؾ� �ȴ�. */
			}
			else if(DMotorTKPWM > DRV_PWMDuty)
			{
				++DRV_PWMDuty;

			}
			else if(DMotorTKPWM < DRV_PWMDuty)
			{
				--DRV_PWMDuty;
			}

			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, DRV_PWMDuty);
      DMotorWaitime = 3;
		}
		break;
		//
	case DMOTOR_STOP:		/* Stop Seq.*/
		if(DMotorWaitime == 0)
		{
			if(DRV_PWMDuty > 0)
			{
				--DRV_PWMDuty;
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, DRV_PWMDuty);
				DMotorWaitime = 10;
			}
			else
			{
				DRV_PWMDuty = 0;
				DMotorTKPWM = 0;
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, DRV_PWMDuty);
        HAL_GPIO_WritePin(GPIOE, E_drvM_BREAK, GPIO_PIN_SET);
				uSysStatusFlag |= f_DMBreak;
				uSysStatusFlag &= ~f_DMotorACT;
				DMotorSeq = DMOTOR_END;
			}
		}
		break;
		//
	case DMOTOR_END:		
	default:		
		
		break;
  }
}
//
uint8_t is_StrringKey()
{
  if(((Local_Key.lK_Last & KEY_LEFT) == KEY_LEFT) || ((Local_Key.lK_Last & KEY_RIGHT) == KEY_RIGHT))
	return 1;
  else
  {
    if(((RFR_Key.rK_Last & RFR_LEFT) == RFR_LEFT) || ((RFR_Key.rK_Last & RFR_RIGHT) == RFR_RIGHT))
      return 1;
    else
    {
      if(uSysStatusFlag & f_AutoRun)
      {
        if(((smt_Command & KEY_LEFT)  == KEY_LEFT) || ((smt_Command & KEY_RIGHT)  == KEY_RIGHT))
          return 1;
        else
          return 0;
      }
      else
        return 0;
    }
  }
}
//
void SM_Stop()
{
  if(SMotorSeq != SMOTOR_STOP)
  {
    SMotorSeq = SMOTOR_STOP;
    SMotorTKPWM = 0;
    SMotorWaitime = 10;
  }
}
#define YES_PUMP 1
// SMotorSeq
void STR_Control()
{
  if((uSysStatusFlag & f_SMotorACT) && (is_StrringKey() == 0))
  {
	  SM_Stop();  
  }
  
  switch(SMotorSeq)
  {
    case SMOTOR_START:
    {
      Port_ReadACT();
      if(SMotorWaitime == 0)
      {
        if(uSysStatusFlag & f_SMLeft)
        {
          HAL_GPIO_WritePin(GPIOE, E_strM_CWCCW, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIOE, E_strM_BREAK, GPIO_PIN_SET);
        }
        else
        {
          HAL_GPIO_WritePin(GPIOE, E_strM_CWCCW, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIOE, E_strM_BREAK, GPIO_PIN_RESET);
        }
        SMotorWaitime = 0;
        SMotorSeq = SMOTOR_ACCEL;
      }
    }
    break;
    case SMOTOR_ACCEL:
    {
      if(SMotorWaitime == 0)
      {
        if(isAutoCondition())
          SMotorTKPWM = STR_ADC[2];
        else
          SMotorTKPWM = 200;
          
        SMotorWaitime = 0;
        SMotorSeq++;
      }
    }
    break;
      //
    case SMOTOR_ACCELACT:
    {
      if(SMotorWaitime == 0)
      {
        STR_PWMDuty = SMotorTKPWM;			
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, STR_PWMDuty);
        SMotorSeq = SMOTOR_END;
      }
    }
    break;
    case SMOTOR_STOP:		/* Stop Seq.*/
    {
      if(SMotorWaitime == 0)
      {
        SMotorTKPWM = 0;
        STR_PWMDuty = 0;
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, STR_PWMDuty);
        uSysStatusFlag &= ~f_SMotorACT;
        SMotorSeq = SMOTOR_END;
      }
    }
    break;
      //
    case SMOTOR_END:		
    default:		
      break;
  }
}
//
//
//#define YES_PUMP 0
void PUMP_Control()
{
  if(PumpWaittime == 0)
  {
    switch(PumpSeq)
    {
      
      case PUMP_START:
      {
        if( load_value.Spray_type == 1)
        {
          HAL_GPIO_WritePin(GPIOB, B_PMPDN, GPIO_PIN_SET);
          PumpWaittime = 2000;			
        }
        PumpSeq = PUMP_SOL_ON;
      }
      break;
      case PUMP_SOL_ON:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPON, GPIO_PIN_SET);
        #endif
        PumpWaittime = 200;			
        PumpSeq = PUMP_SRTH;
      }
      break;

      case PUMP_SRTH:
      {
        #ifdef YES_PUMP 
        HAL_GPIO_WritePin(GPIOB, B_PMPON, GPIO_PIN_RESET);
        #endif 
        #if _PUMP_P			
        cliPrintf(" ON_H"); 
        #endif			
        PumpWaittime = 200;
        if(uSysStatusFlag & f_Pump1st)
        {
          uSysStatusFlag &= ~f_Pump1st;			
          PumpSeq = PUMP_UP1L;
          #if _PUMP_P
          cliPrintf(" - PUMP UP");
          #endif			
        }
        else
        {
          PumpWaittime = 10;
          PumpSeq = PUMP_Stay;
        }
      }
      break;
    
      case PUMP_UP1L:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_UP1H;
        #if _PUMP_P			
        cliPrintf(" UL1"); 
        #endif			
      } 		
      break;

      case PUMP_UP1H:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
        #endif
        PumpWaittime = 200;
        PumpSeq = PUMP_UP2L;
        #if _PUMP_P			
        cliPrintf(" UH1"); 
        #endif
      } 		
      break;

      case PUMP_UP2L: 		
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_UP2H;
        #if _PUMP_P			
        cliPrintf(" UL2"); 
        #endif
      }
      break;

      case PUMP_UP2H: 		
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
        #endif
        PumpWaittime = 200;
        PumpSeq = PUMP_UP3L;
        #if _PUMP_P			
        cliPrintf(" UH2"); 
        #endif
      }
      break;

      case PUMP_UP3L:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_UP3H;
        #if _PUMP_P			
        cliPrintf(" UL3"); 
        #endif
      } 		
              
        break;
        //
      case PUMP_UP3H:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
        #endif
        PumpWaittime = 200;
        #if _PUMP_P			
        cliPrintf(" UH3"); 
        #endif	
        if(uSysStatusFlag & f_OPT_Pumpspeed)
        {
          PumpSeq = PUMP_UP4L;				/* High �л緮 5�� */
        }
        #if 1 
        else
        {
          PumpSeq = PUMP_Stay;
          cliPrintf(" - PUP END_3 Level"); 
        }
        #endif 
      } 		
      break;
        
      case PUMP_UP4L: 		
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_UP4H;
        #if _PUMP_P			
        cliPrintf(" UL4"); 
        #endif
      }
      break;

      case PUMP_UP4H: 
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
        #endif
        PumpWaittime = 200;
        PumpSeq = PUMP_UP5L;
        #if _PUMP_P			
        cliPrintf(" UH4"); 
        #endif
      }		
      break;

      case PUMP_UP5L:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_UP5H;
        #if _PUMP_P			
        cliPrintf(" UL5"); 
        #endif
      } 		
      break;

      case PUMP_UP5H: 		
      {
        #ifdef YES_PUMP
    	  HAL_GPIO_WritePin(GPIOB, B_PMPUP, GPIO_PIN_RESET);
        #endif
        PumpWaittime = 200;
        PumpSeq = PUMP_Stay;
        #if _PUMP_P			
        cliPrintf(" UH5 - PUP END"); 
        #endif
      }
      break;

      case PUMP_Stay:
      {
        
      }
      break;

      case PUMP_STOP:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPOFF, GPIO_PIN_SET);
        #endif
        PumpWaittime = 80;
        PumpSeq = PUMP_STOP1H;
        #if _PUMP_P
        cliPrintf("\n PUMP STOP_L");
        #endif			
      }
      break;

      case PUMP_STOP1H:
      {
        #ifdef YES_PUMP
        HAL_GPIO_WritePin(GPIOB, B_PMPOFF, GPIO_PIN_RESET);
        #endif

        if( load_value.Spray_type == 1)
        {
          HAL_GPIO_WritePin(GPIOB, B_PMPDN, GPIO_PIN_RESET);
        }
        PumpWaittime = 10;
        uSysStatusFlag &= ~f_PumpACT;
        uSysStatusFlag &= ~f_PumpOn;		
        PumpSeq	 = PUMP_Stay;
        #if _PUMP_P
        cliPrintf(" PUMP STOP_H, STOP SEQ END,f_PumpACT[%0d]",uSysStatusFlag & f_PumpACT);
        #endif
      }
      break;

      default:
      break;

    }
  }
}

void FAN_Control()
{
  if(FanWaittime == 0)
  {
    switch(FanSeq)
    {
      case FAN_START:
      {
        //FAN_Speed = 1;
        FanTKPWM = FAN_PWM[FAN_Speed];
        FanSeq = FAN_SPEED;
        FanWaittime = 10;
        #if _FAN_P
        cliPrintf("\n FAN_START SPD[%0d] TKT_PWM[%0d],Real_PWM[%0d]",FAN_Speed,FanTKPWM,FAN_PWMDuty);
        #endif
      } 
      break;

      case FAN_SPEED:
      {
        if(FanTKPWM > FAN_PWMDuty)
        {
          ++FAN_PWMDuty;
          #if _FAN_P
          cliPrintf("\n FAN_DUTY UP TKT_PWM[%0d],Real_PWM[%0d]",FanTKPWM,FAN_PWMDuty);
          #endif
        }
        else if(FanTKPWM < FAN_PWMDuty)
        {
          --FAN_PWMDuty;
          #if _FAN_P
          cliPrintf("\n FAN_DUTY DOWN TKT_PWM[%0d],Real_PWM[%0d]",FanTKPWM,FAN_PWMDuty);
          #endif
        }
        else
        {
          FanSeq = FAN_Stay;
          FAN_PWMDuty = FanTKPWM;
          #if _FAN_P
          cliPrintf("\n FAN_DUTY SAME TKT_PWM[%0d],Real_PWM[%0d]",FanTKPWM,FAN_PWMDuty);
          #endif
          
        }
        TIM4->CCR4 = (FAN_PWMDuty*10);
        FanWaittime = 10;
      }
      break;
      
      case FAN_Stay:
      break;

      case FAN_STOP:
      {
        if(FAN_PWMDuty)
        {
          --FAN_PWMDuty;
        }
        else
        {
          FanTKPWM = 0;		
          FAN_PWMDuty = 0;
          uSysStatusFlag &= ~f_FanACT;
          FanSeq = FAN_Stay;
        }
        TIM4->CCR4 = (FAN_PWMDuty*10);
        FanWaittime = 10;
      }
      break;

    }
  }
}
//
void pn_PumpOn()
{
#if _PUMP_P
  if(NotEmpty_WL())
  {
    if(uSysStatusFlag & f_WLEmpty1)	
    {
      cliPrintf("\n Rear_Empty");
    }
    else if(uSysStatusFlag & f_WLEmpty2)	  
    {
      cliPrintf("\n BB_Empty");
    }
    else
    {
      cliPrintf("\n ALL_WT_FULL");
    }
  }
  else
  {
  	cliPrintf("\n ALL_Water_Tank_Empty");
  }
#endif
  if(NotEmpty_WL())
  {
    if((uSysStatusFlag & f_PumpACT) == 0)
    {
      PumpSeq = PUMP_START;
      uSysStatusFlag |= f_PumpACT;
      uSysStatusFlag |= f_PumpOn;
      PumpWaittime = 10;
  #if _PUMP_P
      cliPrintf("\n PUMP START-");
  #endif		
    }
  }
}
//
void all_MotorStop()
{	
  if(u16_DebugFlag == 1)
  {
    cliPrintf("\n all_MotorStop");
    if(uSysStatusFlag & f_LowBat)			cliPrintf(" -- LOW BATTERY");
    if(uSysStatusFlag & f_Crash)			cliPrintf(" -- CRASH");
    if(uSysStatusFlag & f_RFR_Emergency)	cliPrintf(" -- RFR_Emergency");
    if(uSysStatusFlag & f_ADRVnoWaterStop)	  cliPrintf(" -- NO Water During Auto Drive");
    if(uSysStatusFlag & f_ADRVnoCANData)
    {
      if(f_commCAN)
      {
        cliPrintf(" -- NO CAN Comm.");
      }
      else
      {
        cliPrintf(" -- NO UART Comm.");
      }
    }
    if(uCommStatusFlag & f_U2DEErr)		cliPrintf(" -- SMT DE Error");
    if(uCommStatusFlag & f_U2CSumErr)		cliPrintf(" -- SMT Csum Error");
    if(uCommStatusFlag & f_U2INTNon)		cliPrintf(" -- SMT INT None");
    
  }
  
  
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0);
  if(DRV_PWMDuty)
  {
    if(uSysStatusFlag & f_Crash)
    {
      HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0);
    }
    else
    {
      do
      {
        --DRV_PWMDuty;
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, DRV_PWMDuty);
        HAL_Delay(10);
      }while(DRV_PWMDuty);	
    }
  }
  
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0);
  HAL_GPIO_WritePin(GPIOE, E_drvM_BREAK, GPIO_PIN_RESET);
  uSysStatusFlag |= f_DMBreak;

  DRV_PWMDuty = 0;
  STR_PWMDuty = 0;
  DMotorTKPWM = 0;  	
  SMotorTKPWM = 0;
  DMotorSeq = DMOTOR_STOP;
  SMotorSeq = SMOTOR_STOP;

  if(LKAUTOSeq)
  {
    LKAUTOSeq = 0;  	
  }

  if(uSysStatusFlag & f_PumpACT)
  {
	  PumpSeq = PUMP_STOP;
  	PumpWaittime  = 10;
  }
  
  if(uSysStatusFlag & f_FanACT)
  {
	  if(FanSeq != FAN_STOP)
	  {
      FanTKPWM = 0; 	  
      FAN_PWMDuty = 0;
      TIM4->CCR4 = FAN_PWMDuty;
      FanSeq = FAN_STOP;
      FanWaittime = 10;
	  }
  }

  uSysStatusFlag &= ~f_DMotorACT;
  uSysStatusFlag &= ~f_SMotorACT;
  if(uSysStatusFlag & f_AutoRun)
  {
	  uSysStatusFlag &= ~f_AutoRun;
    if(u16_DebugFlag == 1)
	    cliPrintf("\n Auto Reset");
  }
  Bcon_Control();
}
void str_MotorLimitStop()
{
  SMotorTKPWM = 0;
  STR_PWMDuty = 0;
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, STR_PWMDuty);
  uSysStatusFlag &= ~f_SMotorACT;
  SMotorSeq = SMOTOR_STOP;

}
//
void Init_DrvPortSet()
{
  HAL_GPIO_WritePin(GPIOE, E_drvM_BREAK, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, E_drvM_CWCCW, GPIO_PIN_SET);
  uSysStatusFlag |= f_DMBreak;
  uSysStatusFlag |= f_DMForward;
  HAL_GPIO_WritePin(GPIOE, E_strM_CWCCW, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, E_strM_BREAK, GPIO_PIN_RESET);
  uSysStatusFlag |= f_SMLeft;
}
//

void Motor_Control()
{
  
  if(uSysStatusFlag & f_DMotorACT)	DRV_Control();
  if((uSysStatusFlag & f_Crash) == 0)	
  {
    if(uSysStatusFlag & f_SMotorACT)	STR_Control();
    if(uSysStatusFlag & f_PumpACT)		PUMP_Control();
    if(uSysStatusFlag & f_FanACT)		FAN_Control();
  }
}
/*-------------------------------------------------------------------------*/


