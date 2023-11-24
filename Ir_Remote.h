#ifndef __IR_REMOTE_H
#define __IR_REMOTE_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "stdint.h"
  
uint32_t receive_data(void);
void Ir_pin_config(void);

#ifdef __cplusplus
}
#endif

#endif 