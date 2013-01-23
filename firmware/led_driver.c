#include "led_driver.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "i2cmaster.h"
#include "hw_config.h"


void led_init(void) {
  uart_puts_P("initializing LED driver\r\n");
  i2c_init();
}

void led_all_full(void) {
  // start communication with PCA9535
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
  } else {
	i2c_write(0x12);
	i2c_write(0x00);
	i2c_write(0xFF);
	i2c_write(0x00);
	i2c_write(0xFF);
	i2c_write(0x55);
	i2c_write(0xFA);
	i2c_write(0xFF);
	i2c_write(0xFF);
	i2c_stop();
  }
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

void led_window_open(void) {
  uart_puts_P("LED window open.\r\n");
}

void led_disconnected(void) {
  uart_puts_P("LED disconnected.\r\n");
}

void led_error(void) {
  uart_puts_P("LED error.\r\n");
}
