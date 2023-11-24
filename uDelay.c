#include "uDelay.h"
#include "main.h"


TIM_HandleTypeDef htim4;



void uDelay(uint32_t time_us)
{
  TIM4->CNT = 0;
  while(TIM4->CNT < time_us);
    
}

void Delay_ms(int time_ms)
{
  for(int i = 0; i < time_ms; i++)
    uDelay(1000);
}

void setTime(uint16_t time)
{
  //HAL_TIM_Base_Start(&htim4);
  TIM4->CNT = time;
 
}

uint16_t getTime(void)
{  
//  while (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) == RESET);
//  uint16_t currentTime = __HAL_TIM_GET_COUNTER(&htim4);
//  return currentTime;
  return __HAL_TIM_GetCounter(&htim4);
}

void Tim4_Init(void)
{

RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
//RCC->APB1ENR |= (1<<2);  // Enable the timer6 clock
	
  TIM4->PSC = 72-1;  // 90MHz/90 = 1 MHz ~~ 1 uS delay
  TIM4->ARR = 0xffff;  // MAX ARR value
  TIM4->CR1 |= TIM_CR1_CEN;
//TIM4->CR1 |= (1<<0); // Enable the Counter
//while (!(TIM4->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
  while (!(TIM4->SR & TIM_SR_UIF));
  TIM4->SR &= ~TIM_SR_UIF;
}
