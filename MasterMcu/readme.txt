运行芯片:
TI公司MSP430FR5989，该控制器是16位RISC架构的处理器。其主要特征如下：
	128Kbytes ROM(FRAM)；
	2Kbytes RAM(SRAM)；
	12bit ADC模块以及DAC模块；
	4个SPI口；
	2个I2C口；
	2个USART口；
	48个GPIO
注:FRAM也可以作为内存使用
运行时使用内部晶振, 系统时钟为MCK为8MHz
外设SMCK为1MHz
低功耗时使用辅助时钟ACK为10K

显示屏:
冀雅电子有限公司的JYG-1283212T液晶屏
分辨率:128*32


系统参数保存在Info区和FRAM
系统地址: 1800-19FF	   0.5k
日志: 4400-C7FF	33K  		

RAM: 01C00-023FF   2K
外加FRAM作为RAM: C800-E7FF  8K

CODE and Const: 
E800-FF7F	5.875K
FF80-FFFF  0.125K(128Byte this can not change)
10000-23FFF 80K