#ifndef HW_CONFIG_H
#define HW_CONFIG_H 1

/**
 * Connection of the first button: PA1
 */
#define BTN0_DDR	DDRB
#define BTN0_PORT	PORTB
#define BTN0_PIN	PB1
#define BTN0_IN		PINB

/**
 * Connection of the second button: PA2
 */
#define BTN1_DDR	DDRA
#define BTN1_PORT	PORTA
#define BTN1_PIN	PA2
#define BTN1_IN		PINA

/**
 * I2C error codes.
 */
#define I2C_ERROR 1
#define I2C_SUCCESS 0

/**
 * PCA9532 I2C bus definitions
 */
#define PCA9532_ADDR  0xC0
#define PCA9532_FULL 0x55
#define PCA9532_OFF 0x00

/**
 * HYT271 I2C bus definitions
 */
#define HYT271_ADDR (0x28 << 1)
#define HYT271_CONV_DELAY 6
#define HYT271_BUFFER_LEN 4

/**
 * analog temperature sensors and respective adc channel
 */
#define ANALOG_TEMPERATURE_0 0
#define ANALOG_TEMPERATURE_1 4

#endif /* HW_CONFIG_H */

