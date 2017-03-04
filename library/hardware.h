/*
file:  control.h
description : control all compoment of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/

#include "stm32f10x.h"
#include "define.h"

#define SENSOR_CO 1
#define NUMBER_DEVICE 7
#define STATE_OPEN_DEVICE 0
#define STATE_CLOSE_DEVICE 1
//#define 




void RTC_Configuration(void);
void NVIC_Configuration(void);
void RTC_IRQHandler(void);
uint8_t controlDevice(uint8_t device,uint8_t state);
uint8_t checkDeviceNeedOpenFist(uint8_t device);
uint8_t setTimerArlarmGetup(uint16_t time);