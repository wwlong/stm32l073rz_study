/**
******************************************************************************
* @file    Templates/Src/main.c 
* @author  MCD Application Team
* @brief   Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
* @{
*/

/** @addtogroup Templates
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Counter Prescaler value */
uint32_t uwPrescalerValue = 0;
volatile double infrared_dis;
double pow_test;
GPIO_PinState US_TRIG_PIN_VALUE;
GPIO_PinState US_ECHO_PIN_VALUE;
uint32_t ECHO_TIME = 0;
float distance_from_ultrasonic;
float distance_return;
uint32_t timer_cnt1 = 0;
uint32_t timer_cnt2 = 0;
uint32_t timer_cnt_max = 0;
extern float output_voltage;
extern __IO uint32_t uwADCxConvertedValue;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void EXTI4_15_IRQHandler_Config(void);
/* Private functions ---------------------------------------------------------*/


/*
GPIO input test
设置中断
下降沿触发
出发中断后点亮LED2

GPIOC GPIO_PIN_6
*/
/**
* @brief  Configures EXTI Line13 (connected to PC13 pin) in interrupt mode.
* @param  None
* @retval None
*/
/**
* @brief  Configures EXTI lines 4 to 15 (connected to PC.13 pin) in interrupt mode
* @param  None
* @retval None
*/
static void EXTI4_15_IRQHandler_Config(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    
    /* Enable GPIOC clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* Configure PC.13 pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Enable and set EXTI lines 4 to 15 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
* @brief EXTI line detection callbacks
* @param GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_6)
    {
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
    }
}

/**
* @brief  Main program
* @param  None
* @retval None
*/
int main(void)
{
    /* STM32L0xx HAL library initialization:
    - Configure the Flash prefetch, Flash preread and Buffer caches
    - Systick timer is configured by default as source of time base, but user 
    can eventually implement his proper time base source (a general purpose 
    timer for example or other time source), keeping in mind that Time base 
    duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
    handled in milliseconds basis.
    - Low Level Initialization
    */
    HAL_Init();
    
    /* Configure the system clock to 2 MHz */
    SystemClock_Config();
    
    /* -1- Initialize LEDs mounted on STM32L073-Nucleo Rev A board */
    BSP_LED_Init(LED2);
    
    /* Set TIMx instance */
    TimHandle.Instance = TIM2;
    
    /* Compute the prescaler value to have TIMx counter clock equal to 1000 kHz */
    uwPrescalerValue = ((SystemCoreClock) / 1000000) - 1;
    TimHandle.Init.Period            = SystemCoreClock - 1;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    
    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        //Error_Handler();
    }
    __HAL_TIM_ENABLE(&TimHandle);
	
    /* init GPIOC PIN6 for GPIO input */
    hc_sr04_init();
    /* Add your application code here */
    EXTI4_15_IRQHandler_Config();
    
    infrared_init();
    
    mag3110_init();
    get_mag3110_id();
    i2c_read_mem(MAG3110_READ_ADDR, WHO_AM_I, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, DIE_TEMP, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_X_MSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_X_MSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_X_LSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_MSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_LSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_MSB_REG, &reg_data, &len);
    i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_LSB_REG, &reg_data, &len);
    /*
    准备接受数据
	*/
    
    while (1)
    {
        distance_from_ultrasonic = get_ultrasonic_distance();
        //HAL_Delay(10);
        infrared_dis = get_infrared_measure_dis();
        
        //mag3110 data
        i2c_read_mem(MAG3110_READ_ADDR, STATUS_00_REG, &reg_data, &len);
        //int8_t tmp = reg_data&ZYXDR_MASK;
        get_magnetometer_info();
        get_temperature();
        get_sysmode();
        mag3110_std();
    }
}

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow : 
*            System Clock source            = MSI
*            SYSCLK(Hz)                     = 2000000
*            HCLK(Hz)                       = 2000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            Flash Latency(WS)              = 0
*            Main regulator output voltage  = Scale3 mode
* @retval None
*/
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    
    /* Enable MSI Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
    RCC_OscInitStruct.MSICalibrationValue=0x00;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }
    
    /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
    clocked below the maximum system frequency, to update the voltage scaling value 
    regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
    
    /* Disable Power Control clock */
    __HAL_RCC_PWR_CLK_DISABLE();
    
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
