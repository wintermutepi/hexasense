#include <stdlib.h>
#include <stdint.h>
#define TEMP_MIN -10.f
#define TEMP_MAX  100.f
#define ADC_MIN   0.f
#define ADC_MAX   960.f

/**
 * convert adc value to temperature
 */
float temperature_adc(uint8_t channel);
