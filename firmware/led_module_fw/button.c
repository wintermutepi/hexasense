#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "hw_config.h"

/**
 * See http://www.mikrocontroller.net/articles/Pollin_Funk-AVR-Evaluationsboard#Tasty_Reloaded
 * This is a modified active low version.
 */

#define debounce( port, pin )                                        \
({                                                                   \
  static uint8_t flag = 0; /* new variable on every macro usage */   \
  uint8_t i = 0;                                                     \
                                                                     \
  if( flag ){                  /* check for key release: */          \
    for(;;){                   /* loop... */                         \
      if( !(port & 1<<pin) ){   /* ... until key pressed or ... */    \
        i = 0;                 /* 0 = bounce */                      \
        break;                                                       \
      }                                                              \
      _delay_us( 98 );         /* * 256 = 25ms */                    \
      if( --i == 0 ){          /* ... until key >25ms released */    \
        flag = 0;              /* clear press flag */                \
        i = 0;                 /* 0 = key release debounced */       \
        break;                                                       \
      }                                                              \
    }                                                                \
  }else{                       /* else check for key press: */       \
    for(;;){                   /* loop ... */                        \
      if( (port & 1<<pin) ){  /* ... until key released or ... */   \
        i = 0;                 /* 0 = bounce */                      \
        break;                                                       \
      }                                                              \
      _delay_us( 98 );         /* * 256 = 25ms */                    \
      if( --i == 0 ){          /* ... until key >25ms pressed */     \
        flag = 1;              /* set press flag */                  \
        i = 1;                 /* 1 = key press debounced */         \
        break;                                                       \
      }                                                              \
    }                                                                \
  }                                                                  \
  i;                           /* return value of Macro */           \
})


void button_init(void) {
  // Button pins as inputs with internal pullup enabled
  BTN0_PORT |= (1<<BTN0_PIN);
  BTN0_DDR &= ~(1<<BTN0_PIN);
  BTN1_PORT |= (1<<BTN1_PIN);
  BTN1_DDR &= ~(1<<BTN1_PIN);
  BTN2_PORT |= (1<<BTN2_PIN);
  BTN2_DDR &= ~(1<<BTN2_PIN);
}

bool is_button0_pressed(void) {
  if (debounce(BTN0_IN, BTN0_PIN)) {
	return true;
  } else {
	return false;
  }
}

bool is_button1_pressed(void) {
  if (debounce(BTN1_IN, BTN1_PIN)) {
	return true;
  } else {
	return false;
  }
}

bool is_button2_pressed(void) {
  if (debounce(BTN2_IN, BTN2_PIN)) {
	return true;
  } else {
	return false;
  }
}
