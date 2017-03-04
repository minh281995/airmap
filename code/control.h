/*
file:  control.h
description : control all compoment of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/

#include "stm32f10x.h"

uint8_t getInfo(uint8_t *info);
uint8_t connectDevice(uint8_t *info);
uint8_t checkListData(uint8_t *info);
void gotoSleep(void);
uint8_t gotoStandby(uint8_t state);
void wakeUpDevice(void);
uint8_t checkPower(void);
uint8_t getAllDataSensor(uint8_t *data);
uint8_t sendDataToServer(uint8_t *data);
uint8_t getInfoSorlar(void);
uint8_t setTimerArlam(uint16_t time);
void confirmDataCommucation(uint8_t state);
