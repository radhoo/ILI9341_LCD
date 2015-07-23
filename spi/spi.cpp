#include "spi.h"

/**
 * set spi speed and settings
 */
void spi_init(void) {
	/*SPI_DDR |= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_MISO); 	// set MOSI and SCK as output
	SPI_PORT|= (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_MISO);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR = 0x1; // needed on mega128*/
	PORTB = 0b10110101;			/* Enable drivers */
	DDRB  = 0b11000111;
	SPCR = (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
}


/**
 * send spi data
 */
void spi_send(unsigned char data) {
	SPDR = data;//move data into spdr
	while(!(SPSR & (1<<SPIF)));//wait till the transmission is finished
}
