//* lab_lcd.c *
#include "lab_lcd.h"
#include <avr/io.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

typedef enum { LCD_CMNDR = 0, LCD_DATAR = 1} lcd_destination_t;

/*
    Connections:
    LCD D7 D6 D5 D4 --  PortC 3 2 1 0 (most of the time output)
    LCD E RW RS     --  PortD 2 1 0   (always output)
*/

/* Lab 5-1 essential function for lcd_init(..) and in Lab 5-2 for lcd_put*(..) */
void lcd_put_nibble(lcd_destination_t mode, uint8_t data) {
    // prerequisite: set PortD 210 as OUT

    // prerequisite: E  <- 0

    // RS <- mode (0 command 1 data)
    if ( mode == LCD_CMNDR ) {

    } else {

    }

    // RW <- 0

    // wait tSP1 >= 30ns (1us is OK)

    // set PortC 3210 as OUT (in case it was not yet)

    // D7654 <- PortC3210 <- data

    // E  <- 1

    // wait max(tSP2 >=40ns, tPW >=800ns) (1us is OK)

    // E  <- 0

    // wait tHD2 >=10ns ( 1us is OK)
}

/* Lab 5-1 */
void lcd_init() {
    _delay_ms(20);  // >= 18ms
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // in 8bit mode 0011----
    _delay_ms(10);  // >= 5ms
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // in 8bit mode 0011----
    _delay_ms(1);   // >= 1ms
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // in 8bit mode 0011----
    _delay_ms(1);
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // in 8bit mode -> set to 4bit mode
    _delay_ms(1);
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0 0 1 DL   interface parameters
    lcd_put_nibble(LCD_XXXXR, 0b____); // N F - -    # lines + font
    _delay_ms(1);
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0000       set display, cursor
    lcd_put_nibble(LCD_XXXXR, 0b____); // 1 D C B    and blinking
    _delay_ms(1);
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0 0 0 0    CLS and return home
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0 0 0 1    this takes 2ms+
    _delay_ms(3);
    //--
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0 0 0 0    entry mode
    lcd_put_nibble(LCD_XXXXR, 0b____); // 0 1 ID S   cursor dir + shift
    _delay_ms(1);
    //--
}

/* Lab 5-2 */
void lcd_putchr(char chr) {
    lcd_wait();
    // send both nibbles to the data register
    lcd_put_nibble(LCD_XXXXR, ________________ );
    lcd_put_nibble(LCD_XXXXR, ________________ );
}

void lcd_putstr(char * str) {
    while ( *str != '\0') {
        lcd_putchr(*str);
        ++str;
    }
}

void lcd_putcmd(uint8_t cmnd) {
    lcd_wait();
    // send both nibbles to the command register
    lcd_put_nibble(LCD_XXXXR, ________________ );
    lcd_put_nibble(LCD_XXXXR, ________________ );
}

void lcd_clear(void) {
    lcd_wait();
    // send clear display command - two nibbles or one command
    lcd_putcmd(0b________);
}

/* Lab 5-2 essential function for lcd_wait() */
uint8_t lcd_get_nibble() {
    uint8_t data = 0;
    // prerequisite: set PortD 210 as OUT
    // prerequisite: E  <- 0
    // set PortC 3210 as IN (is already in?)
    // set PortC no pull up resistors
    // RS <- 1
    // RW <- 1
    // wait tSP1 >= 30ns (1us is OK)
    // E  <- 1
    // wait tD >=1us
    // wait 3 more clock cycles (total wait of 2us is OK)
    // read data <- D7654
    // E  <- 0
    // wait tHD2 >=10ns ( 1us is OK)
    return( data );
}

void lcd_wait(void) {
    // start with an empty wait and implement lcd_put*(..) first
    // but at some point implement checking status
    // if the last command is already complete
}

uint8_t lcd_getreg(uint8_t addr) {
    return(0); // replace this with requested code
}

void lcd_putreg(uint8_t addr, uint8_t data) {

}

void lcd_backspace(void) {

}

void lcd_newline(void) {

}
