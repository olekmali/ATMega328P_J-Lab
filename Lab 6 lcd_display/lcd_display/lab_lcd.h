//* lab_lcd.h *
#ifndef LAB_LCD_H_
#define LAB_LCD_H_

#include <stdint.h>

/* Lab 5-1 */
//void  lcd_put_nibble(uint8_t mode, uint8_t data) - library internal
void    lcd_init();

/* Lab 5-2 */
void    lcd_putchr(char   chr);
void    lcd_putstr(char * str);

void    lcd_putcmd(uint8_t cmnd);
void    lcd_clear (void);

//uint8_t lcd_get_nibble() - library internal
void    lcd_wait (void);

/* Lab 5-3 */
uint8_t lcd_getreg(uint8_t addr);
void    lcd_putreg(uint8_t addr, uint8_t data);

void    lcd_backspace(void);
void    lcd_newline(void);

#endif /* LAB_LCD_H_ */
