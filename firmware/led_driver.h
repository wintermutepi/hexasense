#ifndef LED_DRIVER_H
#define LED_DRIVER_H 1

/**
 * initialize the led driver suite
 */
void led_init(void);

/**
 * run all LEDs at full power!
 */
void led_all_full(void);

/**
 * called for displaying the air quality metric
 */
void led_good(void);
void led_intermediate(void);
void led_bad(void);

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

