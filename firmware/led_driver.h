#ifndef LED_DRIVER_H
#define LED_DRIVER_H 1
#include <stdint.h>

#define I2C_ERROR 1
#define I2C_SUCCESS 0
#define PCA9532_FULL 0x55
#define PCA9532_OFF 0x00

/**
 * initialize the led driver suite
 */
void led_init(void);

/**
 * low level communication via I2C.
 */
uint8_t set_pca9532(
	uint8_t psc0,
	uint8_t pwm0,
	uint8_t psc1,
	uint8_t pwm1,
	uint8_t ls0,
	uint8_t ls1,
	uint8_t ls2,
	uint8_t ls3);

/**
 * run all LEDs at full power!
 */
void led_all_full(void);

/***
 * turn all leds off.
 */
void led_all_off(void);

/**
 * called for displaying the air quality metric
 */
void led_good(void);
void led_intermediate(void);
void led_bad(void);


void led_red_full(void);
void led_green_full(void);
void led_blue_full(void);

/** 
 * show that the window is opened
 */
void led_window_open(void);

/**
 * when the device is not connected to any network
 * (you need to pair the device)
 */
void led_disconnected(void);

/**
 * when a fatal error occured
 */
void led_error(void);

#endif /* LED_DRIVER_H */

