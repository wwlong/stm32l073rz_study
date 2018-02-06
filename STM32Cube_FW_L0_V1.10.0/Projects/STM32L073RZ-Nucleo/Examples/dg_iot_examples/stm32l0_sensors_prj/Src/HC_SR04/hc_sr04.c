#include "hc_sr04.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef    TimHandle;
extern uint32_t timer_cnt1;
extern uint32_t timer_cnt2;
double ultrasonic_distance;	//mm
double ultrasonic_distance_max;	//mm
double ultrasonic_distance_min;	//mm
double ultrasonic_distance_sum;	//mm
double ultrasonic_distance_ave;	//mm
uint32_t ultrasonic_time; 	//ms
uint32_t ECHO_HIGH_TIMES = 0;
extern GPIO_PinState US_TRIG_PIN_VALUE;
extern GPIO_PinState US_ECHO_PIN_VALUE;
extern uint32_t ECHO_TIME;
extern uint8_t die_temp;
/* Private functions ---------------------------------------------------------*/
static double sonic_speed_in_air(int8_t degrees_celsius);
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
static double sonic_speed_in_air(int8_t degrees_celsius) {
  double speed = (331 + 0.607 * degrees_celsius)*1000;
  return speed;
}

/*
calculate distance
int us，微秒
int8_t centigrade,当前环境温度，单位：摄氏度
return:距离，单位：mm
*/
double sonic_speed;
//extern float distance_from_ultrasonic;
double cal_distance(int32_t us, int8_t centigrade){
  sonic_speed = sonic_speed_in_air(centigrade) / 1000000; //mm/s --> mm / us
  ultrasonic_distance = sonic_speed * us;
  return ultrasonic_distance; // >>1
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

/*
    通过多次取值计算平均值发测距
    取5次的均值
*/
double get_ultrasonic_distance() {
    ultrasonic_distance_max = 0;
    ultrasonic_distance_min = 0xffff;
    int cnt = 0;
    ECHO_HIGH_TIMES = 0;
    ultrasonic_distance_sum = 0;
    while(cnt < 5) {
         __HAL_TIM_SET_COUNTER(&TimHandle, 0);
        timer_cnt1 = __HAL_TIM_GET_COUNTER(&TimHandle);
        //US_TRIG_PIN_VALUE = HAL_GPIO_ReadPin(US_TRIG_PORT, US_TRIG_PIN);
        HAL_GPIO_WritePin(US_TRIG_PORT, US_TRIG_PIN, GPIO_PIN_SET);
        //US_TRIG_PIN_VALUE = HAL_GPIO_ReadPin(US_TRIG_PORT, US_TRIG_PIN);
        //US_ECHO_PIN_VALUE = HAL_GPIO_ReadPin(US_TRIG_PORT, US_ECHO_PIN);
        while(1) {
            timer_cnt2 = __HAL_TIM_GET_COUNTER(&TimHandle);
            if(timer_cnt2 - timer_cnt1 >= 10) {
                break;
            }
        }
        //US_TRIG_PIN_VALUE = HAL_GPIO_ReadPin(US_TRIG_PORT, US_TRIG_PIN);
        HAL_GPIO_WritePin(US_TRIG_PORT, US_TRIG_PIN, GPIO_PIN_RESET);
        
        /* 计算ECHO处于高电平的时间*/ 
        while(ECHO_HIGH_TIMES < 10000) {
            if(GPIO_PIN_SET == HAL_GPIO_ReadPin(US_ECHO_PORT, US_ECHO_PIN)) {
                timer_cnt1 = __HAL_TIM_GET_COUNTER(&TimHandle); 
                break;
            }
            ECHO_HIGH_TIMES ++;
        }
        if(ECHO_HIGH_TIMES >= 10000) {
            continue;
        }
        
        while(1) {
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(US_ECHO_PORT, US_ECHO_PIN)) {
                timer_cnt2 = __HAL_TIM_GET_COUNTER(&TimHandle); 
                break;
            }
        }
        ECHO_TIME = timer_cnt2 - timer_cnt1;
        ultrasonic_distance = cal_distance(ECHO_TIME, die_temp)/2;
        US_ECHO_PIN_VALUE = HAL_GPIO_ReadPin(US_ECHO_PORT, US_ECHO_PIN);
        /* 找极值 */
        if(ultrasonic_distance_min > ultrasonic_distance) {
            ultrasonic_distance_min = ultrasonic_distance;
        }
        if(ultrasonic_distance_max < ultrasonic_distance) {
            ultrasonic_distance_max = ultrasonic_distance;
        }
        /* 算总和 */
        ultrasonic_distance_sum += ultrasonic_distance;
        cnt ++;
    }
    ultrasonic_distance_ave = ultrasonic_distance_sum / cnt;
    return ultrasonic_distance_ave;
}