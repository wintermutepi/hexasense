#ifndef HW_CONFIG_H
#define HW_CONFIG_H 1

/**
 * Connection of the DHT22 sensor.
 */
#define DHT22_PIN    PD3
#define DHT22_PORT   PORTD
#define DHT22_DDR    DDRD
#define DHT22_PINC   PIND


/**
 * Connection of the first button: PA1
 */
#define BTN0_DDR	DDRA
#define BTN0_PORT	PORTA
#define BTN0_PIN	PA1
#define BTN0_IN		PINA

/**
 * Connection of the second button: PA2
 */
#define BTN1_DDR	DDRA
#define BTN1_PORT	PORTA
#define BTN1_PIN	PA2
#define BTN1_IN		PINA

/**
 * Connection of the third button: PA3
 */
#define BTN2_DDR	DDRA
#define BTN2_PORT	PORTA
#define BTN2_PIN	PA3
#define BTN2_IN		PINA






#endif /* HW_CONFIG_H */

