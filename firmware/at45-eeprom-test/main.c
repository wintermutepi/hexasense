#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "i2cmaster.h"

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

  uart_puts_P("HexaSense prototype startup\n\r");
  i2c_init();
}
int main(void)
{
  init();

  // For debugging. Do not use in production.
  //// LED1: On PC7, active low.
  //DDRC |= (1 << PC7);
  //PORTC &= ~(1 << PC7);

  while(true) {
    ;;
  }
}
