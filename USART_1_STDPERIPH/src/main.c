
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;
char data[50];
char rx_buff[50];
int i=0;
uint32_t count=0;
void delay_ms(uint32_t time)
{
	count=time;
	while(count);
}
void CLK_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

}

void GPIO_Config(void)
{
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;

	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2); //Tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);  //Rx

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_2 |GPIO_Pin_3;

	GPIO_Init(GPIOA,&GPIO_InitStruct);



}

void USART_Config(void)
{
	USART_InitStruct.USART_BaudRate=9600;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;

	USART_Init(USART2,&USART_InitStruct);

	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

	USART_Cmd(USART2,ENABLE);
}

void NVIC_Config(void)
{
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;

	NVIC_Init(&NVIC_InitStruct);
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		char rx_buffer;
		rx_buffer=USART_ReceiveData(USART2);
		if(rx_buffer!='\n')
		{
		   rx_buff[i]=rx_buffer;
		   i++;
		}

		else
		{
			rx_buff[i]=rx_buffer;
			USART_Puts(USART2,rx_buff);
			for(int j=0; j<=50;j++)
			{
				rx_buff[j]='\0';

			}
			i=0;
		}
	}

}
void USART_Puts(USART_TypeDef* USARTx, volatile char *str)
{
	while(*str)
	{
	    while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
        USART_SendData(USARTx,*str);
		*(str++);
	}
}
int main(void)
{
	CLK_Config();
	GPIO_Config();
	USART_Config();
	NVIC_Config();
    SysTick_Config(SystemCoreClock/1000);
    sprintf(data,"hello user ,I am your board..:D\n");
    USART_Puts(USART2,data);
  while (1)
  {


  }
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
