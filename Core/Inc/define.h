
#ifndef __BSP_DEFINE_H
#define __BSP_DEFINE_H


#include "main.h"
#include "stm32f103xe.h"
#include "def.h"



#define TRUE 1
#define FALSE 0

#define C_NRML "\033[0m"
#define C_BLCK "\033[30m"
#define C_RED  "\033[31m"
#define C_GREN "\033[32m"
#define C_YLLW "\033[33m"
#define C_BLUE "\033[34m"
#define C_PRPL "\033[35m"
#define C_AQUA "\033[36m"
#define C_END  "\033[0m"





//* mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

/* Define */
//#define _spdDUDCJS		0	/* ��õ ���� 3�ܿ��� SPEED UP */
#define _SYS_P			1
#define _MDBG_P			0
#define _BAT_P			0
#define _PUMP_P			0	/* PUMP */
#define _FAN_P			0	/* FAN */
#define _SMART_P		0
#define _CAN_P			0

#define _LIMIT_CHK		0
#define _CRACH_CHK		0
#define _U2RCV_CHK		0	/* 1 �̸�, Auto ���¿��� 200ms �̳� U2 Data�� ���� ���ϸ�, Blue �汤�� */
#define _OPT_CHK		0
#define _WLS_CHK		0
#define _NO_BUZ			0	/* 1�̸� NO Buzzer */

#define _LOWBAT_MASK	1	/* 0 �̸� Low Bat. Check �� ���� �ʴ´�. */
#define _WLS_MASK		1	/* 0 �̸� WLS �� �ν����� �ʴ´�. */

#define _SMT_DMSpeed	1	/* 1 �̸� Smart bd ���� ���� �ӵ��� �޾� ó���Ѵ�. */
#define _SMT_SPLAY		0	/* 1 �̸� Smart bd FAN/PUMP�� Control �Ѵ�. */

#define _GLED_USED		1	/* 1 �̸� G/R LED ���, 0 �̸� W/B LED ��� */
#define SOL_VALVE_TYPE  1
#define _PT_NPN			1	/* 0 PNP, 1 NPN <220401> */
#define _ATBD_PST		1	/* 0 Lee BD, 1 = PST BD */
#define _fan4Level		0	/* FAN OFF, 1,2,3 --> OFF 1,2,3,4 --> 1,2,3,4  */
#define _CURTIS			1	/* ���� DRIVER ĿƼ�� */
// 커티스 6으로 추정 기존 5 DRV_SPD 그냥 다 6인듯
//#define DRV_SPDTBNo		6
#define CONST_U2WATIME	2000	/* UART ��� Error ó�� Wait Time*/


#define SWver_High	    1   /* 1 =  ES, 2 = PP, 3 = MP */
#define SWver_Low	    1	/* */

#define SWupdat_Year	21
#define SWupdat_Month	05 
#define SWupdat_Day		20

#define	SOFTWARE_V_MAJOR	2
#define SOFTWARE_V_MINOR	1
#define SOFTWARE_V_PATCH	SWver_High
#define SOFTWARE_V_BUILD	SWver_Low

#define MDL_VerH	SWver_High  /*  */
#define MDL_VerL	1

#define u1Buf_Size  20
#define u1Data_End  0x0D	/* {ENT} */

#define u2Buf_Size  8
#define u2Data_End  '>'	/* {ENT} */

#define u3Buf_Size  8
#define u3Data_End  '>'		/* {ENT} */

#define BT_00	(1 << 0)
#define BT_01	(1 << 1)
#define BT_02	(1 << 2)
#define BT_03	(1 << 3)
#define BT_04	(1 << 4)
#define BT_05	(1 << 5)
#define BT_06	(1 << 6)
#define BT_07	(1 << 7)

#define BT_08	(1 << 8)
#define BT_09	(1 << 9)
#define BT_0a	(1 << 10)
#define BT_0b	(1 << 11)
#define BT_0c	(1 << 12)
#define BT_0d	(1 << 13)
#define BT_0e	(1 << 14)
#define BT_0f	(1 << 15)
//
#define BT_10	(1 << 16)
#define BT_11	(1 << 17)
#define BT_12	(1 << 18)
#define BT_13	(1 << 19)
#define BT_14	(1 << 20)
#define BT_15	(1 << 21)
#define BT_16	(1 << 22)
#define BT_17	(1 << 23)

