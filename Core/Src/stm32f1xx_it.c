/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "define.h"
#include "task.h"
#include "cli.h"
#include "motor.h"
#include "hw_config.h"
#include "debug.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
CAN_RxHeaderTypeDef rxHeader;

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1) {
	}
	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */

	/* USER CODE END MemoryManagement_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
		/* USER CODE END W1_MemoryManagement_IRQn 0 */
	}
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void) {
	/* USER CODE BEGIN BusFault_IRQn 0 */

	/* USER CODE END BusFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_BusFault_IRQn 0 */
		/* USER CODE END W1_BusFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
	/* USER CODE BEGIN UsageFault_IRQn 0 */

	/* USER CODE END UsageFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
		/* USER CODE END W1_UsageFault_IRQn 0 */
	}
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) {
	/* USER CODE BEGIN SVCall_IRQn 0 */

	/* USER CODE END SVCall_IRQn 0 */
	/* USER CODE BEGIN SVCall_IRQn 1 */

	/* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void) {
	/* USER CODE BEGIN PendSV_IRQn 0 */

	/* USER CODE END PendSV_IRQn 0 */
	/* USER CODE BEGIN PendSV_IRQn 1 */

	/* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) {
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles DMA1 channel1 global interrupt.
 */
void DMA1_Channel1_IRQHandler(void) {
	/* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

	/* USER CODE END DMA1_Channel1_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_adc1);
	/* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

	/* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel3 global interrupt.
 */
void DMA1_Channel3_IRQHandler(void) {
	/* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

	/* USER CODE END DMA1_Channel3_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart3_rx);
	/* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

	/* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel5 global interrupt.
 */
void DMA1_Channel5_IRQHandler(void) {
	/* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

	/* USER CODE END DMA1_Channel5_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
	/* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

	/* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel6 global interrupt.
 */
void DMA1_Channel6_IRQHandler(void) {
	/* USER CODE BEGIN DMA1_Channel6_IRQn 0 */

	/* USER CODE END DMA1_Channel6_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
	/* USER CODE BEGIN DMA1_Channel6_IRQn 1 */

	/* USER CODE END DMA1_Channel6_IRQn 1 */
}

/**
 * @brief This function handles USB high priority or CAN TX interrupts.
 */
void USB_HP_CAN1_TX_IRQHandler(void) {
	/* USER CODE BEGIN USB_HP_CAN1_TX_IRQn 0 */

	/* USER CODE END USB_HP_CAN1_TX_IRQn 0 */
	HAL_CAN_IRQHandler(&hcan);
	/* USER CODE BEGIN USB_HP_CAN1_TX_IRQn 1 */

	/* USER CODE END USB_HP_CAN1_TX_IRQn 1 */
}

/**
 * @brief This function handles USB low priority or CAN RX0 interrupts.
 */
/// 여기가 찐 CAN RX
void USB_LP_CAN1_RX0_IRQHandler(void) {
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */
	CanRxMsg CanRxData;
	
	/* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
	HAL_CAN_IRQHandler(&hcan);
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */
	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, &CanRxData.Data);

	#if 1 // 나이사님 ver 
	if((rxHeader.StdId == 0x123)&&(rxHeader.IDE == CAN_ID_STD)&&(rxHeader.DLC == 2))
	{
		CAN_RxData[0] = CanRxData.Data[0];	/* Line ���� �� ����, ���� Command */
		CAN_RxData[1] = CanRxData.Data[1];	/* Ready Byte */

		Received_canID = rxHeader.StdId;
		Received_canIDType = rxHeader.IDE;
		Received_canDLC = rxHeader.DLC;
		canDTIntime = 2000; 		/* nck-1222 */
		uCommStatusFlag |= f_canDTIn;
	}
	if((rxHeader.StdId == 0x500)&&(rxHeader.IDE == CAN_ID_STD)&&(rxHeader.DLC == 8))
	{
		CAN_RxData_Error[0] = CanRxData.Data[0];	
		CAN_RxData_Error[1] = CanRxData.Data[1];
		CAN_RxData_Error[2] = CanRxData.Data[2];	
		CAN_RxData_Error[3] = CanRxData.Data[3];
		CAN_RxData_Error[4] = CanRxData.Data[4];	
		CAN_RxData_Error[5] = CanRxData.Data[5];
		CAN_RxData_Error[6] = CanRxData.Data[6];	
		CAN_RxData_Error[7] = CanRxData.Data[7];	
		can1_Trans8b(0x03);
	
		Received_canID = rxHeader.StdId;
		Received_canIDType = rxHeader.IDE;
		Received_canDLC = rxHeader.DLC;
		
		uCommStatusFlag |= f_canERIn;
	}
	#endif 



	/* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}

/**
 * @brief This function handles CAN RX1 interrupt.
 */
void CAN1_RX1_IRQHandler(void) {
	/* USER CODE BEGIN CAN1_RX1_IRQn 0 */

	/* USER CODE END CAN1_RX1_IRQn 0 */
	HAL_CAN_IRQHandler(&hcan);


}

/**
 * @brief This function handles CAN SCE interrupt.
 */
void CAN1_SCE_IRQHandler(void) {
	/* USER CODE BEGIN CAN1_SCE_IRQn 0 */

	/* USER CODE END CAN1_SCE_IRQn 0 */
	HAL_CAN_IRQHandler(&hcan);
	/* USER CODE BEGIN CAN1_SCE_IRQn 1 */

	/* USER CODE END CAN1_SCE_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void) {
	/* USER CODE BEGIN TIM3_IRQn 0 */
	ms_cnt++;
	if ((ms_cnt % dac_max_value) == 0)
		uTimerFlag |= TIM_10ms;
	if ((ms_cnt % 30) == 0)
		uTimerFlag |= TIM_30ms;
	if ((ms_cnt % 100) == 0)
		uTimerFlag |= TIM_100ms;
	if ((ms_cnt % 200) == 0)
		uTimerFlag |= TIM_200ms;
	if ((ms_cnt % 500) == 0)
		uTimerFlag |= TIM_500ms;


	if (Local_Key.lK_ChatCnt)
		--Local_Key.lK_ChatCnt;
	if (RFR_Key.rK_ChatCnt)
		--RFR_Key.rK_ChatCnt;
	if (DMotorWaitime)
		--DMotorWaitime;
	if (SMotorWaitime)
		--SMotorWaitime;
	if (PumpWaittime)
		--PumpWaittime; /* PUMP */
	if (FanWaittime)
		--FanWaittime; /* FAN */
	if (canDTIntime)
		--canDTIntime; 
	//if(u2Init_time)	--u2Init_time;
	if (u2DTInTime)
		--u2DTInTime;
	//if(uMainWait_tm)	--uMainWait_tm;
	if (u16_RFMNChkCnt)
		--u16_RFMNChkCnt;
	if (LKSeqDlyTime)
		--LKSeqDlyTime;
	if ( Bcon_dly)
		--Bcon_dly;
	if ( RTK_check_flag )
	{
		if ( RTK_check_dly )
		--RTK_check_dly;
	}
	

	/* USER CODE END TIM3_IRQn 0 */
	HAL_TIM_IRQHandler(&htim3);
	/* USER CODE BEGIN TIM3_IRQn 1 */

	/* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles TIM4 global interrupt.
 */
void TIM4_IRQHandler(void) {
	/* USER CODE BEGIN TIM4_IRQn 0 */

	/* USER CODE END TIM4_IRQn 0 */
	HAL_TIM_IRQHandler(&htim4);
	/* USER CODE BEGIN TIM4_IRQn 1 */

	/* USER CODE END TIM4_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */

	/* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles USART2 global interrupt.
 */
void USART2_IRQHandler(void) {
	/* USER CODE BEGIN USART2_IRQn 0 */

	/* USER CODE END USART2_IRQn 0 */
	HAL_UART_IRQHandler(&huart2);
	/* USER CODE BEGIN USART2_IRQn 1 */

	/* USER CODE END USART2_IRQn 1 */
}

/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void) {
	/* USER CODE BEGIN USART3_IRQn 0 */

	/* USER CODE END USART3_IRQn 0 */
	HAL_UART_IRQHandler(&huart3);
	/* USER CODE BEGIN USART3_IRQn 1 */

	/* USER CODE END USART3_IRQn 1 */
}

/**
 * @brief This function handles UART4 global interrupt.
 */
void UART4_IRQHandler(void) {
	/* USER CODE BEGIN UART4_IRQn 0 */

	/* USER CODE END UART4_IRQn 0 */
	HAL_UART_IRQHandler(&huart4);
	/* USER CODE BEGIN UART4_IRQn 1 */

	/* USER CODE END UART4_IRQn 1 */
}

/**
 * @brief This function handles DMA2 channel3 global interrupt.
 */
void DMA2_Channel3_IRQHandler(void) {
	/* USER CODE BEGIN DMA2_Channel3_IRQn 0 */

	/* USER CODE END DMA2_Channel3_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_uart4_rx);
	/* USER CODE BEGIN DMA2_Channel3_IRQn 1 */

	/* USER CODE END DMA2_Channel3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
