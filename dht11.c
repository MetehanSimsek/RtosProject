#include "dht11.h"
#include "uDelay.h"
#include "main.h"

#define Dht_Data_Pin            GPIO_PIN_14
#define Dht_Data_PORT           GPIOC
#define response_byte           40
#define humanity_first_bit      0 
#define humanity_last_bit       8
#define temperature_first_bit   16
#define temperature_last_bit    24


static int mcu_start_signal(void);
static int Receiving_Data(void);

int response[40] = {0},bit;
int temperature, humidity;
static uint16_t time;


void dhtpin_output(void)
{

  GPIO_InitTypeDef dht11_pin = {0} ;
  
  dht11_pin.Pin   =  Dht_Data_Pin;
  dht11_pin.Mode  =  GPIO_MODE_OUTPUT_PP;
  dht11_pin.Pull  =  GPIO_NOPULL;
  dht11_pin.Speed =  GPIO_SPEED_FREQ_LOW;
  
  HAL_GPIO_Init(Dht_Data_PORT,&dht11_pin);
}


void dhtpin_input(void)
{

  GPIO_InitTypeDef dht11_pin = {0} ;
  
  dht11_pin.Pin   =  Dht_Data_Pin;
  dht11_pin.Mode  =  GPIO_MODE_INPUT;
  dht11_pin.Pull  =  GPIO_NOPULL;
  dht11_pin.Speed =  GPIO_SPEED_FREQ_LOW;
  
  HAL_GPIO_Init(Dht_Data_PORT,&dht11_pin);
} 

static int mcu_start_signal(void)
{
  uint16_t Response_Low_Time, Response_High_Time;
  dhtpin_output();
  HAL_GPIO_WritePin(Dht_Data_PORT,Dht_Data_Pin,GPIO_PIN_RESET);
  HAL_Delay(18);
  //HAL_GPIO_WritePin(Dht_Data_PORT,Dht_Data_Pin,GPIO_PIN_SET);
  dhtpin_input();


   //__HAL_TIM_SET_COUNTER(&htim4, 0);
  setTime(0);
  while(HAL_GPIO_ReadPin(Dht_Data_PORT,GPIO_PIN_14) == GPIO_PIN_RESET) // 20 - 40 us
    if(getTime() > 500 ) 
      return 0;
  
  setTime(0);
  while(HAL_GPIO_ReadPin(Dht_Data_PORT,GPIO_PIN_14) == GPIO_PIN_RESET) // 80 us
    if(getTime() > 500 )  
      return 0;
  Response_Low_Time = getTime();
  
  setTime(0);
  while(HAL_GPIO_ReadPin(Dht_Data_PORT,GPIO_PIN_14) == GPIO_PIN_SET) // 80 us
    if(getTime() > 500)  
      return 0;
  Response_High_Time = getTime();

  if(Response_Low_Time < 75 && Response_Low_Time > 85 && Response_High_Time < 75 && Response_High_Time > 85)
      return 0;
  
  return 0;
}



static int Receiving_Data(void)
{

  
  for(int i = 0; i < response_byte; i++)
  {
  setTime(0);
    while(HAL_GPIO_ReadPin(Dht_Data_PORT,Dht_Data_Pin) == GPIO_PIN_RESET)
      if(getTime() > 100) return 0;
  setTime(0);
    while(HAL_GPIO_ReadPin(Dht_Data_PORT,Dht_Data_Pin) == GPIO_PIN_SET)
      if(getTime() > 100) return 0;
    
    time = getTime();
    if(time > 20 && time < 30)
      bit = 0;
    
    else if(time < 60 && time > 80)
      bit = 1;
  
    response[i] = bit;
  } 
  
  for(int i = humanity_first_bit; i < humanity_last_bit; i++)
  {
    humidity = response[i];
    humidity = humidity << 1;
  }
  
  for(int i = temperature_first_bit; i < temperature_last_bit; i++)
  {
    temperature = response[i];
    temperature = temperature << 1;
  }
  
  return 0;
}


size_t dht_read(void)
{
  mcu_start_signal();
  Receiving_Data();
return 1;
}