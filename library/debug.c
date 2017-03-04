/*
file:  communication.c
description : support for debug of device
Create date : 19/2/2017
user cread : Lu Van Cuong
*/
#include "debug.h"

char buffer[100];
uint8_t RXi;
uint8_t RXc;
uint8_t RX_FLAG_END_LINE = 0;
volatile char RX_BUF[RX_BUF_SIZE];

void Clear_RXBuffer(void)
{
	for(RXi=0; RXi<RX_BUF_SIZE; RXi++)
		RX_BUF[RXi] = '\0';
	RXi = 0;
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void USART_DMA_Init(unsigned int BaudRate)
{
	/* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Configure the GPIOs */
    GPIO_InitTypeDef GPIO_InitStructure;
 
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA */
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&buffer[0];
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize = sizeof(buffer);
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStruct);
 
    /* NVIC Configuration */
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	/* USART1 configuration */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
    USART_Init(USART1, &USART_InitStructure);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
 
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    //DMA_Cmd(DMA1_Channel4, ENABLE);
 
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
 
 
    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
    USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void SendUSART_DMA(const char *pucBuffer)
{
    strcpy(buffer, pucBuffer);
 
    /* Restart DMA Channel*/
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA1_Channel4->CNDTR = strlen(pucBuffer);
    DMA_Cmd(DMA1_Channel4, ENABLE);
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void SendUSART(USART_TypeDef* USARTx,uint16_t ch)
{
  USART_SendData(USARTx, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USARTx, USART_IT_TXE) == RESET)
  {}
}





int GetUSART(USART_TypeDef* USARTx)
{
  while (USART_GetFlagStatus(USARTx, USART_IT_RXNE) == RESET)
  {}
  return USART_ReceiveData(USARTx);
}
/*
GETCHAR_PROTOTYPE
{
  return GetUSART(USART1);
}
*/
/*
PUTCHAR_PROTOTYPE
{*/
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  //USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  //while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  //{}

  //return ch;
//}

void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
            RXc = USART_ReceiveData(USART1);
            RX_BUF[RXi] = RXc;
            RXi++;
 
            if (RXc != 13) 
            {
                if (RXi > RX_BUF_SIZE-1) 
                {
                    Clear_RXBuffer();
                }
            }
            else 
            {
                RX_FLAG_END_LINE = 1;
            }
 
            //Echo
            USART_SendData(USART1, RXc);
    }
}

void DMA1_Channel4_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_Cmd(DMA1_Channel4, DISABLE);
}


void showIntegerDebugDegit(int value){

}

void showFloatDebugDegit(float value){

}
void printDebugINT0(char *data){
  char *datarequest=data;
#ifdef USER_DEBUG
  while(*datarequest){
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugINT1(char *data,int value){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='d'){
        if(counter==0)
          showIntegerDebugDegit(value);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugINT2(char *data,int value,int value1){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='d'){
        if(counter==0)
          showIntegerDebugDegit(value);
        if(counter==1)
          showIntegerDebugDegit(value1);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugINT3(char *data,int value,int value1,int value2){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='d'){
        if(counter==0)
          showIntegerDebugDegit(value);
        if(counter==1)
          showIntegerDebugDegit(value1);
        if(counter==2)
          showIntegerDebugDegit(value2);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugFloat1(char *data,float value){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='f'){
        if(counter==0)
          showFloatDebugDegit(value);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugFloat2(char *data,float value,float value1){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='f'){
        if(counter==0)
          showFloatDebugDegit(value);
        if(counter==1)
          showFloatDebugDegit(value1);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}

void printDebugFloat3(char *data,float value,float value1,float value2){
  char *datarequest=data;
  char counter=0;
#ifdef USER_DEBUG
  while(*datarequest){
    if(*datarequest=='%'){
      *datarequest++;
      if(*datarequest=='f'){
        if(counter==0)
          showFloatDebugDegit(value);
        if(counter==1)
          showFloatDebugDegit(value1);
        if(counter==2)
          showFloatDebugDegit(value2);
        counter++;
      }
    }
   SendUSART(USART_DEBUG,(uint16_t) *datarequest);
    *datarequest++;
  }
#endif   
}