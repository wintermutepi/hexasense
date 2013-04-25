#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

uint32_t t2_soft;


ISR( TIMER2_OVF_vect )
{
  t2_soft += 256;
}


void timer_init( void )
{
  t2_soft = 0;
  TCCR2B = 1<<CS21;	// mode 0, prescaler 8
  TIMSK2 |= 1<<TOIE2;	// enable overflow interrupt
}


uint32_t millis( void )			  // read T2 as 32 bit timer
{
  uint32_t val;
  uint8_t tifr;

  cli();
  val = t2_soft + TCNT2;
  tifr = TIFR2;				  // read interrupt flags
  sei();
  if( (tifr & (1<<TOV2)) && !(val & 0x80) ) // overflow prior reading TCNT2 ?
    val += 256;				  // then add overflow

  return val/(F_CPU/(1000*8));
}