#define BT_18	(1 << 24)
#define BT_19	(1 << 25)
#define BT_1a	(1 << 26)
#define BT_1b	(1 << 27)
#define BT_1c	(1 << 28)
#define BT_1d	(1 << 29)
#define BT_1e	(1 << 30)
#define BT_1f	(1 << 31)


/* PART A Define */
#if 0	
	23	PA0 	IN_ADC		Battery 48V +			CON-01,  7��	
	24	PA1 	IN_ADC		RFR Volume				CON-03, 11��					
	25	PA2 	IN_ADC		Current_Ready			CN19	
	26	PA3 	IN_ADC		????_Ready				CN21
	
	29	PA4 	OUT_DAC 	DRV_ACC 				CON-01,  1��	
	30	PA5 	OUT_DAC 	STR_ACC 				CON-01, 11��	
	31	PA6 	OUT_PORT	BL_BZ					CON-02,  4��	
	32	PA7 			
	67	PA8 	IN_Ready	
	68	PA9 	UART 1		TX	Debug				CON-04,  7��	J5 
	69	PA10				RX						CON-04,  8��	J5
	70	PA11	CAN 		RX						CON-04,  11��	
	71	PA12				TX						CON-04,  12�� 
	72	PA13	SYS_JTMS_SWDIO						J7(TMS) 
	76	PA14	SYS_JTCK_SWCLK(TCK) 				J7(TCK) 	
	77	PA15	SYS_JTDI
#endif	
#define A_in_ADC1_0				GPIO_PIN_0
#define A_in_ADC1_1				GPIO_PIN_1
#define A_in_ADC1_2				GPIO_PIN_2
#define A_in_ADC1_3				GPIO_PIN_3
#define GPIO_DAC1				GPIO_PIN_4
#define GPIO_DAC2				GPIO_PIN_5
#define A_BUZZER				GPIO_PIN_6

#define GPIO_UART1_Tx    		GPIO_PIN_9
#define GPIO_UART1_Rx    		GPIO_PIN_10
#define GPIO_CAN1_Rx    		GPIO_PIN_11
#define GPIO_CAN1_Tx    		GPIO_PIN_12


/* PART B Define */
#if 0
	35	PB0 	OUT_PORT	BL_G					CON-02,  2�� 
	36	PB1 	OUT_PORT	BL_B					CON-02,  3�� 
	37	PB2 	
	89	PB3 	SYS_JTDO_TRACESWO		
	90	PB4 	SYS_NJTRST		
	91	PB5 	OUT_PORT	EEP WP
	92	PB6 	OUT_PORT	I2C_SCL
	93	PB7 	OUT_PORT	I2C_SDA
	95	PB8 	OUT_PORT	LED_Y	
	96	PB9 	OUT_PORT	LED_W
	47	PB10	UART3		TX (RDY)				J6			
	48	PB11				RX						J6
	51	PB12	OUT_PORT	PUMP_ON 				CON-01,   2�� 
	52	PB13	OUT_PORT	PUMP_UP 				CON-01,   4�� 
	53	PB14	OUT_PORT	PUMP_OFF				CON-01,   6�� 
	54	PB15	OUT_PORT	PUMP_DN 				CON-01,  10��(��� ����)
#endif
#define B_BEACON_G				GPIO_PIN_0
#define B_BEACON_B				GPIO_PIN_1

#define GPIO_I2C_WP				GPIO_PIN_5	
#define GPIO_I2C_SCL			GPIO_PIN_6	
#define GPIO_I2C_SDA			GPIO_PIN_7
#define B_LED_Y					GPIO_PIN_8					
#define B_LED_W					GPIO_PIN_9					
#define GPIO_UART3_Tx    		GPIO_PIN_10
#define GPIO_UART3_Rx    		GPIO_PIN_11

#define B_PMPON					GPIO_PIN_12					
#define B_PMPUP					GPIO_PIN_13					
#define B_PMPOFF				GPIO_PIN_14					
#define B_PMPDN					GPIO_PIN_15		 			


