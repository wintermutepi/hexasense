#include "adc.h"
// Taken from 
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Analoge_Ein-_und_Ausgabe

void adc_init(void) {
  uint16_t result;
  // interne Referenzspannung als Referenz für den ADC wählen:
  ADMUX = (1<<REFS1) | (1<<REFS0);
  
  // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
  // schon auf 0, also single conversion
  ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
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
