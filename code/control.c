/*
file:  control.c
description : control all compoment of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/
#include "debug.h" 
#include "control.h"
#include "hardware.h"
#include "define.h"
#include "state.h"
extern void comfirmStateData(uint8_t state);
extern void setStateMachine(uint8_t state);;
/*****************************************************************************
function:  getInfo
argunent: uint8_t *info
return : uint8_t
description : function to get inffo device
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t getInfo(uint8_t *info){
  uint8_t value_return=0;
  printDebug("START:uint8_t getInfo"); 
  value_return=setupMasterConnect();
  printDebug("End:uint8_t getInfo"); 
  return value_return;
}
/*****************************************************************************
function:  connectDevice
argunent: uint8_t *info
return : uint8_t
description : function to connect server
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t connectDevice(uint8_t *info){
  uint8_t error_code;
  printDebug("START:uint8_t connectDevice"); 
  error_code=sendMessage(info);
  printDebug("End:uint8_t connectDevice"); 
  return error_code;
}
uint8_t checkListData(uint8_t *info){
  printDebug("START:uint8_t checkListData"); 

  printDebug("End:uint8_t checkListData"); 
  return 0;
}
/*****************************************************************************
function:  gotoSleep
argunent: uint8_t state
return : void
description : function to sleep power notice state
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void gotoSleep(void){
  printDebug("START:void gotoSleep"); 

   setupSleep();
  printDebug("End:void gotoSleep"); 
}
/*****************************************************************************
function:  gotoStandby
argunent: uint8_t state
return : void
description : function to standby power notice state
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t gotoStandby(uint8_t state){
  uint8_t value_return=0;
  printDebug("START:void gotoStandby"); 
  value_return=setupStandby(state);
  printDebug("End:void gotoStandby"); 
  return value_return;
}
void wakeUpDevice(void){
  printDebug("START:void wakeUpDevice"); 

  printDebug("End:void wakeUpDevice"); 
}

/*****************************************************************************
function:  checkPower
argunent: void
return : uint8_t
description : function to get all value sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t checkPower(void){
  uint8_t value_return=0;
  printDebug("START:uint8_t checkPower"); 
  value_return=getpower();
  printDebug("End:uint8_t checkPower"); 
  return value_return;
}

/*****************************************************************************
function:  getAllDataSensor
argunent: data
return : State
description : function to get all value sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t getAllDataSensor(uint8_t *data){
  uint8_t i;
  uint8_t value_return=0;
  printDebug("START:uint8_t getAllDataSensor"); 
  for(i=0;i<NUMBER_DEVICE;i++){
    value_return|=checkDeviceNeedOpenFist(i);
  }
  for(i=0;i<NUMBER_DEVICE;i++){
    value_return|=controlDevice(i,STATE_OPEN_DEVICE);
    value_return|=getValueSensorDevice(data,i);
    value_return|=controlDevice(i,STATE_CLOSE_DEVICE);
  }
  printDebug("End:uint8_t getAllDataSensor"); 
  return value_return;
}
/******************************************************************************
function:  sendDataToServer
argunent: data
return : State
description : function to send data to server
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t sendDataToServer(uint8_t *data){
  uint8_t error_code=0;
  printDebug("START:uint8_t sendDataToServer"); 
  error_code=sendMessage(data);
  printDebug("End:uint8_t sendDataToServer"); 
  return error_code;
}
/******************************************************************************
function:  getInfoSorlar
argunent: data
return : State
description : function to getinfomation solar
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t getInfoSorlar(void){
  printDebug("START:uint8_t getInfoSorlar"); 
  getpower();
  printDebug("End:uint8_t getInfoSorlar"); 
  return 0;
}
/******************************************************************************
function:  setTimerArlam
argunent: time
return : State
description : function getArlam after second
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t setTimerArlam(uint16_t time){
  printDebug("START:uint8_t getInfoSorlar"); 
  setTimerArlarmGetup(time);
  printDebug("End:uint8_t getInfoSorlar"); 
  return 0;
}

/******************************************************************************
function:  confirmDataCommucation
argunent: time
return : State
description : confirmrecivedata
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void confirmDataCommucation(uint8_t state){
  comfirmStateData(state);
}
/******************************************************************************
function:  setupStateMachineActicve
argunent: void
return : void
description : confirmrecivedata
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void setupStateMachineActicve(void){
setStateMachine(STATE_WAKE_UP);
}