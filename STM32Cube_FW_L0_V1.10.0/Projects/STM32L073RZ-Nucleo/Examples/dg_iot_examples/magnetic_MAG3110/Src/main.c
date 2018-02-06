/**
******************************************************************************
* @file    I2C/I2C_TwoBoards_AdvComIT/Src/main.c
* @author  MCD Application Team
* @brief   This sample code shows how to use STM32L0xx I2C HAL API to transmit
*          and receive a data buffer with a communication process based on
*          IT transfer.
*          The communication is done using 2 Boards.
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
#include "mag3110.h"
/** @addtogroup STM32L0xx_HAL_Examples
* @{
*/

/** @addtogroup I2C_TwoBoards_AdvComIT
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
#define MASTER_BOARD
#define I2C_ADDRESS        0x3E
#define MASTER_REQ_READ    0x12
#define MASTER_REQ_WRITE   0x34


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT****  ****I2C_TwoBoards communication based on IT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
__IO uint16_t hTxNumData = 0;
__IO uint16_t hRxNumData = 0;
uint8_t bTransferRequest = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

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
    
    /* Configure the system clock to 32 MHz */
    SystemClock_Config();
    
    /* Configure LED2 */
    BSP_LED_Init(LED2);
	
    /* Configure User push-button */
    BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);
    
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
    
    while(1) {
        i2c_read_mem(MAG3110_READ_ADDR, STATUS_00_REG, &reg_data, &len);
        //int8_t tmp = reg_data&ZYXDR_MASK;
        get_magnetometer_info();
        get_temperature();
        get_sysmode();
        mag3110_std();
    }
    
    
//    //
//    /* Wait for User push-button press before starting the Communication */
//    while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET)
//    {
//    }
//    
//    /* Wait for User push-button release before starting the Communication */
//    while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_SET)
//    {
//    }
}

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow : 
*            System Clock source            = PLL (HSI)
*            SYSCLK(Hz)                     = 32000000
*            HCLK(Hz)                       = 32000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            Flash Latency(WS)              = 1
*            Main regulator output voltage  = Scale1 mode
* @retval None
*/
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
    clocked below the maximum system frequency, to update the voltage scaling value 
    regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    /* Disable Power Control clock */
    __HAL_RCC_PWR_CLK_DISABLE();
    
    /* Enable HSE Oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL4;
    RCC_OscInitStruct.PLL.PLLDIV      = RCC_PLL_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }
    
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
    {
        /* Initialization Error */
        while(1); 
    }
}

/**
* @brief  I2C error callbacks.
* @param  I2cHandle: I2C handle
* @note   This example shows a simple way to report transfer error, and you can
*         add your own implementation.
* @retval None
*/
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
    /** Error_Handler() function is called when error occurs.
    * 1- When Slave don't acknowledge it's address, Master restarts communication.
    * 2- When Master don't acknowledge the last data transferred, Slave don't care in this example.
    */
    if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
    {
        Error_Handler();
    }
}

/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
static void Error_Handler(void)
{
    /* Error if LED2 is slowly blinking (1 sec. period) */
    while(1)
    {
        BSP_LED_Toggle(LED2);
        HAL_Delay(1000);
    }
}

/**
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval 0  : pBuffer1 identical to pBuffer2
*         >0 : pBuffer1 differs from pBuffer2
*/
//static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//    while (BufferLength--)
//    {
//        if ((*pBuffer1) != *pBuffer2)
//        {
//            return BufferLength;
//        }
//        pBuffer1++;
//        pBuffer2++;
//    }
//    
//    return 0;
//}

/**
* @brief  Flushes the buffer
* @param  pBuffer: buffers to be flushed.
* @param  BufferLength: buffer's length
* @retval None
*/
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