/* PART C Define */
#if 0
	15	PC0 	IN_PORT 	STR_S_R (INT.)			CON-02,  10��
	16	PC1 	IN_PORT 	STR_S_L (INT.)			CON-02,   8�� 
	17	PC2 	IN_PORT 	CS_IN1	(INT.)			CON-02,  19�� 
	18	PC3 	IN_PORT 	CS_IN2	(INT.)			CON-02,   21��
	33	PC4 	IN_PORT 	STR_S_C 				CON-02,   7��			
	34	PC5 	OUT_PORT	BL_R					CON-02,   1��		
	63	PC6 	IN_PORT 	WLS_IN1(����)			CON-02,  15��	
	64	PC7 	IN_PORT 	WLS_IN2(��ü)			CON-02,  16�� 
	65	PC8 	IN_Ready
	66	PC9 	IN_Ready		
	78	PC10	UART4		TX (RDY)				J8		
	79	PC11				RX						J8
	80	PC12	IN_PORT 	RFR_F(forward)			CON-03,   1�� 
	7	PC13	IN_PORT 	RFR_AD(autodrive)		CON-03,   7�� 
	8	PC14	IN_PORT 	RFR_FANONUP 			CON-03,   9�� 
	9	PC15	IN_PORT 	RFR_FANOFF				CON-03,  10�� 
#endif
#define	C_STRright_LMT			GPIO_PIN_0			
#define	C_STRleft_LMT			GPIO_PIN_1
#define	C_CRASH_1				GPIO_PIN_2		/* CON02-19 2���� ���ķ� ����Ѵ�. */
//#define	C_CRASH_2				GPIO_PIN_3
#define	C_STRcenter				GPIO_PIN_4
#define C_BEACON_R				GPIO_PIN_5
#define	C_WLS_IN1				GPIO_PIN_6		// ���ι��� 
#define	C_WLS_IN2				GPIO_PIN_7		// ��ü���� 

//#define GPIO_UART4_Tx    		GPIO_PIN_10		// nck_220106
//#define GPIO_UART4_Rx    		GPIO_PIN_11		// nck_220106
#define C_OPT_RDY    			GPIO_PIN_10
#define C_OPT_COMM    			GPIO_PIN_11
#define C_OPT_VISION   			GPIO_PIN_11
#define	IR_FORWARD				GPIO_PIN_12	
#define	IR_AUTODRIVE			GPIO_PIN_13
#define	IR_FANONUP				GPIO_PIN_14
#define	IR_FANOFF				GPIO_PIN_15

/* PORT D Define */
#if 0	
	81	PD0 	IN_PORT 	RFR_B(backword) 	CON-03,   4��
	82	PD1 	IN_PORT 	RFR_L(left) 		CON-03,   5�� 
	83	PD2 	IN_PORT 	RFR_R(right)		CON-03,   6�� 
	84	PD3 	IN_PORT 	RFR_PUMP(on/off)	CON-03,   8�� 
	85	PD4 	IN_PORT 	RFR_MAIN			CON-03,  12�� 
	86	PD5 	UART2		TX(SMART)			CON-03,  21��, J4 
	87	PD6 				RX					CON-03,  23��, J4
	88	PD7
	55	PD8 	IN_Ready(3.3v, 10k PU)
	56	PD9 	IN_Ready(3.3v, 10k PU)
	57	PD10	IN_Ready(3.3v, 10k PU)
	58	PD11	IN_Ready(3.3v, 10k PU)			
	59	PD12	IN_Ready(3.3v, 10k PU)
	60	PD13	IN_Ready(3.3v, 10k PU)	 
	61	PD14	OUT_PWM_Ready(TIM4_CH3) 		TP102
	62	PD15	OUT_PWM_FAN(TIM4_CH4)			CON-01,  19��			
#endif	
#define	IR_BACKWARD		GPIO_PIN_0
#define	IR_LEFT			GPIO_PIN_1	
#define	IR_RIGHT		GPIO_PIN_2
#define	IR_PUMP			GPIO_PIN_3
#define IR_Main			GPIO_PIN_4
#define GIO_UART2_Tx    GPIO_PIN_5 
#define GIO_UART2_Rx    GPIO_PIN_6

#define	PWM_RDY			GPIO_PIN_14 
#define	PWM_FAN			GPIO_PIN_15 

