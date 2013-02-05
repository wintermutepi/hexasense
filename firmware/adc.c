#include "adc.h"
// Taken from 
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Analoge_Ein-_und_Ausgabe

void adc_init(void) {
  // interne Referenzspannung (1.1V) als Referenz für den ADC wählen:
  ADMUX = (1<<REFS1) | (0<<REFS0);
  
  // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
  // schon auf 0, also single conversion

  // ADC Prescaler Calculation: We run at 8 MHz, the ADC needs a frequency 
  // between 0.05 MHz and 0.2 MHz:
  // 0.05 <= 8/x <= 0.2
  // <=> 0.05*x <= 8 <= 0.2*x
  // Satisfied for x=128. Set Prescaler accordingly:
  ADCSRA =  (1<<ADPS1) | (1<<ADPS0);     
  ADCSRA |= (1<<ADEN);                  // ADC aktivieren
 
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
  adc_get_single_sample(0);
}

uint16_t adc_get_single_sample(uint8_t channel) {
  ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung starten
  while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
  }
  return ADCW;
}
