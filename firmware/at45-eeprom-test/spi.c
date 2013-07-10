#include "spi.h"
#include <spi_conf.h>
#include <avr/io.h>

//void spi_init(void) __attribute__ ((optimize(1))); 
void spi_init(void) {
  // Configure output pins. Chip select lines must be configured
  // during peripheral configuration.
  SPI_PORT |= (1 << SPI_SCK); 
  SPI_DDR  |= (1 << SPI_MOSI) | (1 << SPI_SCK); 
  SPI_DDR  &= ~(1 << SPI_MISO); 

  // GLOBAL SPI INIT
  // set SPI rate = CLK/64 
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); 
  SPSR &= ~(1<<SPI2X); 
  // freq SPI CLK/2 
  // SPCR = (1<<SPE)|(1<<MSTR); 
  // SPSR |= (1<<SPI2X); 

}

uint8_t spi_rw(uint8_t data) {
  SPDR = data; 
  while (!(SPSR & 0x80)); 
  return SPDR; 
}