/* PORT E Define */
#if 0
	97	PE0 	OUT_PORT	LED R
	98	PE1 	OUT_PORT	LED G
	1	PE2 	IN_PORT 	LK_E(emergency) 	CON-03,  15��(��� ����)
	2	PE3 	IN_PORT 	LK_F(forward)		CON-03,  17�� 
	3	PE4 	IN_PORT 	LK_B(backward)		CON-03,  20�� 
	4	PE5 	IN_PORT 	LK_L(left)			CON-03,  19�� 
	5	PE6 	IN_PORT 	LK_R(right) 		CON-03,  22�� 
	38	PE7 	
	39	PE8 	OUT_PORT	STR_BREAK(Ready)	
	40	PE9 	OUT_PORT	STR_C/CCW			CON-01,   9�� 
	41	PE10	PUT_PORT	DRV_BREAK			CON-01,   5��	
	42	PE11	OUT_PORT	DRV_C/CCW			CON-01,   3�� 
	43	PE12	OUT_PORT	SOL_V_1(����)		CON-01,  15��(����)
	44	PE13	OUT_PORT						CON-01,  18��
	45	PE14	IN_PORT 	�л�SPEED OPTION	CON-01,  16�� 
	46	PE15	IN_PORT 	���ι��� OPTION 	CON-01,  14��			
#endif
#define E_LED_R   			GPIO_PIN_0
#define E_LED_G   			GPIO_PIN_1
#define E_KEY_EMERGENCY		GPIO_PIN_2
#define E_KEY_FORWARD		GPIO_PIN_3
#define E_KEY_BACKWORD		GPIO_PIN_4
#define E_KEY_LEFT			GPIO_PIN_5
#define E_KEY_RIGHT			GPIO_PIN_6

#define E_strM_BREAK		GPIO_PIN_8
#define E_strM_CWCCW		GPIO_PIN_9

#define E_drvM_BREAK		GPIO_PIN_10
#define E_drvM_CWCCW		GPIO_PIN_11
#define E_SOLv_1			GPIO_PIN_12		/* 	CON01-15 */
//#define E_SOLv_2			GPIO_PIN_13		/*  CON-01-18 --> Not Used */
#define E_OPT_Pumpspeed		GPIO_PIN_14		/* Pump �з�	  f_OPT_Pumpspeed	 CON01- 14 */
#define E_OPT_Autospeed		GPIO_PIN_15		/* ��������ӵ�  f_OPT_AutodrvSPD	 CON01- 16 */

//#define GPIO_USB_DISCONNECT_PIN    GPIO_PIN_8
 
#define GPIO_PORTSOURCE_KEY        GPIO_PortSourceGPIOC
#define GPIO_PORTSOURCE_CRASH      GPIO_PortSourceGPIOC
#define GPIO_PORTSOURCE_STR_R      GPIO_PortSourceGPIOD
#define GPIO_PORTSOURCE_STR_L      GPIO_PortSourceGPIOD

#define GPIO_PINSOURCE_KEY1        GPIO_PINSource0
#define GPIO_PINSOURCE_KEY2        GPIO_PINSource1
#define GPIO_PINSOURCE_KEY7        GPIO_PINSource7
#define GPIO_PINSOURCE_CRASH	   GPIO_PINSource8

#define GPIO_PINSOURCE_STR_R	   GPIO_PINSource8
#define GPIO_PINSOURCE_STR_L	   GPIO_PINSource9

#define GPIO_EXTI_Line_KEY1        EXTI_Line0
#define GPIO_EXTI_Line_KEY2        EXTI_Line1
#define GPIO_EXTI_Line_KEY7        EXTI_Line7
#define GPIO_EXTI_Line_CRASH       EXTI_Line8

#define GPIO_EXTI_Line_STR_R       EXTI_Line8
#define GPIO_EXTI_Line_STR_L       EXTI_Line9


/* u16_DebugFlag  DBG_Print [DBG-xxxx][ENT] */
#define fDBG_Every1S 	(1 << 0)		/* Every 1 Sec, �Ϲ� Status Debug [DBG-0001] */
#define fDBG_StatusCHG	(1 << 1)		/* Systus ����� Debug [DBG-0002] */
//
#define fDBG_Print		(1 << 7)		/* UART 1 Print */


