#ifndef __MQ136LIB_H
#define __MQ136LIB_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

void MQ136Setup(GPIO_TypeDef* MQ136Port, uint16_t MQ136Pin,uint8_t ADC_Channel);
uint16_t MQ136GetResult(void);


#endif