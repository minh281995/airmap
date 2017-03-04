/*
file:  main.c
Create date : 19/2/2017
user cread : Lu Van Cuong
*/
#include "stm32f10x.h"
#include "debug.h"
#include "state.h"
#include "define.h"

static uint8_t g_set_request=NOT_REQUEST;
void setup(void){
  uint8_t  info[10];

}
/******************************************************************************
function:  requestState
argunent: void
return : void
description : check state machine
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
void requestState(void){
  uint8_t statemachine;
  printDebug("START:requestState");
    if(NOT_REQUEST==g_set_request){
    statemachine = getStateRequest();
    //checkPowerState();
    g_set_request=REQUEST;
  } 
  printDebug("End:requestState");
}
/******************************************************************************
function:  main
argunent: void
return : int
description : main program
Create date : 19/2/2017
user cread : Lu Van Cuong
******************************************************************************/
int main(void)
{
  uint8_t statemachine,devicename;
  uint8_t *data;
  uint8_t power_energy,error_code;
  //E_ReturnRequest return_data;
  printDebug("START:main");
  setup();
  statemachine = getStateInfoDevice();
  if(statemachine == FIST_USER){
    error_code=getInfo(data);
  }
  getInfoDevice(data);
  error_code|=connectDevice(data);
  setStateMachine(STATE_WAIT);
  setTimerArlam(120);
  
  while (1)
  {
    statemachine = getStateRequest(); 
    error_code = OK;
    switch(statemachine){
    case    STATE_SETUP:         {  break;}
    case    STATE_WAIT:          {
              // add code finction special
              if(getStateCommunication()){
              }
              setStateMachine(ON_SLEEP);
              gotoSleep();
              g_set_request=NOT_REQUEST;
              break;
            }
    case    STATE_ALWAYS_STAND_BY: { 
            // no action
            g_set_request=NOT_REQUEST;
            break;}
    case    ON_SLEEP:            { 
            // no action
            g_set_request = NOT_REQUEST;
            break;}
    case    STATE_CHARGING:      { break;}
    case    STATE_WAKE_UP:       {
            power_energy = checkPower();
              if(LOW_POWER == power_energy){
                setStateMachine(STATE_STAND_BY);
                error_code = gotoStandby(STATE_STAND_BY);
              }
            g_set_request = NOT_REQUEST;
            break;}
    case    STATE_STAND_BY:      { 
            g_set_request = NOT_REQUEST;
            break;}
    case    STATE_WAIT_SENSOR_RESPONSE: { 
              error_code=getAllDataSensor(data);   
              g_set_request=NOT_REQUEST;
              break;
            }
    case    STATE_TRANSFER:       { 
              error_code=sendDataToServer(data);
              g_set_request=NOT_REQUEST;
              break;
            }
    case    STATE_ERROR:         { 
            g_set_request=NOT_REQUEST;
            break;}
    case    STATE_UNDEFINE:      { 
            g_set_request=NOT_REQUEST;
            break;}
    default:
      {
        g_set_request=NOT_REQUEST;
         break;
      }
     
    }
    // if check error then goto state error
    if(OK!=error_code){
      setStateMachine(STATE_ERROR);
    }
    requestState();

  }
  printDebug("End:main");
}

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}