/* uTimerFlag */
#define TIM_10ms			(1 << 1)
#define TIM_30ms			(1 << 2)
#define TIM_50ms			(1 << 3)
#define TIM_100ms			(1 << 4)
#define TIM_200ms			(1 << 5)
#define TIM_500ms			(1 << 6)
#define TIM_1Sec			(1 << 7)
//
#define TIM_1SecMaker		(1 << 8)


/* uSysStatusFlag f_LimitLeft */
#define f_DMotorACT  		(1 << 0)	/* ���� Motor ���� ��  */
#define f_DMForward 		(1 << 1)	/* ���� Motor CWCCW */
#define f_DMBreak			(1 << 2)	/* ���� Motor Break */
#define f_SMotorACT			(1 << 3)	/* ���� Motor ���� ��  */
#define f_SMLeft       		(1 << 4)
#define f_PumpACT			(1 << 5)	/* PUMP */ // ACK 값으로 변경 
#define f_FanACT     		(1 << 6)	/* FAN */ // 중간시작 요청으로 변경
#define f_AutoRun			(1 << 7)	/* 1 = �������� Req. */

#define f_LimitCenter		(1 << 8)	/* 1 = ���� ����  */
#define f_LimitLeft			(1 << 9)	/* 1 = �� Limit ����  */
#define f_LimitRight		(1 << 10)	/* 1 = �� Limit ����  */
#define f_ALLWLEmpty		(1 << 11)	/* 1 = �������� ������   */
#define f_LowBat			(1 << 12)	/* 1 = Battery V. Low 35V ���� */
#define f_Crash				(1 << 13)	/* 1 = �浹 ���� ����  */
#define f_RFR_Emergency		(1 << 14)	/* 1 = RFR ���� ����  */
#define f_TraceLine			(1 << 15)	/* 1 = Line ��ȣ ���� */
//
#define f_OPT_Pumpspeed		(1 << 16)	/* 1 = ���� ���� ��   */
#define f_OPT_AutodrvSPD	(1 << 17)	/* 1 = ���� ���� ��   */
#define f_WLEmpty1			(1 << 18)	/* 1 = �������� ������ ����  */
#define f_WLEmpty2			(1 << 19)	/* 1 = �������� ������ ��ü  */
#define f_WTEmptBuzReq		(1 << 20)
#define f_LKATKInitStatus	(1 << 21)
#define a_NotCondition		(1 << 22)
#define a_Auto_Ready		(1 << 23)


#define f_PumpOn			(1 << 24)
#define f_FanOn				(1 << 25)
#define f_smartConnected	(1 << 26)
#define f_Pump1st			(1 << 27)
#define f_ADRVnoWaterStop	(1 << 28)	/* ���������� No Water Stop  nck-1214 */
#define f_ADRVnoCANData		(1 << 29)	/* When ���������� No CAN Data,  Stop  nck-1222 */


/* uKeyStatusFlag  */
#define flk_Forward			(1 << 0)
#define flk_Backward		(1 << 1)
#define flk_Left			(1 << 2)
#define flk_Right			(1 << 3)

#define frk_Emergency		(1 << 4)
#define frk_Forward			(1 << 5)
#define frk_Backward		(1 << 6)
#define frk_Left			(1 << 7)
//
#define frk_Right			(1 << 8)
#define frk_AutoDrv			(1 << 9)
#define frk_Pump			(1 << 10)
#define frk_FanOnUp			(1 << 11)
#define frk_FanOff			(1 << 12)

#define fLK_Ok   			(1 << 16)
#define fLK_Repeat 			(1 << 17)
#define fRK_Ok				(1 << 18)
#define fRK_Repeat			(1 << 19)

#define smt_Forward			(1 << 25)
#define smt_Backward		(1 << 26)
#define smt_Left			(1 << 27)
#define smt_Right			(1 << 28)

// uCommStatusFlag f_uart3DTIn
#define f_uart2DTIn			(1 << 0)
#define f_canDTIn			(1 << 1)
#define f_canERIn			(1 << 2)
#define f_uart1DTIn			(1 << 3)

#define f_U2CSumErr			(1 << 8)
#define f_U2DEErr			(1 << 9)
#define f_U2INTNon			(1 << 10)


