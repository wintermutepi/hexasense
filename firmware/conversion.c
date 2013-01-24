#ifndef CONVERSION_H
#define CONVERSION_H 1
#include <stdlib.h>
#include "conversion.h"
#include "adc.h"
#include <avr/pgmspace.h>

float temperature_adc(void) {
  uint16_t adc = adc_get_single_sample(0);

    return ( TEMP_MIN+(TEMP_MAX-TEMP_MIN)/(ADC_MAX-ADC_MIN) * adc );
}
#endif /* CONVERSION_H */
