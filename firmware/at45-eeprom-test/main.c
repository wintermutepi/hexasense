#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "at45.h"
#include "at45_test.h"
#include "epd27.h"
#include "spi.h"
#include "timer.h"

PROGMEM const
#define unsigned
#define char prog_uint8_t
#include "cat_2_7.xbm"
#undef char
#undef unsigned

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
  timer_init();
  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
  spi_init();
  epd27_init();
}

int main(void)
{
  init();
  uint8_t state = 0;
  uint32_t timerticks;

  uart_puts_P("\n\rHexaSense prototype startup completed.\n\r");
  // Deletes the whole flash.
  //at45_test_init();

  // For debugging. Do not use in production.
  //// LED1: On PC7, active low.
  //DDRC |= (1 << PC7);
  //PORTC &= ~(1 << PC7);
  //bool enabled = true;
  while(true) {
    //at45_test_loop();
    //if (enabled) {
    //  EPD_PORT_DISCHARGE &= ~(1 << EPD_PIN_DISCHARGE);
    //  PWM_stop();
    //  enabled = false;
    //} else {
    //  EPD_PORT_DISCHARGE |= (1 << EPD_PIN_DISCHARGE);
    //  PWM_start();
    //  enabled = true;
    //}
    //uart_puts_P("Waiting for cog ready signal. ");
    //epd27_wait_cog_ready();
    epd27_begin(); // power up the EPD panel
    epd27_set_temperature(22); // adjust for current temperature

    switch(state) {
      default:
      case 0:         // clear the screen
        epd27_clear();
        ++state;
        break;

      case 1:         // clear -> text
        epd27_image_whitescreen(cat_2_7_bits);
        //epd27_clear();
        state=0;
        break;

        //  case 2:         // text -> picture
        //    EPD.image(TEXT_BITS, PICTURE_BITS);
        //    ++state;
        //    break;

        //  case 3:        // picture -> text
        //    EPD.image(PICTURE_BITS, TEXT_BITS);
        //    state = 2;  // backe to picture nex time
        //    break;
    }
    epd27_end();   // power down the EPD panel

    uart_puts_P("Timestamp: ");
    timerticks = millis();
    char buffer[50];
    ultoa(timerticks, buffer, 10);
    uart_puts(buffer);
    uart_puts_P( "state: ");
    itoa(state, buffer, 10);
    uart_puts(buffer);
    uart_puts_P("  loop.\r\n");
    _delay_ms(5000);
  }
}
