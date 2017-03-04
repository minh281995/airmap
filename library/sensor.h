/*
file:  sensor.c
description : manager all sensor of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/

#include "stm32f10x.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

#include "stdio.h"
// struct data of sensor
typedef struct _DataSensorStruct{
    uint16_t temperature;
    uint16_t humidity;
    uint32_t pressure;
    float COdata;
    float SO2data;
    float CO2data;
    float O3data;
    float PM2_5data;
    float PM10data;
    float PM1data;
    float verhicle
}DataSensorStruct;

#define SENSOR_ID_CO 0
#define SENSOR_ID_CO2 1
#define SENSOR_ID_SO2 2
#define SENSOR_ID_O3 3
#define SENSOR_ID_TEMP_HUMI 4
#define SENSOR_ID_PRESSURE 5
#define SENSOR_ID_DUST 6
#define SENSOR_ID_VERHICLE 7

/*Private define SHT75 */
#define DIR_OUT GPIO_Mode_Out_PP
#define DIR_IN  GPIO_Mode_IN_FLOATING
#define SHT_DATA_H GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define SHT_DATA_L GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SHT_CLK_H  GPIO_SetBits(GPIOC, GPIO_Pin_14)
#define SHT_CLK_L  GPIO_ResetBits(GPIOC, GPIO_Pin_14)
#define DATA_IN    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
/* Private macro */

/*define MQ136*/
#ifndef __MQ136LIB_H
#define __MQ136LIB_H
#endif

void MQ136Setup(GPIO_TypeDef* MQ136Port, uint16_t MQ136Pin,uint8_t ADC_Channel);
float sensorGetSO2Result(void);
float sensorGetCOResult(void);
float sensorGetO3Result(void);
float sensorGetDustResult(void);
float sensorGetCO2Result(void);

uint32_t sensorGetHumidity_TemperatureResult(void);
float sensorGetPressureResult(void);
float sensorGetAirVerhicleResult(void);
void setupSensorConfig(void);
void I2C_Configuration(void);
void SPI_Configuration(void);
uint8_t SPIx_Transfer(uint8_t data);
uint8_t getStatusPower(void);
uint8_t getValueSensorDevice(uint8_t * data,uint8_t device);
uint8_t getpower(void);

/*****************************************************************************
function:  Khai báo hàm cho SHT 75
argunent: float
return : uint8_t
description : function to get value  sensor
Create date :
user cread : 
****************************************************************************/
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
/*****************************************************************************
function:  Khai báo hàm cho MQ136
argunent: float
return : uint8_t
description : function to get value  sensor
Create date :
user cread : 
****************************************************************************/
void MQ136Setup(GPIO_TypeDef* MQ136Port, uint16_t MQ136Pin,uint8_t ADC_Channel);
uint16_t MQ136GetResult(void);


