#include "SHT75_Ver2.h"

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
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

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
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
uint16_t SHT_Temp(void)
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

/**
  * @brief  Configures 
  * @param  None
  * @retval None
  */
uint16_t SHT_Humi(void)
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