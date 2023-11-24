#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdint.h"
  
void dhtpin_output(void);
void dhtpin_input(void);
size_t dht_read(void);

#ifdef __cplusplus
}
#endif

#endif 