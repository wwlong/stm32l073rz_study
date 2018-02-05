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
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(US_TRIG_PORT, &GPIO_InitStructure);
  /*pc6--echo */
  GPIO_InitStructure.Pin = US_ECHO_PIN;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(US_ECHO_PORT, &GPIO_InitStructure);
  return 0;
}

/*
声音在空气中传播速度计算
double degrees_celsius，摄氏度，℃
return double,mm/us
*/
static double sonic_speed_in_air(double degrees_celsius) {
  double speed = (331 + 0.607 * degrees_celsius)*1000;
  return speed;
}

/*
calculate distance
int us，微秒
floart centigrade,当前环境温度，单位：摄氏度
return:距离，单位：mm
*/
double sonic_speed;
extern float distance_from_ultrasonic;
double cal_distance(int us, double centigrade){
  sonic_speed = sonic_speed_in_air(centigrade) / 1000000; //mm/s --> mm / us
  distance_from_ultrasonic = sonic_speed * us;
  return distance_from_ultrasonic; // >>1
}

int timer_init(void) {
  TIM_HandleTypeDef TimHandle;
  /* Set TIMx instance */
  TimHandle.Instance = TIM2;
  
  /* Compute the prescaler value to have TIMx counter clock equal to 1000 kHz */
  uint32_t uwPrescalerValue = ((SystemCoreClock) / 1000000) - 1;
  TimHandle.Init.Period            = SystemCoreClock - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
	
  }
  __HAL_TIM_ENABLE(&TimHandle);
  HAL_Delay(100);
  int32_t time_cnt = __HAL_TIM_GET_COUNTER(&TimHandle);
  return 0;
}