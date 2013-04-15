#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "adc.h"
#include "dht_sensor.h"
#include "dew_point.h"
#include "hyt271.h"
#include "button.h"
#include "led_driver.h"
#include "adc_temp_conversion.h"
#include "i2cmaster.h"

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
#define STRESS_TEST 1

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
  /**
   * run the initialization code of all components
   */
  adc_init();
  button_init();
  led_init();
  i2c_init();
}

void read_digital_sensors(void) {
  char buffer[7];
#ifndef STRESS_TEST
  uart_puts_P("HYT271 sensor:\r\n");
#endif
  double hyt271_hum = 0.0;
  double hyt271_temp = 0.0;
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
	  double dp=dew_point(hyt271_temp, hyt271_hum);
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

void read_analog_sensor(void) {
  float temperature1=temperature_adc(0); // convert from adc value to temperaure 
  float temperature2=temperature_adc(4); // convert from adc value to temperaure 
#ifndef STRESS_TEST
  uart_puts_P("Temperature from ADC0: ");
#endif
  char temperature_string_buffer[10];
  dtostrf(temperature1, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
#ifndef STRESS_TEST
  uart_puts_P("\r\n");
#else
	  uart_puts_P(";");
#endif
#ifndef STRESS_TEST
  uart_puts_P("Temperature from ADC4: ");
#endif
  dtostrf(temperature2, 5,2, temperature_string_buffer);   // convert interger into string (decimal format)         
  uart_puts(temperature_string_buffer);        // and transmit string to UART
#ifndef STRESS_TEST
  uart_puts_P("\r\n");
#else
	  uart_puts_P(";");
#endif
}

void button_loop(void) {
  enum led_states {
	LED_OFF = 0,
	LED_ALL = 1,
	LED_RED = 2,
	LED_GREEN = 3,
	LED_BLUE = 4
  };
  enum led_states statemachine;
  statemachine=LED_OFF;
  led_all_off();
  while(1) {
	if (is_button0_pressed()) {
	  uart_puts_P("BTN0 pressed.\r\n");
//<<<<<<< HEAD
//	  uart_puts_P("DHT22 sensor:\r\n");
//	  DHT22_ERROR_t errorCode = read_dht22( &temp1, &temp2 ); 
//	  switch(errorCode) { 
//		case DHT_ERROR_NONE: 
//		  uart_puts_P("Temperature: ");
//		  dtostrf(temp2, 5, 2, buffer);   // convert interger into string (decimal format)         
//		  uart_puts(buffer);        // and transmit string to UART
//		  uart_puts_P("\r\nHumidity: ");
//		  itoa(temp1, buffer, 10);   // convert interger into string (decimal format)         
//		  uart_puts(buffer);        // and transmit string to UART
//		  uart_puts_P("\r\n");
//		  break; 
//		case DHT_BUS_HUNG:
//		  uart_puts_P("Bus hung error reading DHT22 sensor.\r\n");
//		  break;
//		case  DHT_ERROR_NOT_PRESENT:
//		  uart_puts_P("Error reading DHT22 sensor: sensor not present.\r\n");
//		  break;
//		case  DHT_ERROR_ACK_TOO_LONG:
//		  uart_puts_P("Error reading DHT22 sensor: ACK takes too long.\r\n");
//		  break;
//		case  DHT_ERROR_SYNC_TIMEOUT:
//		  uart_puts_P("Error reading DHT22 sensor: Sync timeout.\r\n");
//		  break;
//		case  DHT_ERROR_DATA_TIMEOUT:
//		  uart_puts_P("Error reading DHT22 sensor: Data timeout.\r\n");
//		  break;
//		case  DHT_ERROR_CHECKSUM:
//		  uart_puts_P("Error reading DHT22 sensor: Checksum invalid.\r\n");
//		  break;
//		default:
//		  uart_puts_P("Unknown error reading DHT22 sensor.\r\n");
//		  break;
//	  }
//	  uart_puts_P("HYT271 sensor:\r\n");
//	  double hyt271_hum = 0.0;
//	  double hyt271_temp = 0.0;
//	  HYT271_ERROR_t error_code =  hyt271_get_measurements(&hyt271_hum, &hyt271_temp);
//	  switch(error_code) {
//		case HYT271_ERROR_NONE:
//		  uart_puts_P("Temperature: ");
//		  dtostrf(hyt271_temp, 5, 2, buffer);   // convert interger into string (decimal format)         
//		  uart_puts(buffer);        // and transmit string to UART
//		  uart_puts_P("\r\nHumidity: ");
//		  itoa(hyt271_hum, buffer, 10);   // convert interger into string (decimal format)         
//		  uart_puts(buffer);        // and transmit string to UART
//		  uart_puts_P("\r\n");
//		  uart_puts_P("Dewpoint based on HYT271 sensor: ");
//		  double dp=dew_point(hyt271_temp, hyt271_hum);
//		  itoa(dp, buffer, 10);   // convert interger into string (decimal format)         
//		  uart_puts(buffer);        // and transmit string to UART
//		  uart_puts_P("\r\n");
//		  break;
//		case HYT271_BUS_ERROR:
//		  uart_puts_P("Bus error while reading HYT271 sensor.\r\n");
//		  break;
//		default:
//		  uart_puts_P("Unknown error reading HYT271 sensor.\r\n");
//		  break;
//	  }
//
//=======
	  read_digital_sensors();
>>>>>>> UNTESTED code: two modi for post-production evaluation and stresstest via #define
	}
	if (is_button1_pressed()) {
	  uart_puts_P("BTN1 pressed.\r\n");
	  read_analog_sensor();
	}
	if (is_button2_pressed()) {
	  uart_puts_P("BTN2 pressed.\r\n");
	  switch (statemachine) {
		case LED_OFF: led_all_full(); statemachine = LED_ALL; break;
		case LED_ALL: led_red_full(); statemachine = LED_RED; break;
		case LED_RED: led_green_full(); statemachine = LED_GREEN; break;
		case LED_GREEN: led_blue_full(); statemachine = LED_BLUE; break;
		case LED_BLUE: led_all_off(); statemachine = LED_OFF; break;
	  }
	}
  }
}

void stress_test(void) {
  led_all_full(); 
  while(1) {
	read_digital_sensors();
	read_analog_sensor();
	uart_puts_P("\r\n");
	_delay_ms(60000);
  }
}

int main(void)
{
  init();

  // For debugging. Do not use in production.
  //// LED1: On PC7, active low.
  //DDRC |= (1 << PC7);
  //PORTC &= ~(1 << PC7);

#ifndef STRESS_TEST
  button_loop();
#else
  stress_test();
#endif

}
