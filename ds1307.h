#pragma once
#ifndef __DS1307_H
#define __DS1307_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "main.h"  

void I2C1_Init(void);

void ds1307_tim_date_set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint8_t year, uint8_t date);
  
void set_sec(uint8_t second);
void set_min(uint8_t minute);
void set_hour(uint8_t hour);
  
void set_day(uint8_t day);
void set_date(uint8_t _date);
void set_month(uint8_t _month);
void set_year(uint8_t _year);  

void get_time(void);


int write_to_eeprom(uint8_t send_data, uint16_t eeprom_addr);
int read_to_eeprom(uint16_t eeprom_addr);

#ifdef __cplusplus
}
#endif

#endif 