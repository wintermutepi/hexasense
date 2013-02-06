#ifndef ADC_H
#define ADC_H 1

#include <avr/io.h>

#define SUPERSAMPLING_MAX_ITERATION 8

void adc_init(void);
uint16_t adc_get_single_sample(uint8_t channel);

#endif /* ADC_H */

