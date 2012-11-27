#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "adc.h"


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
  adc_init();

  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
}

int main(void)
{
  char buffer[7];
  uint16_t num=0;

  init();

  /*
   * Transmit string from program memory to UART
   */
  uart_puts_P("HexaSense prototype\n\r");


  /* 
   * Use standard avr-libc functions to convert numbers into string
   * before transmitting via UART
   */     

  while(1) {
    num = adc_get_single_sample(0);
    itoa( num, buffer, 10);   // convert interger into string (decimal format)         
    uart_puts(buffer);        // and transmit string to UART
    uart_puts_P("\r\n");
    _delay_ms(1000);
  }

}
