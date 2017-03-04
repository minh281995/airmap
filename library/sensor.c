/*
file:  communication.c
description : manager all sensor of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/

#include "debug.h" 
#include "sensor.h"
#include "define.h"
DataSensorStruct m_datasensor;

uint16_t ADCValue[4] = {0};

void MQ136Setup(GPIO_TypeDef* MQ136Port, uint16_t MQ136Pin)
{
  	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStructure;
	uint32_t RCCPort;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(MQ136Port));
	assert_param(IS_GPIO_PIN(MQ136Pin));

	// enable clock for MQ136Port
	if(MQ136Port==GPIOA)
		RCCPort=RCC_APB2Periph_GPIOA;
	else if(MQ136Port==GPIOB)
		RCCPort=RCC_APB2Periph_GPIOB;
	else if(MQ136Port==GPIOC)
		RCCPort=RCC_APB2Periph_GPIOC;
	else if(MQ136Port==GPIOD)
		RCCPort=RCC_APB2Periph_GPIOD;
	else if(MQ136Port==GPIOE)
		RCCPort=RCC_APB2Periph_GPIOE;
	else if(MQ136Port==GPIOF)
		RCCPort=RCC_APB2Periph_GPIOF;
	else if(MQ136Port==GPIOG)
		RCCPort=RCC_APB2Periph_GPIOG;
	
	RCC->APB2ENR |= RCCPort;

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = MQ136Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(MQ136Port, &GPIO_InitStruct);
	
	/* Put everything back to power-on defaults */
	/* Enable ADC1 reset state */
	RCC->APB2RSTR |= RCC_APB2Periph_ADC1;
	/* Release ADC1 from reset state */
	RCC->APB2RSTR &= ~RCC_APB2Periph_ADC1;
	
	// enable clock for ADC1
	// to save energy, reduce clock frequency of ADC1
	RCC->APB2ENR |=RCC_APB2Periph_ADC1;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1, &ADC_InitStructure);
	// enable ADC1
	ADC1->CR2 |= CR2_ADON_Set;
	
	
	/*calibaration ADC1 before start conversation*/
	/* Resets the selected ADC calibration registers */  
	ADC1->CR2 |= CR2_RSTCAL_Set;
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC1->CR2 |= CR2_CAL_Set;
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
}
/*****************************************************************************
function:  sensorGetSO2Result
argunent: float
return : uint8_t
description : function to get value SO2 sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetSO2Result(uint8_t ADC_Channel)
{
    	assert_param(IS_ADC_CHANNEL(ADC_Channel));
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_71Cycles5);
	// start conversation
	ADC1->CR2 |= CR2_EXTTRIG_SWSTART_Set;
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
        return ADC1->DR;
//	return ADCValue[0]/2.2;
}
/*****************************************************************************
function:  sensorGetCOResult
argunent: float
return : uint8_t
description : function to get value CO sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetCOResult(void)
{       

	return ADCValue[0]/2.2;
}
/*****************************************************************************
function:  sensorGetO3Result
argunent: float
return : uint8_t
description : function to get value O3 sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetO3Result(void)
{
	return ADCValue[0]/2.2;
}
/*****************************************************************************
function:  sensorGetDustResult
argunent: float
return : uint8_t
description : function to get value Dust sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetDustResult(void)
{
	return ADCValue[0]/1.2;
}
/*****************************************************************************
function:  sensorGetCO2Result
argunent: float
return : uint8_t
description : function to get value CO2 sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetCO2Result(void)
{
	return ADCValue[0]/3.3;
}
/*****************************************************************************
function:  sensorGetHumidity_TemperatureResult
argunent: uint32_t
return : uint8_t
description : function to get value temperature and himidity sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
uint32_t sensorGetHumidity_TemperatureResult(void)
{
	return 0;
}
/*****************************************************************************
function:  sensorGetPressureResult
argunent: float
return : uint8_t
description : function to get value pressure air sensor
Create date : 24/2/2017
user cread : Trinh The Loc

****************************************************************************/
uint32_t sensorGetSHT75TemperatureResult(void)
{
	int16_t  iAck;
	int16_t	 iIOByte;
	
	SHT_Start();						//alert SHT75
	iAck = SHT_WriteByte(0x03);			// send measure temp command and ack status
	if(iAck==1) return(1);
	SHT_Wait();							//wait for SHT75 measure to comolete
	iIOByte = SHT_ReadByte();			//read SHT75 temp data 
	return(iIOByte);
}
/*****************************************************************************
function:  sensorGetPressureResult
argunent: float
return : uint8_t
description : function to get value pressure air sensor
Create date : 24/2/2017
user cread : Trinh The Loc

****************************************************************************/
uint32_t sensorGetSHT75HumidityResult(void)
{
	int16_t iAck;
	int16_t iIOByte;
	
	SHT_Start();							//alert SHT75
	iAck = SHT_WriteByte(0x05);				//send measure RH command and read ack status
	if(iAck==1) return(1);
	SHT_Wait();								//wait for SHT75 measurement to complete 	
	iIOByte = SHT_ReadByte();				//read SHT75 temp data 
	return(iIOByte);
}
/*****************************************************************************
function:  sensorGetPressureResult
argunent: float
return : uint8_t
description : function to get value pressure humidity sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetPressureResult(void)
{
	return ADCValue[0]/2.3;
}
/*****************************************************************************
function:  sensorGetAirVerhicleResult
argunent: float
return : uint8_t
description : function to get value SO2 sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
float sensorGetAirVerhicleResult(void)
{
	return ADCValue[0]/1.0;
}


/*****************************************************************************
function:  setupSensorConfig
argunent: void
return : void
description : setup All sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
****************************************************************************/
void setupSensorConfig(void){
  printDebug("START:void ADC_Config"); 
     ADC_InitTypeDef            	ADC_InitStructure;
     GPIO_InitTypeDef           	GPIO_InitStructure;
     DMA_InitTypeDef 	        DMA_InitStructure;					
    /* Enable clock DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* Enable clock ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    
    /* Configure Pin */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* DMA Configure */ 
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCValue; // address of array data
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 3;  // kich thuoc mang du lieu tuong ung so phan tu cua ADCValue
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    /* DMA1_Stream0 enable */ 
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    /* ADC Common Init */
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 3;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* DMA */ 
    /* ADC1 regular channels configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5);

    /* Enable ADC DMA */ 
    ADC_DMACmd(ADC1, ENABLE);
    
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibration register */  
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));
    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  printDebug("End:void ADC_Config"); 
}

