/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SHT75_H
#define __SHT75_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

/*Private define */
#define DIR_OUT GPIO_Mode_Out_PP
#define DIR_IN  GPIO_Mode_IN_FLOATING
#define SHT_DATA_H GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define SHT_DATA_L GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define SHT_CLK_H  GPIO_SetBits(GPIOC,GPIO_Pin_14)
#define SHT_CLK_L  GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define DATA_IN    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
/* Private macro */


void SHT_Start(void);
void SHT_Wait(void);
void SHT_Reset(void);
void SHT_ReadData(float *temp, float *truehumi);
void SHT_Init(void);
void SHT_CalculateData(int16_t temp, int16_t humid, float *tc, float *rhlin, float *rhtrue);
void GPIO_Direction(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef Mode);
void SHT_ConfigGPIO(void);
uint8_t  SHT_WriteByte(uint8_t Data);
uint8_t  SHT_ReadByte(void);
uint16_t SHT_Temp(void);
uint16_t SHT_Humi(void);



#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */

