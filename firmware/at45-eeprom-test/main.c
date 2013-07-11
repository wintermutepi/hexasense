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
#include "hyt271.h"
#include "i2cmaster.h"
#include "dew_point.h"
#include "hw_config.h"
#include "adc.h"
#include "adc_temp_conversion.h"

PROGMEM const
#define unsigned
#define char prog_uint8_t
#include "cat_2_7.xbm"
#include "md_sensor1_2_7.xbm"
#undef char
#undef unsigned

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/* 9600 baud */
#define UART_BAUD_RATE      9600      

/**
 * this firmware has two modi: 
 * (1) button-triggered operation: meant for post-production testing
 * (2) stress-testing: for long-time hardware testing.
 * Define STRESS_TEST to select (2), (1) is default.
 */
//#define STRESS_TEST 1

void init(void) {
  /*
   *  Initialize UART library, pass baudrate and AVR cpu clock
   *  with the macro 
   *  UART_BAUD_SELECT() (normal speed mode )
   *  or 
   *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
   */
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU)); 
  timer_init();
  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
  uart_puts_P("\n\rHexaSense UART init complete.\n\r");
  spi_init();
  epd27_init();
  i2c_init();
  adc_init();
}

void read_digital_sensors(void) {
  char buffer[7];
#ifndef STRESS_TEST
  uart_puts_P("HYT271 sensor:\r\n");
#endif
  double hyt271_hum = 0.0;
  double hyt271_temp = 0.0;
  double dp = 0.0;
  HYT271_ERROR_t error_code =  hyt271_get_measurements(&hyt271_hum, &hyt271_temp);
  switch(error_code) {
    case HYT271_ERROR_NONE:
#ifndef STRESS_TEST
      uart_puts_P("Temperature: ");
#endif
      dtostrf(hyt271_temp, 5, 2, buffer);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
#ifndef STRESS_TEST
      uart_puts_P("\r\nHumidity: ");
#else
      uart_puts_P(";");
#endif
      itoa(hyt271_hum, buffer, 10);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
#ifndef STRESS_TEST
      uart_puts_P("\r\n");
      uart_puts_P("Dewpoint based on HYT271 sensor: ");
#else
      uart_puts_P(";");
#endif
      dp = dew_point(hyt271_temp, hyt271_hum);
      itoa(dp, buffer, 10);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
#ifndef STRESS_TEST
      uart_puts_P("\r\n");
#else
      uart_puts_P(";");
#endif
      break;
    case HYT271_BUS_ERROR:
      uart_puts_P("Bus error while reading HYT271 sensor.\r\n");
      break;
    default:
      uart_puts_P("Unknown error reading HYT271 sensor.\r\n");
      break;
  }
}


void read_analog_sensors(void) {
  float temperature0 = 0.0; 
  float temperature1 = 0.0; 
  temperature0=temperature_adc(ANALOG_TEMPERATURE_0); // convert from adc value to temperaure 
  temperature1=temperature_adc(ANALOG_TEMPERATURE_1); // convert from adc value to temperaure 
#ifndef STRESS_TEST
  uart_puts_P("Temperature from ADC0: ");
#endif
  char temperature_string_buffer[10];
  dtostrf(temperature0, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
#ifndef STRESS_TEST
  uart_puts_P("\r\n");
#else
	  uart_puts_P(";");
#endif
#ifndef STRESS_TEST
  uart_puts_P("Temperature from ADC4: ");
#endif
  dtostrf(temperature1, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
#ifndef STRESS_TEST
  uart_puts_P("\r\n");
#else
  uart_puts_P(";");
#endif
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
    uart_puts_P("Waiting for cog ready signal. ");
    epd27_wait_cog_ready();
    epd27_begin(); // power up the EPD panel
    epd27_set_temperature(22); // adjust for current temperature

    switch(state) {
      default:
      case 0:         // clear the screen
        epd27_clear();
        ++state;
        break;

      case 1:       
        epd27_image_whitescreen(cat_2_7_bits);
        //epd27_clear();
        ++state;
        break;

      case 2:       
        epd27_image_transition(cat_2_7_bits, md_sensor1_2_7_bits);
        ++state;
        break;

      case 3:        
        epd27_image_transition(md_sensor1_2_7_bits, cat_2_7_bits);
        state=2;
        break;
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
    read_digital_sensors();
    read_analog_sensors();
    _delay_ms(5000);
  }
}
