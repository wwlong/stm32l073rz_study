/* Includes ------------------------------------------------------------------*/
#include  "mag3110.h"
#include <string.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
int8_t mag_reg_data;
uint8_t reg_data;
uint16_t len=1;
//uint8_t dev_addr = MAG3110_IIC_ADDR;
//tri_axis_mag_t mag3110_mag;
uint8_t dr_status;
uint8_t out_x_msb;
uint8_t out_x_lsb;
uint8_t out_y_msb;
uint8_t out_y_lsb;
uint8_t out_z_msb;
uint8_t out_z_lsb;

uint16_t out_x;
uint16_t out_y;
uint16_t out_z;
uint16_t out_xmin;
uint16_t out_ymin;
uint16_t out_zmin;
uint16_t out_xmax;
uint16_t out_ymax;
uint16_t out_zmax;

uint8_t mag3110_id;
uint8_t sysmode;
uint8_t off_x_msb;
uint8_t off_x_lsb;
uint8_t off_y_msb;
uint8_t off_y_lsb;
uint8_t off_z_msb;
uint8_t off_z_lsb;
uint16_t off_x;
uint16_t off_y;
uint16_t off_z;

uint8_t die_temp;
uint8_t ctrl_reg1;
uint8_t ctrl_reg2;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
static void Error_Handler(void);
void mag3110_init(void)
{
    mag3110_standby();
    reg_data = DATA_RATE_5MS;
    i2c_write_mem(MAG3110_WRITE_ADDR, CTRL_REG1, &reg_data, &len);
    reg_data = 0;
    i2c_read_mem(MAG3110_READ_ADDR, CTRL_REG1, &reg_data, &len);
    mag3110_active();
    get_temperature();
}

void mag3110_standby(void)
{
    uint8_t reg_data;
    i2c_read_mem(MAG3110_READ_ADDR, CTRL_REG1, &reg_data, &len);
    reg_data = reg_data&0xFF|STANDBY_MASK;
    i2c_write_mem(MAG3110_WRITE_ADDR, CTRL_REG1, &reg_data, &len);
}

void mag3110_active(void) {
    i2c_read_mem(MAG3110_READ_ADDR, CTRL_REG1, &reg_data, &len);
    reg_data = reg_data&0xFC|ACTIVE_MASK;
    i2c_write_mem(MAG3110_WRITE_ADDR, CTRL_REG1, &reg_data, &len);
    reg_data = 0x00;
    i2c_read_mem(MAG3110_READ_ADDR, CTRL_REG1, &reg_data, &len);
}
/*
读寄存器的值
*/
int32_t i2c_read(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len) {
    /*
    读取REGISTER寄存器的值
    1.  向读寄存器(i2c_addr)写reg_addr寄存器的地址
    2.  从读寄存器(i2c_addr)读取传输过来的reg_addr寄存器的值
    */
	//step1
 	i2c_addr = MAG3110_READ_ADDR;
    do
    {
        if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)&reg_addr, sizeof(uint8_t))!= HAL_OK)
        {
            /* Error_Handler() function is called when error occurs. */
            Error_Handler();
        }
        
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);
	//step2
    do
    {
        if(HAL_I2C_Master_Receive_IT(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)data, *len)!= HAL_OK)
        {
            /* Error_Handler() function is called when error occurs. */
            Error_Handler();
        }
        
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);
	
	return 0;
}
/*
读寄存器的值
*/
int32_t i2c_read_mem(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len) {
    /*
    读取REGISTER寄存器的值
    1.  向读寄存器(i2c_addr)写reg_addr寄存器的地址
    2.  从读寄存器(i2c_addr)读取传输过来的reg_addr寄存器的值
    */
	//step1
 	i2c_addr = MAG3110_READ_ADDR;
    do
    {
        if(HAL_I2C_Mem_Read_IT(&I2cHandle,(uint16_t)i2c_addr, (uint8_t)reg_addr,sizeof(uint8_t), data, sizeof(uint8_t))!=HAL_OK) 
        {
            /* Error_Handler() function is called when error occurs. */
            Error_Handler();
        }
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);
	return 0;
}
/*
写寄存器的值
*/
int32_t i2c_write(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len) {
    /* Update bTransferRequest to send buffer write request for Slave */
    i2c_addr = MAG3110_WRITE_ADDR;
	//dev_addr = MAG3110_IIC_ADDR;
    /*##-2- Master sends write request for slave #############################*/
    do
    {
        if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)&reg_addr, sizeof(uint8_t))!= HAL_OK)
        {
            /* Error_Handler() function is called when error occurs. */
            Error_Handler();
        }
        
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);    
    /*##-4- Master sends aTxBuffer to slave ##################################*/
    do
    {
        if(HAL_I2C_Master_Transmit_IT(&I2cHandle, (uint16_t)i2c_addr, (uint8_t*)data, *len)!= HAL_OK)
        {
            /* Error_Handler() function is called when error occurs. */
            Error_Handler();
        }
        
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);
    
	return 0;
}
//HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)

