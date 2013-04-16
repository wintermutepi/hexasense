#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "at45.h"

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      9600      

void init(void) {
  /*
   *  Initialize UART library, pass baudrate and AVR cpu clock
   *  with the macro 
   *  UART_BAUD_SELECT() (normal speed mode )
   *  or 
   *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
   */
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
  at45_init();
}
int main(void)
{
  init();
  uart_puts_P("\n\rHexaSense prototype startup completed.\n\r");

  // For debugging. Do not use in production.
  //// LED1: On PC7, active low.
  //DDRC |= (1 << PC7);
  //PORTC &= ~(1 << PC7);

  while(true) {
    char conversion_buffer[50];
    uart_puts_P(" Querying AT45 status: ");
    uint8_t at45_stat = at45_status();
    itoa(at45_stat, conversion_buffer, 10);
    uart_puts(conversion_buffer);
    if (at45_is_ready()) {
      uart_puts_P(", device ready.\r\n");
    } else {
      uart_puts_P(", device NOT ready.\r\n");
    }

    uint8_t buf1[4];
    uart_puts_P(" Deleting buffer 1: ");
    memset(buf1, 0, 4);
    at45_write_to_buf_1(buf1, 4, 0);
    uart_puts_P("complete.\r\n");

    uart_puts_P(" Reading buffer 1: ");
    memset(buf1, 0, 4);
    at45_read_from_buf_1(buf1, 4, 0);
    itoa(buf1[0], conversion_buffer, 10);
    uart_puts(conversion_buffer);
    uart_puts_P(" received.\r\n");

    uart_puts_P(" Writing buffer 1: ");
    memset(buf1, 0, 4);
    buf1[0]=23;
    buf1[1]=42;
    at45_write_to_buf_1(buf1, 4, 0);
    uart_puts_P("complete.\r\n");
   
    uart_puts_P(" Reading buffer 1 again: ");
    memset(buf1, 0, 4);
    at45_read_from_buf_1(buf1, 4, 0);
    itoa(buf1[0], conversion_buffer, 10);
    uart_puts(conversion_buffer);
    uart_puts_P(" received.\r\n");


    uart_puts_P("\r\n");
    _delay_ms(1000);
  }
}
