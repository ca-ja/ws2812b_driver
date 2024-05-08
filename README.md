# WS2812B DRIVER
This module implements the lightweight WS2812B LED driver. The lightweight driver targets small 8-bit processors. The lightness lies mainly in static allocation, pure C, and above all simple implementation via SPI. Driving LEDs via SPI has the advantage of making do with a 4 MHz baudrate, which allows the appropriate NZR code to be sent at the required 800 kbps.

The driver itself is in the `ws2812b` directory, with a simple test in the parent directory. 

## DRIVER USAGE
The driver was physically tested with an STM8S103F3P processor with firmware compiled using STM8 Cosmic. The CPU and SPI clock settings were as follows:

```
uint32_t CLK_setup(void)
{
	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16 MHz
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1); // 16 MHz
	CLK_HSICmd(ENABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	return CLK_GetClockFreq();
}

void SPI_setup(void)
{
	SPI_DeInit();
	SPI_Init(SPI_FIRSTBIT_MSB,
					SPI_BAUDRATEPRESCALER_4,	// 16/4=4 MHz
					SPI_MODE_MASTER,
					SPI_CLOCKPOLARITY_LOW,
					SPI_CLOCKPHASE_1EDGE,
					SPI_DATADIRECTION_1LINE_TX,
					SPI_NSS_SOFT,
					0x00);
	SPI_Cmd(ENABLE);
}
```

The driver is separated from the HAL layer implementation! Therefore, before using it, you must first initialize pointers to the key SPI functions, which are the send data and send completion check functions (the driver is blocking, interrupting)! See ws2812b_spi_init documentation.