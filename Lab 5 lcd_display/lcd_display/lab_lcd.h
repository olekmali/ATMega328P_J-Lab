//* lab_lcd.h *
#ifndef LAB_LCD_H_
#define LAB_LCD_H_

#include <stdint.h>

typedef enum { LCD_COMMANDR = 0, LCD_DATAR = 1} lcd_destination_t;

/* Lab 5-1 */
void    lcd_init();
void    lcd_putn_nibble(lcd_destination_t dest, uint8_t data);

/* Lab 5-2 */
void    lcd_wait(void);
uint8_t lcd_get_nibble();
uint8_t lcd_getreg(uint8_t addr);

uint8_t lcd_putreg(uint8_t addr, uint8_t data);

void    lcd_putchr(unsigned char   chr);
void    lcd_putstr(unsigned char * str);

/* Lab 5-3 */
void    lcd_clear(void);
void    lcd_backspace(void);
void    lcd_newline(void);

#endif /* LAB_LCD_H_ */