#if 1
/* NVM Memory Address Define    */
#define MEMadr_PAGE0		0xA0	/* 1010 000 0 */
#define MEMadr_PAGE1		0xA2	/* 1010 001 0 */
#define MEMadr_PAGE2		0xA4	/* 1010 010 0 */
#define MEMadr_PAGE3		0xA6	/* 1010 011 0 */

#define EVT_CNT_ADDR		0xF0		
#define MAGIC_CHECK_ADRL    0xFE
#define MAGIC_CHECK_ADRH    0xFF

#define DRV_PWMadr1		0x10
#define DRV_PWMadr2		0x11
#define DRV_PWMadr3		0x12
#define DRV_PWMadr4		0x13
#define DRV_PWMadr5		0x14

#define STR_PWMadr1		0x20
#define STR_PWMadr2		0x21
#define STR_PWMadr3		0x22

#define PUMP_PWMadr1	0x30
#define PUMP_PWMadr2	0x31
#define PUMP_PWMadr3	0x32

#define FAN_PWMadr1		0x40
#define FAN_PWMadr2		0x41
#define FAN_PWMadr3		0x42

#define Model_Name0		0xA0	/* H */
#define Model_Name1		0xA1	/* O */
#define Model_Name2		0xA2	/* S */
#define Model_Name3		0xA3	/* A */
#define Model_Name4		0xA4	/* N */
#define Model_Name5		0xA5	/* N */
#define Model_Name6		0xA6	/* A */
#define Model_Name7		0xA7	/* - */

#define MFacture_YAddr	0xA8	/* 20+ */
#define MFacture_MAddr	0xA9	/* 1 ~ 12 */
#define MFacture_DAddr	0xAA	/* 1 ~ 31 */

#define Serial_0Addr	0xAB
#define Serial_1Addr	0xAC
#endif

#define DMOTOR_STOP		0
#define	DMOTOR_START	1		/* �������� ���� Port Set */
#define	DMOTOR_BRKOFF	2		/* Drive M. Break Release */
#define	DMOTOR_ACCEL	3		/* Tarket Accel PWM  */
#define	DMOTOR_ACCELACT	4		/* PWM Set */
#define	DMOTOR_END		5

#define SMOTOR_STOP		0
#define	SMOTOR_START	1		/* �������� ���� Port Set */
#define	SMOTOR_BRKOFF	2		/* Drive M. Break Release */
#define	SMOTOR_ACCEL	3		/* Tarket Accel PWM  */
#define	SMOTOR_ACCELACT	4		/* PWM Set */
#define	SMOTOR_END		5

#define PUMP_NULL		0
#define PUMP_START		1	/* PUMP ON Low */
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
#define PUMP_SOL_ON     16

#define FAN_NULL		0
#define FAN_START		1
#define FAN_SPEED		2
#define FAN_Stay		3
#define FAN_STOP		5

#define KEY_NONE		0
#define KEY_FORWARD		0x01
#define KEY_BACKWORD	0x02
#define KEY_LEFT		0x04
#define KEY_RIGHT		0x08
//
#define KEY_PUMP		0x10
#define KEY_AUTORUN		0x20
#define KEY_FANONUP		0x40
#define KEY_FANOFF		0x80

#define RFR_NONE		0
#define RFR_FORWARD		0x01		
#define RFR_ATDRV		0x02		
#define RFR_FANONUP		0x04		
#define RFR_FANOFF		0x08	
#define RFR_BACKWORD	0x10
#define RFR_LEFT		0x20
#define RFR_RIGHT		0x40
#define RFR_PUMP 		0x80
#define RFR_EMERGENCY	0x100


#define bcon_R		0x01
#define bcon_G		0x04
#define bcon_B		0x02
#define bcon_RG		bcon_R | bcon_G
#define bcon_RB		bcon_R | bcon_B
#define bcon_GB		bcon_G | bcon_B
#define bcon_WHT	bcon_R | bcon_G | bcon_B
#define bcon_BZ		0x08
#define bcon_All	0x0F

#define NONE 0x01
#define BACK 0x02
#define FRONT 0x04
#define CAN_FRONT_REQ 0x01
#define CAN_BACK_REQ 0x02
//



#endif 
