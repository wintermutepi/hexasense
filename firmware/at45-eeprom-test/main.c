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
#include "button.h"

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
#define UART_BAUD_RATE 9600      

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
  spi_init();
  epd27_init();
  i2c_init();
  adc_init();
  button_init();
  /*
   * now enable interrupt, since UART library is interrupt controlled
   */
  sei();
}

void read_digital_sensors(void) {
  char buffer[7];
  uart_puts_P("HYT271 sensor\r\n");
  double hyt271_hum = 0.0;
  double hyt271_temp = 0.0;
  double dp = 0.0;
  HYT271_ERROR_t error_code =  hyt271_get_measurements(&hyt271_hum, &hyt271_temp);
  switch(error_code) {
    case HYT271_ERROR_NONE:
      uart_puts_P("Temperature: ");
      dtostrf(hyt271_temp, 5, 2, buffer);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
      uart_puts_P("\r\nHumidity: ");
      itoa(hyt271_hum, buffer, 10);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
      uart_puts_P("\r\n");
      uart_puts_P("Dewpoint based on HYT271 sensor: ");
      dp = dew_point(hyt271_temp, hyt271_hum);
      itoa(dp, buffer, 10);   // convert interger into string (decimal format)         
      uart_puts(buffer);        // and transmit string to UART
      uart_puts_P("\r\n");
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
  uart_puts_P("Temperature from ADC0: ");
  char temperature_string_buffer[10];
  dtostrf(temperature0, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
  uart_puts_P("\r\n");
  uart_puts_P("Temperature from ADC4: ");
  dtostrf(temperature1, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
  uart_puts_P("\r\n");
}

int main(void)
{
  init();

  uart_puts_P("\n\r HEXASENSE BRINGUP FIRMWARE\n\r");
  uart_puts_P("\n\r all inits complete.\n\r");

  // test the buttons
  uart_puts_P("testing the buttons:\n\r");
  uart_puts_P("press button 0... ");
  while(1) {
    if (is_button0_pressed()) {
      uart_puts_P("BTN0 pressed.\r\n");
      break;
    }
  }
  uart_puts_P("press button 1... ");
  while(1) {
    if (is_button1_pressed()) {
      uart_puts_P("BTN1 pressed.\r\n");
      break;
    }
  }
  uart_puts_P("press button 2... ");
  while(1) {
    if (is_button2_pressed()) {
      uart_puts_P("BTN2 pressed.\r\n");
      break;
    }
  }

  // test the display
  uart_puts_P("\n\r testing the display:\n\r");
  epd27_wait_cog_ready();
  epd27_begin(); // power up the EPD panel
  epd27_set_temperature(22); // adjust for current temperature
  epd27_clear();
  uart_puts_P("If you see a white screen, press any button.\n\r");
  button_loop();
  epd27_image_whitescreen(cat_2_7_bits);
  epd27_end();   // power down the EPD panel
  uart_puts_P("If you see a cat, press any button.\n\r");
  button_loop();

  // test the LEDs
  uart_puts_P("\n\r testing the LEDs:\n\r");
  //TODO: turn on LEDs
  uart_puts_P("If there are three 5mm LEDs on, press any button.\n\r");
  button_loop();
  //TODO: turn off LEDs

  // test the digital sensors
  uart_puts_P("\n\r testing the digial sensor:\n\r");
  read_digital_sensors();
  uart_puts_P("Are the values for temperature, humidity and dew point plausible? ");
  uart_puts_P("Then press any button.\n\r");
  button_loop();

  // calibrate the analog sensors
  uart_puts_P("\n\r calibrate the analog sensors:\n\r");
  uart_puts_P("Connect the -10 deg C resistor and turn the OFFSET-resistor ");
  uart_puts_P("until the voltmeter indicats 0V (+-50mV is sufficient).\n\r");
  uart_puts_P("Then connect the 100 deg C resistor and turn the TEMP-resistor ");
  uart_puts_P("until the voltmeter indicats 1V (+-50mV is sufficient).\n\r");
  uart_puts_P("Repeat these steps for the 2nd channel.\n\r");
  button_loop();

  // test the analog sensors
  uart_puts_P("\n\r testing the analog sensors:\n\r");
  read_analog_sensors();
  uart_puts_P("Are the values for temperature plausible? ");
  uart_puts_P("Then press any button.\n\r");
  button_loop();

  // test the flash chip
  uart_puts_P("\n\r testing the flash chip:\n\r");
  //TODO: display something loaded from the flash chip
  uart_puts_P("If you see [***TBD***], press any button.\n\r");
  button_loop();

  // if all is right, say so
  uart_puts_P("all tests successful! \n\r");
  uart_puts_P("The device may now be flashed with the production firmware.\n\r");
}
