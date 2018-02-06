# STM32L073RZ configure
## stm32l073rz的pin分布如图所示
.	![stm32l073rz_pin_info](./pic/stm32l073rz_pin_info.png)
## ultrasonic GPIO configure
1.	VCC3.3
2.	GND
3.	PC5 -- trig
4.	PC6 -- echo
pin脚如图所示:
.	![ultrasonic_gpio_cong](./pic/ultrasonic_gpio_conf.png)

## infrared GPIO configure
1.	VCC 5.0V
2.	GND
3.	VO	-- PA.0,模拟信号输入
.	![GP2Y0A41SK0F_connect](./pic/GP2Y0A41SK0F_connect.png)
## mag3110 GPIO configure
1.	VCC 5.0V
2.	GND
3.	SCL -- PB8(CN10, pin 3 (Arduino D15))
4.	SDA -- PB9(CN10, pin 5 (Arduino D14))