#include <stdlib.h>
#include "conversion.h"
#include "adc.h"
#include "uart.h"
#include <avr/pgmspace.h>

float temperature_adc(void) {
  // TODO: Implement supersampling.
  uint16_t adc = adc_get_single_sample(0);
  char buffer[7];

  uart_puts_P("ADC Value: ");
  itoa( adc, buffer, 10);   // convert interger into string (decimal format)         
  uart_puts(buffer);        // and transmit string to UART
  uart_puts_P("\r\n");
  return ( TEMP_MIN+(TEMP_MAX-TEMP_MIN)/(ADC_MAX-ADC_MIN) * (float)adc );
}