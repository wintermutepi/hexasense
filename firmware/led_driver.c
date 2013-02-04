#include "led_driver.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "i2cmaster.h"
#include "hw_config.h"


void led_init(void) {
  uart_puts_P("initializing LED driver\r\n");
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_OFF, // LED0-3: off.
		PCA9532_OFF, // LED4-7: off.
		PCA9532_OFF, // LED8-11: off.
		PCA9532_OFF  // LED12-15: off.
	)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
  }
}

uint8_t set_pca9532(
	uint8_t psc0,
	uint8_t pwm0,
	uint8_t psc1,
	uint8_t pwm1,
	uint8_t ls0,
	uint8_t ls1,
	uint8_t ls2,
	uint8_t ls3)
{
   i2c_init();
  // start communication with PCA9535
  if (i2c_start(PCA9532_ADDR+I2C_WRITE)) {
	uart_puts_P("I2C: Failed to access device.\r\n");
	return I2C_ERROR;
  } else {
	// PSC0 subaddress + auto-increment
	i2c_write(0x12);
	// prescaler 0
	i2c_write(psc0);
	// pwm0 duty cycle
	i2c_write(pwm0);
	// prescaler 1
	i2c_write(psc1);
	// pwm1 duty cycle
	i2c_write(pwm1);
	// LED 0-3 
	i2c_write(ls0);
	// LED 4-7 
	i2c_write(ls1);
	// LED 8-11
	i2c_write(ls2);
	// LED 12-15 
	i2c_write(ls3);
  }
  i2c_stop();
  return I2C_SUCCESS;
}


void led_all_off(void) {
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_OFF, // LED0-3: off.
		PCA9532_OFF, // LED4-7: off.
		PCA9532_OFF, // LED8-11: off.
		PCA9532_OFF  // LED12-15: off.
		)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
  }
}

void led_all_full(void) {
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_FULL, // LED0-3: on.
		PCA9532_FULL, // LED4-7: on.
		PCA9532_FULL, // LED8-11: on.
		PCA9532_FULL  // LED12-15: on.
		)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
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

void led_red_full(void) {
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_FULL, // LED0-3: on.
		PCA9532_OFF, // LED4-7: off.
		PCA9532_OFF, // LED8-11: off.
		PCA9532_OFF  // LED12-15: off.
		)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
  }
}

void led_green_full(void) {
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_OFF,
		PCA9532_FULL,
		PCA9532_OFF,
		PCA9532_OFF
		)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
  }
}

void led_blue_full(void) {
  if (set_pca9532(
		0x00, // prescaler 0: maximum frequency
		0xFF, // pwm0: 256/256
		0x00, // prescaler 1: maximum frequency
		0xFF, // pwm1: 256/256
		PCA9532_OFF,
		PCA9532_OFF,
		PCA9532_FULL,
		PCA9532_OFF
		)) 
  {
	uart_puts_P("LED initialization failed.\r\n");
  }
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
