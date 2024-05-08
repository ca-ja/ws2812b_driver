/*
 * ws2812b.h
 *
 *  Created on: 2024
 *      Author: caganjan@gmail.com
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#define  Test_Bit(var, bit)   (((var) & (1 <<(bit)))>0)

#define H	0b00000111
#define L	0b00000001

#define NUMBER_OF_LEDS          12
#define SPI_BYTES_PER_COLOR	    4
#define NUMBER_OF_COLORS        3

typedef struct {
    void (*write)(uint8_t data);
    uint8_t (*is_wr_rdy)(void);
} SPI_Interface;

void ws2812b_spi_init(void *spi_send_handler, void *spi_send_ready_hanler);
void ws2812b_send_uniform_GRB(uint8_t green_weight, uint8_t red_weight, uint8_t blue_weight);

#endif /* WS2812B_H_ */