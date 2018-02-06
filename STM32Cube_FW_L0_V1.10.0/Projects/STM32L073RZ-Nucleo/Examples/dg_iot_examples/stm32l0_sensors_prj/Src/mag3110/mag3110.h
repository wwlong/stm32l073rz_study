#ifndef MAG3110_H_
#define MAG3110_H_

/* Define to prevent recursive inclusion -------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif 
    /* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"
    /* Exported macro ------------------------------------------------------------*/
    
    /* Exported types ------------------------------------------------------------*/
    /* I2C handler declared in "main.c" file */
    extern I2C_HandleTypeDef I2cHandle;
    /* Exported constants --------------------------------------------------------*/
    /* User can use this section to tailor I2Cx/I2Cx instance used and associated
    resources */
    /* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
    
#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()
    
    /* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1
    /* I2C TIMING Register define when I2C clock source is SYSCLK */
    /* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 32 MHz */
    //#define I2C_TIMING    0x10A13E56 /* 100 kHz with analog Filter ON, Rise Time 400ns, Fall Time 100ns */ 
#define I2C_TIMING      0x00B1112E /* 400 kHz with analog Filter ON, Rise Time 250ns, Fall Time 100ns */     
    /* Definition for I2Cx's NVIC */
#define I2Cx_IRQn                       I2C1_IRQn
#define I2Cx_IRQHandler                 I2C1_IRQHandler
    /* from main.h*/
    /* Size of Reception buffer */
#define MAG_TXBUFFERSIZE 256
#define MAG_RXBUFFERSIZE                    MAG_TXBUFFERSIZE
#define I2C_ADDRESS        0x3E
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"
    /* Exported types ------------------------------------------------------------*/
    typedef  struct single_byte {
        uint8_t msb;
        uint8_t lsb;
    }single_byte_t;
    typedef struct tri_axis_mag{
        single_byte_t x;
        single_byte_t y;
        single_byte_t z;
    }tri_axis_mag_t;
    
    /*
    TODO
    面向对象
    typedef struct mag3110{
    int8_t temperature;
    tri_axis_mag_t xyz;
}mag3110_t;
    */
    /* Exported constants --------------------------------------------------------*/
    extern I2C_HandleTypeDef I2cHandle;
    extern uint8_t bTransferRequest ;
    extern __IO uint16_t hRxNumData ;
    extern uint8_t reg_data;
    extern uint16_t len;
    /* end of from main.h*/
    
#define MAG3110_IIC_ADDR 0x0E
#define MAG3110_WRITE_ADDR 0x1C
#define MAG3110_READ_ADDR 0x1D
    
#define   DR_STATUS   0x00
    /* XYZ data registers*/
#define   OUT_X_MSB_REG   0x01
#define   OUT_X_LSB_REG   0x02
#define   OUT_Y_MSB_REG   0x03
#define   OUT_Y_LSB_REG   0x04
#define   OUT_Z_MSB_REG   0x05
#define   OUT_Z_LSB_REG   0x06
#define   WHO_AM_I        0x07
#define   SYSMOD          0x08
#define   OFF_X_MSB       0x09
#define   OFF_X_LSB       0x0A
#define   OFF_Y_MSB       0x0B
#define   OFF_Y_LSB       0x0C
#define   OFF_Z_MSB       0x0D
#define   OFF_Z_LSB       0x0E
#define   DIE_TEMP        0x0F
#define   CTRL_REG1       0x10
#define   CTRL_REG2       0x11
    
#define STANDBY_MASK 0x00
#define DR1_MASK 0x40
#define DATA_RATE_5MS DR1_MASK
    
#define ACTIVE_MASK  0x01
#define STATUS_00_REG 0x00
#define ZYXDR_MASK 0x08
#define MAG3110_ID 0x4c
    
    /* Exported functions ------------------------------------------------------- */
    /* mag3110 driver*/
    /*
    mode
    standby
    active
    */
    /*
    Initialize mag3110
    */
    void mag3110_init(void);
    void mag3110_standby(void);
    void mag3110_active(void);
    int32_t i2c_read(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len);
    int32_t i2c_write(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len);
    int32_t i2c_write_mem(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len);
    int32_t i2c_read_mem(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len);
    int32_t get_magnetometer_info(void);
    int32_t get_temperature(void);
    int32_t get_sysmode(void);
    int8_t get_mag3110_id(void);
    int32_t mag3110_std(void);
#ifdef __cplusplus
}
#endif
#endif