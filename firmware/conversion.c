#include "conversion.h"

int8_t temperature_conversion(uint16_t adc) {
  uart_puts_P("converting adc value to temperature\r\n");
  return ( TEMP_MIN+(TEMP_MAX-TEMP-MIN)/(ADC_MAX-ADC_MIN)*adc );
}
