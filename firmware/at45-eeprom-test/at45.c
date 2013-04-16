#include "at45.h"
#include <avr/io.h>

// Command words from datasheet, p. 28ff.
#define B1_TO_MM_PAGE_PROG_W_ERASE 0x83
#define MM_PAGE_TO_B1_XFER 0x53
#define BUFFER_1_READ 0xd4
#define BUFFER_1_WRITE 0x84
#define STATUS_REG_READ 0xd7


void at45_select(void) { 
  AT45_PORT &= ~(1 << AT45_CS); 
} 

void at45_release(void) { 
  AT45_PORT |= (1 << AT45_CS); 
} 

// at45db161d read/write 
uint8_t at45_rw(uint8_t data) { 
  SPDR = data; 
  while (!(SPSR & 0x80)); 
  return SPDR; 
} 

// initialisation of SPI towards the AT45 chip 
void at45_init(void) __attribute__ ((optimize(1))); 

void at45_init(void) { 
  // Configure output pins.
  AT45_PORT |= (1 << AT45_CS) | (1 << AT45_SCK); //| (1 << AT45_MOSI); 
  AT45_DDR |= (1 << AT45_CS) | (1 << AT45_MOSI) | (1 << AT45_SCK); 
  // Configure input pins.
  //AT45_PORT |= (1 << AT45_MISO);
  AT45_DDR &= ~(1 << AT45_MISO); 
  // freq SPI CLK/2 

 // SPCR = (1<<SPE)|(1<<MSTR); 
 // SPSR |= (1<<SPI2X); 

 // set SPI rate = CLK/64 
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); 
  SPSR &= ~(1<<SPI2X); 
} 

// what's our status? 
uint8_t at45_status(void) { 
  uint8_t result; 
  at45_select(); 
  at45_rw(STATUS_REG_READ); 
  // the status is clocked out continuously as data written to SO. So:
  // do dummy writes.
  at45_rw(0xFF); 
  result = at45_rw(0xFF); 
  at45_release(); 

  return result; 
} 

uint8_t at45_is_ready(void) {
  uint8_t result = at45_status();
  return (result & (1 << 7));
}

// page to buffer 
uint8_t at45_read_page_to_buf_1(uint32_t addr_page) { 
  // 4096 - number of pages per AT45DB161D 
  //addr_page += 256; 
  if(addr_page < 4096) 
  { 
	at45_select(); 
	at45_rw(MM_PAGE_TO_B1_XFER); 
	at45_rw((char)(addr_page >> 6)); 
	at45_rw((char)(addr_page << 2)); 
	at45_rw(0x00); 
	at45_release(); 
	return 0; 
  } 
  return 1; 
} 

// copy from the buffer over to the chip 
uint8_t at45_write_from_buf_1(uint32_t addr_page) __attribute__ ((optimize(1))); 

uint8_t at45_write_from_buf_1(uint32_t addr_page) { 
  // 4096 - number of pages AT45DB161D 
  if(addr_page < 4096) 
  { 
	at45_select(); 
	at45_rw(B1_TO_MM_PAGE_PROG_W_ERASE); // write data from buffer1 to page 
	at45_rw((uint8_t)(addr_page>>6)); 
	at45_rw((uint8_t)(addr_page<<2)); 
	at45_rw(0x00); 
	at45_release(); 
	return 0; 
  } 
  return 1; 
} 

// copying from BUFFER1 to RAM 
void at45_read_from_buf_1(uint8_t * dst, uint32_t count, uint16_t addr) { 
  uint32_t i = 0; 
  at45_select(); 
  at45_rw(BUFFER_1_READ);                  
  at45_rw(0x00); 
  at45_rw((uint8_t)(addr>>8)); 
  at45_rw((uint8_t)addr); 
  at45_rw(0x00); 
  while(count--) dst[i++] = at45_rw(0xFF); 
  at45_release(); 
} 

// transferring the data over to BUFFER 1 
void at45_write_to_buf_1(const uint8_t *src, uint32_t count, uint16_t addr) __attribute__ ((optimize(1))); 

void at45_write_to_buf_1(const uint8_t *src, uint32_t count, uint16_t addr) { 
  uint32_t i = 0; 
  at45_select(); 
  at45_rw(BUFFER_1_WRITE);                  
  at45_rw(0x00); 
  at45_rw((uint8_t)(addr>>8)); 
  at45_rw((uint8_t)addr); 
  //at45_rw(0x00); 
  while(count--) at45_rw(src[i++]); 
  at45_release();    

} 

