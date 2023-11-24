#ifndef __UDELAY_H
#define __UDELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "main.h"
  
void Tim4_Init(void);
void uDelay(uint32_t time_us);
void Delay_ms(int time_ms);
void setTime(uint16_t time);
uint16_t getTime(void);

#ifdef __cplusplus
}
#endif

#endif 