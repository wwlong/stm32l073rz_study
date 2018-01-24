/**
  @page FLASH_DualBoot  FLASH Dual Boot example (with workaround)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    FLASH/FLASH_DualBoot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FLASH Dual boot example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This example guides you through the different configuration steps by mean of HAL API on
how to program bank1 and bank2 of the STM32L0xx internal Flash memory mounted on STM32L073Z-EVAL
and possibly boot on one of them.

================================================================================
WARNING : This example is intended to be used only on old STM32L07xx/STM32L08xx 
device version (RevID previous to 0x2008) to demonstrate the proposed 
implementation which uses a SW workaround. The RevId can be retrieved via the 
function HAL_GetREVID().
================================================================================  

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 32 MHz. 
In this example, the user code is loaded in Flash memory (according to scatter file)
and DualBoot workaround run in RAM.

Below are the steps to run this example:
1- Choose the FLASH_DualBoot_Bank2_Boot project and generate its binary (ie: FLASH_DualBoot.bin)

2- You can use STM32 ST-LINK Utilities or any similar tool to initially reset the 
   BFB2 bit (disable the dual boot feature).
   
3- Load this binary at the bank2 of the flash(at the address 0x08018000) using 
   STM32 ST-LINK Utilities(www.st.com) or any similar tool. 
   
@note:
 - You can avoid step 1 by directly loading the binary file provided with the example
 - You have to configure your preferred toolchain in order to generate the binary
   file after compiling the project.  
   
4- using STM32 ST-LINK Utilities(www.st.com) or any similar tool
   pre-configure following values in registers:
 - To boot on Bank2: at the address DATA_VALUE_SELECT= 0x08080000 set DATA_BANK2_SELECTED= 0x00000037
   otherwise it will boot on Bank1 by default.
   
@note: at the address SYSCFG_MEMRMP_ADDRESS= 0x40010000 check that you have 0x00000000 (FLASH BOOT and Bank1 default)
@note: at the address 0x4002201C (FLASH_OPTR register) check that you have 0x807000AA (BTFB2=0 for RDP level0) 
       or 0x80700000 (BTFB2=0 for RDP level1) or 0x807000CC (BTFB2=0 for RDP level2).
@note: Just in case of Flash locked during pre-configuration, first run FLASH_Unlock() function.
  
5- Choose the FLASH_DualBoot_Bank1_Boot project and run it, this project will be loaded
   in the bank1 of the flash (default boot): at the address 0x08000000  
   
- If program in bank1 is selected, a message with a blue text back color will be 
  displayed on LCD and LED1 will remain toggling while LED2 is turn on.

- If program in bank2 is selected, a message with a red text back color will be 
  displayed on LCD and LED2 will remain toggling while LED1 is turn on.

- If error occurs LED3 is turn on.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - FLASH/FLASH_DualBoot/Inc/stm32l0xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_DualBoot/Inc/stm32l0xx_it.h              Header for stm32l0xx_it.c
  - FLASH/FLASH_DualBoot/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_DualBoot/Src/stm32l0xx_it.c              Interrupt handlers
  - FLASH/FLASH_DualBoot/Src/main.c                      Main program
  - FLASH/FLASH_DualBoot/Src/system_stm32l0xx.c          STM32L0xx system clock configuration file
  - FLASH/FLASH_DualBoot/Binary/FLASH_DualBoot.bin       Binary file to load at bank2

@par Hardware and Software environment

  - This example runs on STM32L07xx and STM32L08xx devices. (Refer to the warning at the beginning
    of this readme.txt)
    
  - This example has been tested with STM32L073Z-EVAL board and can be
    easily tailored to any other supported device and development board.
      
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
