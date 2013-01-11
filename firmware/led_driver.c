#include "led_driver.h"
#include <avr/pgmspace.h>
#include "uart.h"


void led_init(void) {
  uart_puts_P("initializing LED driver\r\n");

}

void led_good(void) {
  uart_puts_P("LED good.\r\n");
}

void led_intermediate(void) {
  uart_puts_P("LED intermediate.\r\n");
}

void led_bad(void) {
  uart_puts_P("LED bad.\r\n");
}

void led_window_open(void) {
  uart_puts_P("LED window open.\r\n");
}

void led_disconnected(void) {
  uart_puts_P("LED disconnected.\r\n");
}

void led_error(void) {
  uart_puts_P("LED error.\r\n");
}
