#include "led_driver.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "i2cmaster.h"
#include "hw_config.h"


void led_init(void) {
  uart_puts_P("initializing LED driver\r\n");
  i2c_init();
  // start communication with PCA9535
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
  } else {
	// PSC0 subaddress + auto-increment
	i2c_write(0x12);
	// prescaler 0: maximum frequency
	i2c_write(0x00);
	// pwm0 duty cycle: 256/256
	i2c_write(0xFF);
	// prescaler 1: maximum frequency
	i2c_write(0x00);
	// pwm1 duty cycle: 256/256
	i2c_write(0xFF);
  }
  i2c_stop();
  // turn all LEDs off.
  led_all_off();
}

void led_all_full(void) {
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
  } else {
	//LS0 subaddress + auto-increment
	i2c_write(0x16);
	// LED 0-3 on
	i2c_write(0x55);
	// LED 4-7 on
	i2c_write(0x55);
	// LED 8-11 on
	i2c_write(0x55);
	// LED 12-15 on
	i2c_write(0x55);
  }
  i2c_stop();
}

void led_all_off(void) {
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
  } else {
	//LS0 subaddress + auto-increment
	i2c_write(0x16);
	// LED 0-3 off
	i2c_write(0x00);
	// LED 4-7 off
	i2c_write(0x00);
	// LED 8-11 off
	i2c_write(0x00);
	// LED 12-15 off
	i2c_write(0x00);
  }
  i2c_stop();
}


void led_good(void) {
  uart_puts_P("LED good.\r\n");
}

void led_intermediate(void) {
  uart_puts_P("LED intermediate.\r\n");
}

void led_bad(void) {
  uart_puts_P("LED bad.\r\n");
}

void led_red_full(void) {
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
  } else {
	//LS0 subaddress + auto-increment
	i2c_write(0x16);
	// LED 0-2 on
	i2c_write(0x15);
  }
  i2c_stop();
}

void led_window_open(void) {
  uart_puts_P("LED window open.\r\n");
}

void led_disconnected(void) {
  uart_puts_P("LED disconnected.\r\n");
}

void led_error(void) {
  uart_puts_P("LED error.\r\n");
}
