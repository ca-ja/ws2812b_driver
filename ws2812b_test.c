#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "ws2812b.h"

#define SPI_OUTPUT_LEN  NUMBER_OF_LEDS*NUMBER_OF_COLORS*SPI_BYTES_PER_COLOR

static uint8_t spi_output[SPI_OUTPUT_LEN];
static int spi_output_idx;

// Target dependent stuffs (fakes, mocks,...) ////////////////////
typedef enum {RESET = 0, SET = !RESET} FlagStatus;

typedef enum {
  SPI_FLAG_TXE    = (uint8_t)0x02, /*!< Transmit buffer empty */
} SPI_Flag_TypeDef;

void SPI_SendData(uint8_t Data)
{
	printf("%X ", Data);

  if(spi_output_idx<SPI_OUTPUT_LEN)
    spi_output[spi_output_idx++] = Data;
}

FlagStatus SPI_GetFlagStatus(SPI_Flag_TypeDef SPI_FLAG)
{
	return 1;
}
// //////////////////////////////////////////////////////////////// 


int main(int argc, char* argv[]) {
  printf("WS2812B DRIVER TEST!\r\n");
  
  spi_output_idx = 0;
  memset(spi_output, 0, SPI_OUTPUT_LEN);

  int i, j;
  uint8_t green = 0;
  uint8_t red = 255;
  uint8_t blue = 170;

	ws2812b_spi_init((void*)SPI_SendData, (void*)SPI_GetFlagStatus);
	ws2812b_send_uniform_GRB(green, red, blue);
  
  // All LEDs has the same color as ws2812b_send_uniform_GRB does
  // (see WS2812B_color_byte_preparation in ws2812b.c documentation)
  for(j=0;j<NUMBER_OF_LEDS;j++)
  {
    // GREEN ASSERT: 0 -> 0000 0000 -> 0001 0001 0001 0001 0001 0001 0001 0001 -> 11 11 11 11
    for(i=0;i<4;i++)
      assert(spi_output[i+(j*12)] == 0x11);

    // RED ASSERT: 255 -> 1111 1111 -> 0111 0111 0111 0111 0111 0111 0111 0111 -> 77 77 77 77 
    for(i=0;i<4;i++)
      assert(spi_output[4+i+(j*12)] == 0x77);

    // BLUE ASSERT: 170 -> 1010 1010 -> 0111 0001 0111 0001 0111 0001 0111 0001 -> 71 71 71 71 
    for(i=0;i<4;i++)
      assert(spi_output[8+i+(j*12)] == 0x71);
  }

  return 0;
}