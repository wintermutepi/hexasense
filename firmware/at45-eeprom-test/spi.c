#include "spi.h"
#include <at45.h>
#include <avr/io.h>

//void spi_init(void) __attribute__ ((optimize(1))); 
void spi_init(void) {
  // Configure output pins.
  AT45_PORT |= (1 << AT45_CS) | (1 << AT45_SCK); //| (1 << AT45_MOSI); 
  AT45_DDR |= (1 << AT45_CS) | (1 << AT45_MOSI) | (1 << AT45_SCK); 
  // Configure input pins.
  //AT45_PORT |= (1 << AT45_MISO);
  AT45_DDR &= ~(1 << AT45_MISO); 

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
