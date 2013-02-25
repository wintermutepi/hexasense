#include "adc_temp_conversion.h"
#include "adc.h"
#include "uart.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

float temperature_adc(void) {
  uint16_t adc = adc_get_super_sample(0);
  return ( TEMP_MIN+(TEMP_MAX-TEMP_MIN)/(ADC_MAX-ADC_MIN) * (float)adc );
}
