#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "adc.h"
#include "dht_sensor.h"
#include "button.h"
#include "led_driver.h"
#include "conversion.h"


/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 8000000UL
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

  /**
   * run the initialization code of all components
   */
  adc_init();
  button_init();
  led_init();

  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
}

int main(void)
{
  char buffer[7];
  uint16_t num=0;
  uint8_t temp1; 
  float temp2;
  float temperature=0;

  init();
  uart_puts_P("HexaSense prototype\n\r");

  // LED1: On PC7, active low.
  DDRC |= (1 << PC7);
  PORTC &= ~(1 << PC7);

  led_all_full();

  while(1) {
      DHT22_ERROR_t errorCode = read_dht22( &temp1, &temp2 ); 
      switch(errorCode) { 
        case DHT_ERROR_NONE: 
          dtostrf(temp2, 5, 2, buffer);   // convert interger into string (decimal format)         
          uart_puts(buffer);        // and transmit string to UART
          uart_puts_P(";");
          itoa(temp1, buffer, 10);   // convert interger into string (decimal format)         
          uart_puts(buffer);        // and transmit string to UART
          break; 
        default:
          break;
      }
      temperature=temperature_adc(); // convert from adc value to temperaure 
      //itoa( temperature, buffer, 10);   // convert interger into string (decimal format)         
      
      static char temperature_string_buffer[10];
      dtostrf(temperature, 5, 2, &temperature_string_buffer);
      uart_puts_P(";");
      uart_puts(temperature_string_buffer);        // and transmit string to UART
      uart_puts_P("\r\n");
    _delay_ms(60000); // wait one minute
  }
}
