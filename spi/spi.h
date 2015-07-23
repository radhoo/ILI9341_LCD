#pragma once

#include <avr/io.h>

/*#define MCU __AVR_ATmega128A__

#if MCU == __AVR_ATmega8__
	#define SPI_DDR DDRB
	#define SPI_MOSI PB3
	#define SPI_MISO PB4
	#define SPI_SCK PB5
#elif MCU == __AVR_ATmega168__
	#define SPI_DDR DDRB
	#define SPI_MOSI PB3
	#define SPI_SCK PB5
#elif MCU == __AVR_ATmega128A__*/
	#define SPI_DDR DDRB
#define SPI_PORT PORTB
	#define SPI_SCK PB1
	#define SPI_MOSI PB2
	#define SPI_MISO PB3
//#endif

/**
 * set spi speed and settings
 */
void spi_init(void);

/**
 * send spi data
 */
void spi_send(uint8_t data);
