#include "hc_sr04.h"
static double sonic_speed_in_air(double degrees_celsius);
/*
	HC_SRC04 driver
	GPIO初始化
*/
int hc_sr04_init(void)
{
  /*
  	配置hc-sr04 GPIO
   */
    /* Enable GPIOC clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  /*pc5--trig*/
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pin = US_TRIG_PIN;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  HAL_GPIO_Init(US_TRIG_PORT, &GPIO_InitStructure);
  /*pc6--echo */
  GPIO_InitStructure.Pin = US_ECHO_PIN;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(US_ECHO_PORT, &GPIO_InitStructure);
  return 0;
}

/*
	声音在空气中传播速度计算
	double degrees_celsius，摄氏度，℃
	return double,mm/s
*/
static double sonic_speed_in_air(double degrees_celsius) {
	double sonic_speed = (331 + 0.607 * degrees_celsius)*1000;
	return sonic_speed;
}

int timing(void) {

}
/*
	calculate distance
	int ms，毫秒
	floart centigrade,当前环境温度，单位：摄氏度
	return:距离，单位：mm
*/
double distance(int ms, double centigrade){
	double sonic_speed = sonic_speed_in_air(centigrade) / 1000; //mm/s --> mm / ms
	return ((sonic_speed * ms) / 2); // >>1
}

int timer_init(void) {
	TIM_HandleTypeDef TimHandle;
//	TimHandle.Instance = TIM2;
//	TimHandle.Init.Period        = SystemCoreClock - 1;
//	TimHandle.Init.Prescaler     = 0;
//	TimHandle.Init.ClockDivision = 0;
//	TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
//	HAL_TIM_Base_Init(&TimHandle);
//	HAL_TIM_Base_Start(&TimHandle);
//	__HAL_TIM_ENABLE(&TimHandle);
//	HAL_Delay(10000);
//	int32_t time_cnt = __HAL_TIM_GET_COUNTER(&TimHandle);
	
	  /* Set TIMx instance */
  TimHandle.Instance = TIM2;

  /* Initialize TIMx peripheral as follows:
       + Period = 2500 - 1
       + Prescaler = (SystemCoreClock/2500) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = SystemCoreClock - 1;
  TimHandle.Init.Prescaler         = 0;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;

  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {

  }
  __HAL_TIM_ENABLE(&TimHandle);
	HAL_Delay(100);
	int32_t time_cnt = __HAL_TIM_GET_COUNTER(&TimHandle);
}