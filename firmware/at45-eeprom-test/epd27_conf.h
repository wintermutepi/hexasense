#ifndef EPD27_CONF_H
#define EPD27_CONF_H 1

/**
 * Definition for the 2"7 display
 */

// output
#define EPD_PIN_EPD_CS PB0
#define EPD_PORT_EPD_CS PORTB
#define EPD_DDR_EPD_CS DDRB

// output
#define EPD_PIN_PANEL_ON PD2
#define EPD_PORT_PANEL_ON PORTD
#define EPD_DDR_PANEL_ON DDRD

// BORDER is not connected on the EPD 2.7 board
//#define EPD_PIN_BORDER
//#define EPD_PORT_BORDER
//#define EPD_DDR_BORDER

// output
#define EPD_PIN_DISCHARGE PD4
#define EPD_PORT_DISCHARGE PORTD
#define EPD_DDR_DISCHARGE DDRD

// output
#define EPD_PIN_PWM PD5
#define EPD_PORT_PWM PORTD
#define EPD_DDR_PWM DDRD

// output
#define EPD_PIN_RESET PD6
#define EPD_PORT_RESET PORTD
#define EPD_DDR_RESET DDRD

//input
#define EPD_PIN_BUSY PD7
#define EPD_PORT_BUSY PORTD
#define EPD_INPUT_PORT_BUSY PIND
#define EPD_DDR_BUSY DDRD

#define STAGE_TIME 630
#define LINES_PER_DISPLAY 176
#define DOTS_PER_LINE 264
#define BYTES_PER_LINE 264/8
#define BYTES_PER_SCAN 176/4
#define FILLER true



#endif /* EPD27_CONF_H */
