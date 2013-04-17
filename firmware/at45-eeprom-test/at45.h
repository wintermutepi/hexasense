#ifndef AT45_H
#define AT45_H 1

#include <stdint.h>

#define AT45_PORT PORTB
#define AT45_DDR DDRB
#define AT45_CS PB2
#define AT45_MOSI PB3
#define AT45_MISO PB4
#define AT45_SCK PB5

#define AT45_BUFFER_SIZE 512

void at45_init(void);
void at45_erase_all_pages(void);
uint8_t at45_status(void); 
uint8_t at45_is_ready(void);
uint8_t at45_read_page_to_buf_1(uint32_t addr_page);
uint8_t at45_write_from_buf_1(uint32_t addr_page);
void at45_read_from_buf_1(uint8_t * dst, uint32_t count, uint16_t addr);
void at45_write_to_buf_1(const uint8_t *src, uint32_t count, uint16_t addr);

#endif /* AT45_H */

