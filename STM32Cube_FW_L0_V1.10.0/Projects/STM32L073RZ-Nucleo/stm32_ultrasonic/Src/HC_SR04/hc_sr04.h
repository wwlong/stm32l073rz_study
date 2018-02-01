#ifndef HC_SR04_H_
#define HC_SR04_H_

#ifdef __cplusplus
 extern "C" {
#endif 
   
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint32_t uwPrescalerValue;
/* Exported macro ------------------------------------------------------------*/
/* ultrasonic ӽޅƤ׃*/
#define US_TRIG_PIN GPIO_PIN_5
#define US_ECHO_PIN GPIO_PIN_6
#define US_TRIG_PORT GPIOC
#define US_ECHO_PORT GPIOC
/* Exported functions ------------------------------------------------------- */
/*
	hc_src04的驱动
	init
		gpio选择和配置
		gpio的timer设置
	发送波形
	接收波形
	根据接收的数据计算结果
*/
   /*hc-sr04 int*/
int hc_sr04_init(void);
/*
	计时
*/
int timing(void);
/*
	calculate distance
	int us，毫秒
	floart centigrade,当前环境温度，单位：摄氏度
	return:距离，单位：mm
*/
double cal_distance(int us, double centigrade);

int timer_init(void);
/*
	
*/
//
#ifdef __cplusplus
}
#endif

#endif 