/*
file:  state.c
description : manager all state of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/
#include "debug.h" 

#include "state.h"

static uint8_t m_statemachine=STATE_SETUP; 
static uint16_t m_lengdata_recive; 
static uint8_t m_datarecive; 
/*****************************************************************************
function:  getStateInfoDevice
argunent: void
return : uint8_t
description : function to get infomation ofdevice in fist user
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t getStateInfoDevice(void){
  printDebug("START:uint8_t getStateInfoDevice"); 

  printDebug("End:uint8_t getStateInfoDevice"); 
  return 0;
}
/*****************************************************************************
function:  getInfoDevice
argunent: uint8_t * info
return : void
description : function to get infomation of device 
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void getInfoDevice(uint8_t * info){
  printDebug("START:void getInfoDevice"); 

  printDebug("End:void getInfoDevice"); 
}
/*****************************************************************************
function:  setStateMachine
argunent: uint8_t state
return : void
description : function to set state machine
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void setStateMachine(uint8_t state){
  printDebug("START:void setStateMachine"); 
  m_statemachine=state;
  printDebug("End:void setStateMachine"); 
}

uint8_t getStateRequest(void){
  printDebug("START:uint8_t getStateRequest"); 

  printDebug("End:uint8_t getStateRequest"); 
  return m_statemachine;
}

/******************************************************************************
function:  comfirmStateData
argunent: time
return : State
description : confirm recivedata
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void comfirmStateData(uint8_t state){


}
/******************************************************************************
function:  getStateCommunication
argunent: void
return : State
description : 
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
uint8_t getStateCommunication(void){
return m_datarecive;
}
