/*
file:  communication.c
description : manager all device 
Create date : 19/2/2017
user cread : Lu Van Cuong
*/
#include "debug.h" 
#include "device.h"

void RTC_StandByConfiguration(void)
{
  printDebug("START:RTC_StandByConfiguration"); 
  /* Check if the StandBy flag is set */
  if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {/* System resumed from STANDBY mode */

    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

  }
  else
  {/* StandBy flag is not set */


    /* RTC clock source configuration ----------------------------------------*/
    /* Reset Backup Domain */
    BKP_DeInit();
  
    /* Enable LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* RTC configuration -----------------------------------------------------*/
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Set the RTC time base to 1s */
    RTC_SetPrescaler(32767);  
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
  printDebug("End: RTC_StandByConfiguration"); 
}

// Before use standby mode 
void StandbyConfig(void)
{
  printDebug("START:{"); 
	/* Enable PWR clock */
	RCC->APB1ENR |=RCC_APB1Periph_PWR;
	/* Enable BACKUP clock */
	RCC->APB1ENR |=RCC_APB1Periph_BKP;
	
	/* Allow access to BKP Domain */
	//*(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;
       //(uint32_t *) 0x00023000 = (uint32_t)ENABLE;
	RTC_Configuration(); /* Configure RTC clock source and prescaler */
  printDebug("End:{"); 
}


void SleepModeConfig(void){
  printDebug("START:void SleepModeConfig"); 
	/* Enable PWR clock */
	RCC->APB1ENR |=RCC_APB1Periph_PWR;
  printDebug("End:void SleepModeConfig"); 
}
/******************************************************************
function:  setupSleep
argunent: void
return : uint8_t
description : function to set sleep
Create date : 19/2/2017
user cread : Lu Van Cuong
*******************************************************************/
uint8_t setupSleep(void){
  printDebug("START:uint8_t setupSleep"); 
	/* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; /* Disable deepsleep */
  SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk; /* Disable sleeponexit */
  __DSB(); /* Ensure effect of last store takes effect */
  __WFI(); /* Enter sleep mode */
  printDebug("End:uint8_t setupSleep"); 
  return 0;
}
uint8_t setupStandby(uint8_t state){
  printDebug("START:uint8_t setupStandby"); 

  if(STATE_ALWAYS_STANDBY==state){
      PWR->CR |= CR_CWUF_Set; /* Clear Wake-up flag */
      PWR->CR |= CR_PDDS_Set; /* Select STANDBY mode */
      /* Set SLEEPDEEP bit of Cortex System Control Register */
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /* Enable deepsleep */
      __WFI(); /* Request Wait For Interrupt */
  }

  if(STATE_STOP_STANDBY==state){
      /* Disable STANDBY mode */
      PWR->CR =PWR->CR&~CR_PDDS_Set;
  }
  
  printDebug("End:  if"); 
  return 0;
}
/*
uint8_t controlOpenDevice(uint8_t state){
  printDebug("START:uint8_t controlOpenDevice"); 

  printDebug("End:uint8_t controlOpenDevice"); 
  return 0;
}*/
uint8_t setupActive(void){
  printDebug("START:uint8_t setupActive"); 

  	/* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; /* Disable deepsleep */
  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; /* Disable sleeponexit */
  __DSB(); /* Ensure effect of last store takes effect */
  __WFI(); /* Enter sleep mode */
  printDebug("End:uint8_t setupActive"); 
  return 0;
}


