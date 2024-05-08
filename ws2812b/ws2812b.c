#include <stdint.h>
#include "ws2812b.h"

static SPI_Interface ws2812b_spi;

static uint8_t green[SPI_BYTES_PER_COLOR];
static uint8_t red[SPI_BYTES_PER_COLOR];
static uint8_t blue[SPI_BYTES_PER_COLOR];

static void ws2812b_spi_wr(uint8_t data);
static uint8_t ws2812b_spi_wr_ready(void);
static void ws2812b_color_byte_preparation(uint8_t *color, uint8_t weight);


/** @fn void ws2812b_spi_init(void *spi_send_handler, void *spi_send_ready_hanler)
*   @brief		Stores pointers on HAL dependend SPI functions
*	@param[in]	spi_send_handler		SPI send function handler
*   @param[in]	spi_send_ready_hanler   SPI send ready indicator
*
* The function initializes pointers to key SPI functions implemented in HAL
* of the corresponding processor. If the function pototype does not match, it is necessary to create  
* something like an adapter and adapt the SPI function calls!
*	
*	For example, in the case of the STM8S SPL, the initialization is as follows:
*		ws2812b_spi_init((void*)SPI_SendData, (void*)SPI_GetFS);
*	where
*		uint8_t SPI_GetFS(void)
*		{
*			return (uint8_t)SPI_GetFlagStatus(SPI_FLAG_TXE);
*		}
*
*/
void ws2812b_spi_init(void *spi_send_handler, void *spi_send_ready_hanler)
{
    ws2812b_spi.write = spi_send_handler;
    ws2812b_spi.is_wr_rdy = spi_send_ready_hanler;
}

/** @fn void ws2812b_send_uniform_GRB(uint8_t green_weight, uint8_t red_weight, uint8_t blue_weight)
*   @brief		Turns ON all LEDs into the same GRB color (uniformly).
*	@param[in]	green_weight
*	@param[in]	red_weight
*	@param[in]	blue_weight
*
*/
void ws2812b_send_uniform_GRB(uint8_t green_weight, uint8_t red_weight, uint8_t blue_weight)
{	
	int8_t i,j;

	ws2812b_color_byte_preparation(green, green_weight);
	ws2812b_color_byte_preparation(red, red_weight);
	ws2812b_color_byte_preparation(blue, blue_weight);

	for(j=0;j<NUMBER_OF_LEDS;j++)
	{
		for(i=SPI_BYTES_PER_COLOR-1;i>=0;i--)
		{
            ws2812b_spi_wr(green[i]);
            while(!ws2812b_spi_wr_ready()){}
		}
		for(i=SPI_BYTES_PER_COLOR-1;i>=0;i--)
		{
            ws2812b_spi_wr(red[i]);
            while(!ws2812b_spi_wr_ready()){}
		}
		for(i=SPI_BYTES_PER_COLOR-1;i>=0;i--)
		{
            ws2812b_spi_wr(blue[i]);
            while(!ws2812b_spi_wr_ready()){}
		}
	}
}

/** @fn void ws2812b_color_byte_preparation(uint8_t *color, uint8_t weight)
*   @brief              Converts color weight to NRZ code buffer for SPI
*	@param[in]	weight	color weight 0-255
*   @param[out] color   fout-byte color buffer of NRZ code 
*
*		each bit from weight needs to be mapped to 
*		four-bit NRZ code in the color buffer,
*		so 8 bits color weight x 4 bits reprezentation = 32 bits
*		which leads to 4 byte SPI buffer!
*		weight bits	| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
*		color bytes |   3   |   2   |   1   |   0   |
*		color bits	|7-4|3-0|7-4|3-0|7-4|3-0|7-4|3-0|
*/
static void ws2812b_color_byte_preparation(uint8_t *color, uint8_t weight)
{
	uint8_t weight_bit;
	uint8_t color_byte;
    uint8_t mask;
	
	for(weight_bit=0;weight_bit<7;weight_bit=weight_bit+2)
	{
        // just small indexing LUT
		if(weight_bit == 0)
			color_byte = 0;
		else if(weight_bit == 2)
			color_byte = 1;
		else if(weight_bit == 4)
			color_byte = 2;
		else if(weight_bit == 6)
			color_byte = 3;
		else
			color_byte = 0;

		// color bits 0-3
		mask = Test_Bit(weight,weight_bit) > 0 ? H : L;
		color[color_byte] = mask;
		// color bits 4-7
		mask = Test_Bit(weight,weight_bit+1) > 0 ? H<<4 : L<<4;
		color[color_byte] = color[color_byte] | mask;
	}
}

static void ws2812b_spi_wr(uint8_t data)
{
    ws2812b_spi.write(data);
}

static uint8_t ws2812b_spi_wr_ready(void)
{
    return ws2812b_spi.is_wr_rdy();
}