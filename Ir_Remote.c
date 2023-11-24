#include "Ir_Remote.h"
#include "uDelay.h"
#include "main.h"


#define Ir_Pin          GPIO_PIN_8
#define Ir_Port         GPIOA
#define data_lenght     32




uint32_t count_time;


uint32_t receive_data(void)
{

  uint32_t data = 0;

  /* Start of Frame */
  while(!(HAL_GPIO_ReadPin(Ir_Port,Ir_Pin))); // 9ms waiting as low
  
  while((HAL_GPIO_ReadPin(Ir_Port,Ir_Pin)));  // 4.5ms  waiting as high
   
  for(int i = 0; i < data_lenght; i++)
    {
      count_time = 0;
      while (!(HAL_GPIO_ReadPin(Ir_Port,Ir_Pin))); // wait for pin to go high.. this is 562.5us LOW
      while ((HAL_GPIO_ReadPin(Ir_Port,Ir_Pin)))  // count the space length while the pin is high 
      {
        count_time++;
        uDelay(100);
      }
      
      if(count_time > 12)
      {
        data |= ( 1UL << (data_lenght-i)); // write 1;
      }
      else 
        data &= ~( 1UL << (data_lenght-i)); // write 0;
    }

  return data;
}

void Ir_pin_config(void)
{
  GPIO_InitTypeDef Ir_Io;
  
  HAL_GPIO_WritePin(Ir_Port,Ir_Pin,GPIO_PIN_RESET);
  
  Ir_Io.Mode  = GPIO_MODE_INPUT;
  Ir_Io.Pin   = GPIO_PIN_8;
  Ir_Io.Pull  = GPIO_NOPULL;
  Ir_Io.Speed = GPIO_SPEED_FREQ_LOW;
  
  HAL_GPIO_Init(Ir_Port, &Ir_Io);
}

