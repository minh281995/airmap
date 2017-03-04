/*
file:  control.h
description : control all compoment of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/

#include "debug.h" 
#include "hardware.h"


uint8_t TimeDisplay = 0;
// port Device control
uint32_t g_GPIO_device[NUMBER_DEVICE] ={GPIOB_BASE,GPIOB_BASE,GPIOB_BASE,GPIOB_BASE,GPIOB_BASE,GPIOB_BASE,GPIOB_BASE};
// pindevice conntrol;
uint32_t g_GPIO_pin_device[NUMBER_DEVICE]= {GPIO_Pin_0,GPIO_Pin_0,GPIO_Pin_0,GPIO_Pin_0,GPIO_Pin_0,GPIO_Pin_0,GPIO_Pin_0};

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void){
  printDebug("START:void NVIC_Configuration"); 
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  printDebug("End:void NVIC_Configuration"); 
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void){
  printDebug("START:void RTC_Configuration"); 
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  printDebug("End:  {}"); 
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void){
  printDebug("START:void RTC_IRQHandler"); 
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
  //printDebug("START:  {"); 
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Enable time update */
    TimeDisplay++;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
  //printDebug("End:  {"); 
  }
  printDebug("End:  {"); 
}
/*******************************************************************************
function:  controlDevice
argunent: uint8_t device,uint8_t state
return : uint8_t
description : function control Device 
Create date : 19/2/2017
user cread : Lu Van Cuong
*******************************************************************************/
uint8_t controlDevice(uint8_t device,uint8_t state){
    if(device<NUMBER_DEVICE){
      if(STATE_OPEN_DEVICE==state){
        GPIO_SetBits((GPIO_TypeDef*)g_GPIO_device[device],g_GPIO_pin_device[device]);
      }
      else {
        if(STATE_CLOSE_DEVICE==state){
          GPIO_ResetBits((GPIO_TypeDef*)g_GPIO_device[device],g_GPIO_pin_device[device]);
        }
        else
          return RETURN_UNDEFINE;
      }
    }
    else 
      return NG;
    
  return OK;
}
/*******************************************************************************
function:  checkDeviceNeedOpenFist
argunent: uint8_t device
return : void
description : function to open device need user very muchtime
Create date : 19/2/2017
user cread : Lu Van Cuong
*******************************************************************************/
uint8_t checkDeviceNeedOpenFist(uint8_t device){
  if(device<NUMBER_DEVICE)
    GPIO_SetBits((GPIO_TypeDef*)g_GPIO_device[device],g_GPIO_pin_device[device]);
  else 
    return NG;
  return OK;
}

/******************************************************************************
function:  setTimerArlarmGetup
argunent: time
return : State
description : function getArlam after second
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t setTimerArlarmGetup(uint16_t time){
  printDebug("START:uint8_t getInfoSorlar"); 

  printDebug("End:uint8_t getInfoSorlar"); 
  return 0;
}
/* them thong tin moi*/