#include "hyt271.h"
#include <hw_config.h>
#include <i2cmaster.h>
#include <uart.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

HYT271_ERROR_t hyt271_get_measurements(double* humidity, double* temperature) {
  uint8_t buffer[HYT271_BUFFER_LEN];

  // 1. Request measurement.
  // start communication with HYT271
  if (i2c_start(HYT271_ADDR + I2C_WRITE)) {
    uart_puts_P("I2C: Failed to write-access hyt271 device.\r\n");
    i2c_stop();
    return HYT271_BUS_ERROR;
  } else {
    i2c_write(0x00);
  }
  i2c_stop();

  // 2. Wayt for some time 
  _delay_ms(HYT271_CONV_DELAY);

  // 3. Gather measurement.
  if (i2c_start(HYT271_ADDR+I2C_READ) ) {
    uart_puts_P("I2C: Failed to read-access hyt271 device.\r\n");
    i2c_stop();
    return HYT271_BUS_ERROR;
  } else {
    for(uint8_t i=0; i<HYT271_BUFFER_LEN-1; i++) {
      buffer[i]=i2c_readAck();
    }
    buffer[HYT271_BUFFER_LEN-1] = i2c_readNak();
  }
  i2c_stop();

  // 4. convert data. From the reference implementation of hygrosens:
  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++
   *	GENERAL SENSOR INFORMATION
   *-------------------------------
   *	
   *	sensor abilities:
   *		/ humidity
   *			> 0x0000	- 	0x3FFF	hex
   *			> 0			- 	16383	dec
   *			> 0			- 	100		%	relative humidity
   *
   *		/ temperature
   *			> 0x0000	-	0x3FFF	hex
   *			> 0			-	16383	dec
   *			> -40		-	+125	°C
   *			> -40		-	+257	°F
   *			> +233.15	-	+398.15	K
   *
   *	raw value format:
   *
   *		|	1.byte	|	2.byte	|	3.byte	|	4.byte	|
   *		|-----------------------|-----------------------|
   *		|		Humidity		|		Temperature		|
   *		|-----------------------|-----------------------|
   *		|	2 bit	|	14 bit	|	14 bit	|	2 bit 	|
   *		|-----------|-----------|-----------|-----------|
   *		|	state	|	data	|	data	|	state	|
   *
   *
   *++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  uint16_t rawhum = ((buffer[0]<<8 | buffer[1]) & 0x3FFF);
  *humidity = (100.0/16384.0*rawhum);
  uint16_t rawtemp = buffer[2]<<6 | (buffer[3]&0x3F);
  *temperature = (165.0/16384.0*rawtemp)-40.0;

  return HYT271_ERROR_NONE;
}
