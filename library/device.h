
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#define CR_CWUF_Set              ((uint32_t)0x00000004)
#define CR_PDDS_Set              ((uint32_t)0x00000002)
#define STATE_STOP_STANDBY             0 // turn off standby
#define STATE_ALWAYS_STANDBY           1  // turn on standby
#define CR_DBP_BB_DMMY                 0x42000000 // nedd eddit


void RTC_StandByConfiguration(void);
void StandbyConfig(void);
void SleepModeConfig(void);
uint8_t setupSleep(void);
uint8_t setupStandby(uint8_t state);
//uint8_t controlOpenDevice(uint8_t state);
uint8_t setupActive(void);