void I2C_Configuration(void){
  printDebug("START:void I2C_Configuration"); 
GPIO_InitTypeDef GPIO_InitStructure;
I2C_InitTypeDef I2C_InitStructure;
#ifdef FAST_I2C_MODE
#define I2C_SPEED 400000
#define I2C_DUTYCYCLE I2C_DutyCycle_16_9  
#else /* STANDARD_I2C_MODE*/
#define I2C_SPEED 100000
#define I2C_DUTYCYCLE I2C_DutyCycle_2
#endif /* FAST_I2C_MODE*/
  
  /* Enable Clock GPIOB */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  /* Enable Clock I2C1 */ 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
  
  /* GPIO I2C1 Configure - PB8 - SCL & PB9 - SDA*/	
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // operate in Open Drain state
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // enable pull up resistors
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);


	
/************************************* MASTER ******************************/
  /* I2C De-initialize */
  I2C_DeInit(I2C1); // reset I2C default
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // we have 2 mode standard: I2C & SMBus, we choose I2C
  I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE; 
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &I2C_InitStructure);	
  /* I2C ENABLE */
  I2C_Cmd(I2C1, ENABLE); 
  printDebug("End:void I2C_Configuration"); 
}

void SPI_Configuration(void){
  printDebug("START:void SPI_Configuration"); 
    /* Initialization struct */
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
	
    /* Step 1: Initialize SPI Master */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    SPI_Init(SPI1, &SPI_InitStruct);
	
    SPI_Cmd(SPI1, ENABLE);
    /* Step 2: Initialize GPIO */
    RCC_AHB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* GPIO pins for MOSI, MISO, and SCK */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* GPIO pin for SS */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* Disable SPI slave device */
    GPIOA->BSRR = GPIO_Pin_15;
  printDebug("End:void SPI_Configuration"); 
}

