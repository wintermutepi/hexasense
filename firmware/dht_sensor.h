#ifndef _DHT22_H_
#define _DHT22_H_

/***
 * This is a slightly modified version of funkytransistor's 
 * public domain code, available from
 * http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=974797
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DHT22_DATA_BIT_COUNT 40
#define DHT22_PIN    PB1
#define DHT22_PORT   PORTB
#define DHT22_DDR    DDRB
#define DHT22_PINC   PINB


typedef enum
{
  DHT_ERROR_NONE = 0,
  DHT_BUS_HUNG,
  DHT_ERROR_NOT_PRESENT,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_SYNC_TIMEOUT,
  DHT_ERROR_DATA_TIMEOUT,
  DHT_ERROR_CHECKSUM,
} DHT22_ERROR_t;


DHT22_ERROR_t read_dht22( 
    uint8_t *dht_humidity_now, 
    float *dht_temperature_now 
  );

#endif
