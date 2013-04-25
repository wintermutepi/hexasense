#ifndef TIMER_H
#define TIMER_H 1
#include <stdint.h>


/* define CPU frequency in Mhz here if not defined in Makefile */
//#ifndef F_CPU
//#define F_CPU 16000000UL
//#endif

/**
 * Peter Dannecker code, Public Domain
 * http://www.mikrocontroller.net/attachment/16302/TIMER32.C
 * http://www.mikrocontroller.net/topic/avr-timer-mit-32-bit#new
 */
void timer_init( void );
uint32_t millis( void );

#endif /* TIMER_H */