int32_t i2c_write_mem(uint16_t i2c_addr, uint8_t reg_addr, uint8_t *data, uint16_t *len) {
    /* Update bTransferRequest to send buffer write request for Slave */
    i2c_addr = MAG3110_WRITE_ADDR;
    /*##-2- Master sends write request for slave #############################*/
    do
    {
		HAL_I2C_Mem_Write_IT(&I2cHandle, (uint16_t)i2c_addr, reg_addr, 1, data, 1);
        /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if itӳ busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
        while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        {
        } 
        
        /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
    }
    while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);    
	return 0;
}
int32_t get_magnetometer_info(void) {
	uint16_t len = 1;
    /*
        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
        ZYXOW   ZOW     YOW     XOW     ZYXDR   ZDR     YDR     XDR
    */
    i2c_read_mem(MAG3110_READ_ADDR, DR_STATUS, &dr_status, &len);
    if(ZYXDR_MASK & dr_status) {
//    	i2c_read_mem(MAG3110_READ_ADDR, OUT_X_MSB_REG, &mag3110_mag.x.msb, &len);
//        i2c_read_mem(MAG3110_READ_ADDR, OUT_X_LSB_REG, &mag3110_mag.x.lsb, &len);
//        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_MSB_REG, &mag3110_mag.y.msb, &len);
//        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_LSB_REG, &mag3110_mag.y.lsb, &len);
//        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_MSB_REG, &mag3110_mag.z.msb, &len);
//        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_LSB_REG, &mag3110_mag.z.lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_X_LSB_REG, &out_x_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_X_MSB_REG, &out_x_msb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_LSB_REG, &out_y_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_MSB_REG, &out_y_msb, &len);;
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_LSB_REG, &out_z_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_MSB_REG, &out_z_msb, &len);
        out_x = out_x_msb<<8;
        out_x += out_x_lsb;
        out_y = out_y_msb<<8;
        out_y += out_y_lsb;
        out_z = out_z_msb<<8;
        out_z += out_z_lsb;
        /* get off value*/
        i2c_read_mem(MAG3110_READ_ADDR, OFF_X_LSB, &off_x_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OFF_X_MSB, &off_x_msb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OFF_Y_LSB, &off_y_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OFF_Y_MSB, &off_y_msb, &len);;
        i2c_read_mem(MAG3110_READ_ADDR, OFF_Z_LSB, &off_z_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OFF_Z_MSB, &off_z_msb, &len);
    }
    else {
        HAL_Delay(10);
    }
	return 0;
}
int32_t get_temperature(void) {
    uint16_t len =1;
    i2c_read_mem(MAG3110_READ_ADDR, DIE_TEMP, (uint8_t*)&die_temp, &len);
    return die_temp;
}


int32_t get_sysmode(void) {
    uint16_t len =1;
    i2c_read_mem(MAG3110_READ_ADDR, SYSMOD, (uint8_t*)&sysmode, &len);
    return sysmode;
}
int8_t get_mag3110_id(void) {
    uint16_t len = 1;
    i2c_read_mem(MAG3110_READ_ADDR, WHO_AM_I, (uint8_t*)&mag3110_id, &len);
    return mag3110_id;
}
/* mag3110  x y standardization*/
uint8_t first_config = 0;
int32_t mag3110_std(void) {
    uint16_t len = 1;
    /*
        Bit7    Bit6    Bit5    Bit4    Bit3    Bit2    Bit1    Bit0
        ZYXOW   ZOW     YOW     XOW     ZYXDR   ZDR     YDR     XDR
    */
    i2c_read_mem(MAG3110_READ_ADDR, DR_STATUS, &dr_status, &len);
    if(ZYXDR_MASK & dr_status) {
        i2c_read_mem(MAG3110_READ_ADDR, OUT_X_LSB_REG, &out_x_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_X_MSB_REG, &out_x_msb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_LSB_REG, &out_y_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Y_MSB_REG, &out_y_msb, &len);;
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_LSB_REG, &out_z_lsb, &len);
        i2c_read_mem(MAG3110_READ_ADDR, OUT_Z_MSB_REG, &out_z_msb, &len);
        out_x = out_x_msb<<8;
        out_x += out_x_lsb;
        out_y = out_y_msb<<8;
        out_y += out_y_lsb;
        out_z = out_z_msb<<8;
        out_z += out_z_lsb;
        /* get off value*/
        if(0 == first_config) {
            out_xmin = out_x;
            out_ymin = out_y;
            out_zmin = out_z;
            out_xmax = out_x;
            out_ymax = out_y;
            out_zmax = out_z;
            first_config = 1;
        }
        if(out_x < out_xmin) {
            out_xmin = out_x;
        }
        else if(out_x > out_xmax) {
            out_xmax = out_x;
        }
        if(out_y < out_ymin) {
            out_ymin = out_y;
        }
        else if(out_y > out_ymax) {
            out_ymax = out_y;
        }
        if(out_z < out_zmin) {
            out_zmin = out_z;
        }
        else if(out_z > out_zmax) {
            out_zmax = out_z;
        } 
        /* 其他情况不需要处理 */
        off_x = (out_xmin + out_xmax) > 2;
        off_y = (out_ymin + out_ymax) > 2;
        off_z = (out_zmin + out_zmax) > 2;
        off_x_msb = off_x >> 8;
        off_x_lsb = off_x & 0xff;
        off_y_msb = off_y >> 8;
        off_y_lsb = off_y & 0xff;
        off_z_msb = off_z >> 8;
        off_z_lsb = off_z & 0xff;
    }
    return 0;
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