#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "at45_test.h"
#include "uart.h"
#include "at45.h"



void at45_test_init(void) {
  uart_puts_P(" Deleting AT45 (all pages): ");
  at45_erase_all_pages();
  while (! at45_is_ready()) {
    _delay_ms(500);
    uart_puts_P(".");
  }
  uart_puts_P(" done.\r\n");
}

void at45_test_loop(void) {
  at45_test_buffer1();
  at45_test_write_page();
}

void at45_test_buffer1(void) {
  char conversion_buffer[50];
  uart_puts_P(" Querying AT45 status: ");
  uint8_t at45_stat = at45_status();
  itoa(at45_stat, conversion_buffer, 10);
  uart_puts(conversion_buffer);
  if (at45_is_ready()) {
    uart_puts_P(", device ready.\r\n");
  } else {
    uart_puts_P(", device NOT ready.\r\n");
  }

  uint8_t buf1[4];
  uart_puts_P(" Deleting buffer 1: ");
  memset(buf1, 0, 4);
  at45_write_to_buf_1(buf1, 4, 0);
  uart_puts_P("complete.\r\n");

  uart_puts_P(" Reading buffer 1: ");
  memset(buf1, 0, 4);
  at45_read_from_buf_1(buf1, 4, 0);
  itoa(buf1[0], conversion_buffer, 10);
  uart_puts(conversion_buffer);
  uart_puts_P(" received.\r\n");

  uart_puts_P(" Writing buffer 1: ");
  memset(buf1, 0, 4);
  buf1[0]=23;
  buf1[1]=42;
  at45_write_to_buf_1(buf1, 4, 0);
  uart_puts_P("complete.\r\n");

  uart_puts_P(" Reading buffer 1 again: ");
  memset(buf1, 0, 4);
  at45_read_from_buf_1(buf1, 4, 0);
  itoa(buf1[0], conversion_buffer, 10);
  uart_puts(conversion_buffer);
  uart_puts_P(" received.\r\n");

}

void at45_test_write_page(void) {
  uint8_t buf2[AT45_PAGE_SIZE];
  char conversion_buffer[50];

  uart_puts_P(" Reading content from flash:");
  memset(buf2, 0x00, AT45_PAGE_SIZE);
  if (at45_read_page_to_buf_1(0)) {
    uart_puts_P(" Error transferring flash page to buffer1\r\n");
  } else {
    uart_puts_P(" Transfered flash page to buffer1 successfully.\r\n");
  }
  at45_read_from_buf_1(buf2, AT45_PAGE_SIZE, 0);
  for( uint16_t i = 0; i < AT45_PAGE_SIZE; i += 1) {
    itoa(buf2[i], conversion_buffer, 16);
    uart_puts(conversion_buffer);
  }
  uart_puts_P(" received.\r\n");

  uart_puts_P(" Generating increased buffer: ");
  memset(buf2, buf2[0]+1, AT45_PAGE_SIZE);
  at45_write_to_buf_1(buf2, AT45_PAGE_SIZE, 0);
  if (at45_write_from_buf_1(0)) {
    uart_puts_P(" Error transferring buffer1 to flash.\r\n");
  } else {
    uart_puts_P(" Transfered buffer1 to flash successfully.\r\n");
  }

  uart_puts_P(" Reading 0x1Fs from flash again:");
  memset(buf2, 0x00, AT45_PAGE_SIZE);
  if (at45_read_page_to_buf_1(0)) {
    uart_puts_P(" Error transferring flash page to buffer1\r\n");
  } else {
    uart_puts_P(" Transfered flash page to buffer1 successfully.\r\n");
  }
  at45_read_from_buf_1(buf2, AT45_PAGE_SIZE, 0);
  for( uint16_t i = 0; i < AT45_PAGE_SIZE; i += 1) {
    itoa(buf2[i], conversion_buffer, 16);
    uart_puts(conversion_buffer);
  }
  uart_puts_P(" received.\r\n");
}


