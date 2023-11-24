#include "ds1307.h"
#include "pcd8544.h"
#include "stdio.h"


#define  RTC_Write_Adress       0xD0
#define  RTC_Read_Adress        0xD1
#define  Second_addr            0x00
#define  Minutes_addr           0x01
#define  Hours_addr             0x02
#define  Day_addr               0x03
#define  Date_addr              0x04
#define  Month_addr             0x05
#define  Year_addr              0x06
#define  Control_addr           0x07
#define  RAM_first_addr         0x08

#define  AM_12Hours             0x00
#define  PM_12Hours             0x01
#define  Format24h              0x02

#define  One_byte               0x01

#define Clock_Adress            0x00

I2C_HandleTypeDef hi2c1;

typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
}rtc_time;

typedef struct {
  uint8_t  day;
  uint8_t  date;
  uint8_t  month;
  uint8_t  year;
}rtc_date;

static int bcd_to_dec(uint8_t variable);

rtc_time time;
rtc_date date;

uint8_t receive_variable;
uint8_t memory;

int write_to_eeprom(uint8_t send_data, uint16_t eeprom_addr)
{
  HAL_I2C_Mem_Write(&hi2c1,0xA0,eeprom_addr,2,&send_data,2,HAL_MAX_DELAY);
  return send_data;
}

int read_to_eeprom(uint16_t eeprom_addr)
{

  HAL_I2C_Mem_Read(&hi2c1,0xA1,eeprom_addr,2,&receive_variable,2,HAL_MAX_DELAY);
  return receive_variable;
}

void get_time(void)
{
  char ptr_sec[3], ptr_min[3], ptr_hour[3];

  HAL_I2C_Mem_Read(&hi2c1,RTC_Read_Adress,(uint16_t)Second_addr,One_byte,&(time.sec),One_byte,1);
  time.sec = bcd_to_dec(time.sec);
  HAL_I2C_Mem_Read(&hi2c1,RTC_Read_Adress,(uint16_t)Minutes_addr,One_byte,&(time.min),One_byte,1);
  time.min = bcd_to_dec(time.min);
  HAL_I2C_Mem_Read(&hi2c1,RTC_Read_Adress,(uint16_t)Hours_addr,One_byte,&(time.hour),One_byte,1);
  time.hour = bcd_to_dec(time.hour);
  
  sprintf(ptr_sec,"%02d",time.sec);
  sprintf(ptr_min,"%02d",time.min);
  sprintf(ptr_hour,"%02d",time.hour);
  lcdPrint(ptr_hour,0,0);
  lcdPrint(":",12,0);
  lcdPrint(ptr_min,17,0);
  lcdPrint(":",29,0);
  lcdPrint(ptr_sec,34,0);

 
  
}

static int bcd_to_dec(uint8_t variable)
{
    int dec_value = 0;
    dec_value += (variable & 0x0F); // Sagdaki 4 biti al
    dec_value += ((variable >> 4) & 0x0F) * 10; // Sol 4 biti al, 10 ile çarp
    return dec_value;
}

void ds1307_tim_date_set(uint8_t _sec, uint8_t _min, uint8_t _hour, uint8_t _day, uint8_t _month, uint8_t _year, uint8_t _date)
{
  
  memory = read_to_eeprom(Clock_Adress);
  if(memory == 0)
  {
    time.sec = _sec;
    set_sec(time.sec);     
    
    time.min = _min; 
    set_min(time.min);
    
    time.hour = _hour;
    set_hour(time.hour);
    
    date.day = _day; 
    set_day(date.day);      
    
    date.date = _date;  
    set_date(date.date);        
    
    date.month = _month;
    set_month(date.month);       
    
    date.year =  _year;
    set_year(date.year);      
    
    write_to_eeprom(0x01,Clock_Adress);
  }
}

void set_sec(uint8_t second)
{
  time.sec = second;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Second_addr,(uint16_t)One_byte,&time.sec,(uint16_t)One_byte,HAL_MAX_DELAY);
}

void set_min(uint8_t minute)
{
  time.sec = minute;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Minutes_addr,(uint16_t)One_byte,&time.min,(uint16_t)One_byte,HAL_MAX_DELAY);
}

void set_hour(uint8_t hour)
{
  time.hour = hour;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Hours_addr,(uint16_t)One_byte,&time.hour,(uint16_t)One_byte,HAL_MAX_DELAY);  
}

void set_day(uint8_t day)
{
  date.date = day;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Day_addr,(uint16_t)One_byte,&date.day,(uint16_t)One_byte,HAL_MAX_DELAY);  
}

void set_date(uint8_t _date)
{
  date.date = _date;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Date_addr,(uint16_t)One_byte,&date.date,(uint16_t)One_byte,HAL_MAX_DELAY); 
}

void set_month(uint8_t _month)
{
  date.month = _month;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Month_addr,(uint16_t)One_byte,&date.date,(uint16_t)One_byte,HAL_MAX_DELAY); 
}

void set_year(uint8_t _year)
{
  date.year = _year;
  HAL_I2C_Mem_Write(&hi2c1,(uint16_t)RTC_Write_Adress,(uint16_t)Year_addr,(uint16_t)One_byte,&date.year,(uint16_t)One_byte,HAL_MAX_DELAY);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

}