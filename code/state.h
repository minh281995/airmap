#include "stm32f10x.h"

enum {
    STATE_SETUP=0,
    STATE_WAIT,
    STATE_ALWAYS_STAND_BY,
    ON_SLEEP,
    STATE_CHARGING,
    STATE_WAKE_UP,
    STATE_STAND_BY,
    STATE_WAIT_SENSOR_RESPONSE,
    STATE_TRANSFER,
    STATE_ERROR,
    STATE_UNDEFINE
};


uint8_t getStateInfoDevice(void);
void getInfoDevice(uint8_t * info);
void setStateMachine(uint8_t state);
uint8_t getStateRequest(void);
void comfirmStateData(uint8_t state);