uint8_t SPIx_Transfer(uint8_t data){
  printDebug("START:uint8_t SPIx_Transfer"); 
    // Write data to be transmitted to the SPI data register
    SPI1->DR = data;
    // Wait until transmit complete
    while (!(SPI1->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPI1->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPI1->SR & (SPI_I2S_FLAG_BSY));
    // Return received data from SPI data register
  printDebug("End:uint8_t SPIx_Transfer"); 
    return SPI1->DR;
}
 

uint8_t getStatusPower(void){
  printDebug("START:uint8_t getStatusPower"); 

  printDebug("End:uint8_t getStatusPower"); 
  return 0;
}

/******************************************************************
function:  getValueSensorDevice
argunent: uint8_t * data,uint8_t device
return : uint8_t
description : function to get value sensor noitice type sensor
Create date : 19/2/2017
user cread : Lu Van Cuong
*******************************************************************/
uint8_t getValueSensorDevice(uint8_t * data,uint8_t device){
  uint32_t dataraw=0;
  printDebug("START:uint8_t getValueDevice"); 
  switch(device){
      case SENSOR_ID_CO :          { 
            m_datasensor.COdata=sensorGetCOResult(); 
            break;}
      case SENSOR_ID_CO2 :         { 
            m_datasensor.CO2data=sensorGetCO2Result(); 
            break;}
      case SENSOR_ID_SO2 :         { 
            m_datasensor.SO2data=sensorGetSO2Result(); 
            break;}
      case SENSOR_ID_O3 :         { 
            m_datasensor.O3data=sensorGetO3Result(); 
            break;}
      case SENSOR_ID_TEMP_HUMI :   { 
            dataraw=sensorGetHumidity_TemperatureResult();
            m_datasensor.temperature=(uint16_t)(dataraw&0x0000ffff);
            m_datasensor.humidity=(uint16_t)((dataraw>>16)&0x0000ffff);
            break;}
      case SENSOR_ID_PRESSURE :    { 
            m_datasensor.pressure=sensorGetPressureResult(); 
            break;}
      case SENSOR_ID_DUST :        { 
            m_datasensor.PM2_5data=sensorGetDustResult(); 
            break;}
      case SENSOR_ID_VERHICLE :    { 
            m_datasensor.verhicle=sensorGetAirVerhicleResult(); 
            break;}
      default: { break; }
  }
  
  data=(uint8_t *) &m_datasensor;
  printDebug("End:uint8_t getValueDevice"); 
  return 0;
}
/******************************************************************
function:  getpower
argunent: void
return : uint8_t
description : function to get value power
Create date : 19/2/2017
user cread : Lu Van Cuong
*******************************************************************/
uint8_t getpower(void){
 printDebug("START:getpower"); 
 
 printDebug("START:getpower"); 
 return LOW_POWER;
/******************************************************************
function:  read and write data, wait and reset and readData and Init from SHT75
argunent: void
return : uint8_t
description : function to get 
Create date : 
user cread : 
*******************************************************************/
 void SHT_Start(void)
{
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);
	
	SHT_DATA_H;		//Data high
	SHT_CLK_L;		//Clk low
	//Delay(1us)	 	// 1us
	SHT_CLK_H;		//clk high
	//Delay(1us)		// 1us
	SHT_CLK_L;		//data low
	//Delay(1us)		// 1us
	SHT_CLK_L;		//clk low
	//Delay(1us)		// 1us
	SHT_CLK_H;		//clk high
	//Delay(1us)		// 1us
	SHT_DATA_H;			//data high
	//Delay(1us)		// 1us
	SHT_CLK_L;		//clk low
	//Delay(1us)		// 1us
}
uint8_t  SHT_WriteByte(uint8_t Data)
{
	uint8_t i, mask = 0x80;
	int16_t iAck;
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);	//Data Out
	//Delay(2us)
	for(i=0; i<8; i ++)
	{
		SHT_CLK_L;					//clk low
		if((Data&mask)>0) 
			SHT_DATA_H	;			//data high
		else
			SHT_DATA_L	;			//data low
		//Delay(2us)
		SHT_CLK_L;                                     //clk low
		//Delay(2us);
		mask = mask >> 1;
	}
	SHT_CLK_L;			//clk low
	//Delay(2us);
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_IN);
	iAck = DATA_IN;
	SHT_CLK_H;			//clk high
	//Delay(2us);
	SHT_CLK_L;			//clk low
	return(iAck);
}

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
uint8_t  SHT_ReadByte(void)
{
	int8_t i;
	int16_t iIOByte;
	const int16_t mask0 = 0x0000;
	const int16_t mask1 = 0x0001;
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_IN);
	for(i=0; i<8; i++)
	{
		iIOByte = iIOByte << 1;
		SHT_CLK_H;		//clk high
		//Delay(2us);
		if(DATA_IN) 
			iIOByte |= mask1;
		else 
			iIOByte |= mask0;
		SHT_CLK_L;		//clk low
		//Delay(2us);
	}
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);
	SHT_DATA_L; 			//data low
	//Delay(2us);
	SHT_CLK_H;			//clk high
	//Delay(2us);
	SHT_CLK_L;			//clk low
	//Delay(2us);
	SHT_DATA_H;			//data high
	
	//shift in LSB data
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_IN);
	for(i=0; i<8; i++)
	{
		iIOByte = iIOByte <<1;
		SHT_CLK_H;				//clk high
		//Delay(2us);
		if(DATA_IN) 
			iIOByte |= mask1;
		else 
			iIOByte |= mask0;
		SHT_CLK_L;			//clk low
		//Delay(2us);
	}
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);
	SHT_DATA_H;			//data high
	//Delay(2us);
	SHT_CLK_H;			//clk high
	//Delay;
	SHT_CLK_L;			//clk low
	
	return(iIOByte);
}
/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
void SHT_Wait(void)
{
	int16_t iDelay;
	
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);
	SHT_DATA_H;			//data high
	SHT_CLK_L;			//clk low
	//Delay
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_IN);
	for(iDelay=0; iDelay<30000;iDelay++)
	{
		if(!DATA_IN) break;			//if sht_data_pin low, SHT75 ready 
		//Delay(10us);
	}
}

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
void SHT_Reset(void)
{
	int8_t i;
	
	GPIO_Direction(GPIOB, GPIO_Pin_12, DIR_OUT);
	SHT_DATA_H;			//data high
	SHT_CLK_L;			// clk low
	//Delay(2);
	for(i=0; i<9; i++)
	{
		SHT_CLK_H;			//clk high
		//Delay(2);
		SHT_CLK_L;			//clk low
		//Delay(2);
	}
}
/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
void SHT_CalculateData(int16_t temp, int16_t humid, float *tc, float *rhlin, float *rhtrue)
{
	float fTrueHumi1, rh;
	
	//calculate temperature reading
	//tc = (float) ((temp*0.01)) - 40.0;
	//calculate Real RH reading 
	rh =  humid;
	
	//rhlin = (rh*0.0405) - (rh*rh*0.000028) - 4.0;
	
	//calculate True RH reading 
	//rhtrue = ((tc - 25.0) * (0.01 + (0.00008 * rh))) + rhlin; 
}

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
void SHT_ReadData(float *temp, float *truehumi)
{
	int16_t iResTemp, iResHumi;
	float fRealHumi;
	iResTemp = 0; truehumi = 0;
	
	iResTemp = SHT_Temp();		// measure temp
	iResHumi = SHT_Humi();		//measure RH
	SHT_CalculateData(iResTemp, iResHumi, temp, fRealHumi, truehumi);
}

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
void SHT_Init(void)
{
	SHT_Reset(); 		//reset SHT75
	//Delay(20ms);
}
/******************************************************************
function:  setUp for MQ136
argunent: void
return : uint8_t
description : function to get 
Create date : 
user cread :
******************************************************************/
void MQ136Setup(GPIO_TypeDef* MQ136Port, uint16_t MQ136Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStructure;
	uint32_t RCCPort;
	/* Check the parameters */
	assert_param(IS_GPIO_ALL_PERIPH(MQ136Port));
	assert_param(IS_GPIO_PIN(MQ136Pin));

	// enable clock for MQ136Port
	if(MQ136Port==GPIOA)
		RCCPort=RCC_APB2Periph_GPIOA;
	else if(MQ136Port==GPIOB)
		RCCPort=RCC_APB2Periph_GPIOB;
	else if(MQ136Port==GPIOC)
		RCCPort=RCC_APB2Periph_GPIOC;
	else if(MQ136Port==GPIOD)
		RCCPort=RCC_APB2Periph_GPIOD;
	else if(MQ136Port==GPIOE)
		RCCPort=RCC_APB2Periph_GPIOE;
	else if(MQ136Port==GPIOF)
		RCCPort=RCC_APB2Periph_GPIOF;
	else if(MQ136Port==GPIOG)
		RCCPort=RCC_APB2Periph_GPIOG;
	
	RCC->APB2ENR |= RCCPort;

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = MQ136Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(MQ136Port, &GPIO_InitStruct);
	
	/* Put everything back to power-on defaults */
	/* Enable ADC1 reset state */
	RCC->APB2RSTR |= RCC_APB2Periph_ADC1;
	/* Release ADC1 from reset state */
	RCC->APB2RSTR &= ~RCC_APB2Periph_ADC1;
	
	// enable clock for ADC1
	// to save energy, reduce clock frequency of ADC1
	RCC->APB2ENR |=RCC_APB2Periph_ADC1;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1, &ADC_InitStructure);
	// enable ADC1
	ADC1->CR2 |= CR2_ADON_Set;
	
	
	/*calibaration ADC1 before start conversation*/
	/* Resets the selected ADC calibration registers */  
	ADC1->CR2 |= CR2_RSTCAL_Set;
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibaration */
	ADC1->CR2 |= CR2_CAL_Set;
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
        /* kiem tra thong tin*/
}