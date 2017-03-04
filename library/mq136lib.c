#include "mq136lib.h"

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

uint16_t MQ136GetResult(uint8_t ADC_Channel)
{
	assert_param(IS_ADC_CHANNEL(ADC_Channel));
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_71Cycles5);
	// start conversation
	ADC1->CR2 |= CR2_EXTTRIG_SWSTART_Set;
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC1->DR;
}





