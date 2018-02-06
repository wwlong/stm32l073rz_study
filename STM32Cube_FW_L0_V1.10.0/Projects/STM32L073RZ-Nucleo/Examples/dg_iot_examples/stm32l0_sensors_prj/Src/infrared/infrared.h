#ifndef INFRARED_H_
#define INFRARED_H_

#ifdef __cplusplus
 extern "C" {
#endif 
extern volatile double infrared_dis;
 /* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"  
int infrared_init(void);
double get_infrared_measure_dis();
#ifdef __cplusplus
}
#endif

#endif 