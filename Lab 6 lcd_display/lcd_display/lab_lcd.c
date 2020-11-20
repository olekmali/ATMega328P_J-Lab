//* lab_lcd.c *
#include "lab_lcd.h"
#include <avr/io.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

typedef enum { LCD_CMNDR = 0, LCD_DATAR = 1} lcd_destination_t;

/*
    Connections:
    LCD D7 D6 D5 D4 --  PortC 3 2 1 0 (most of the time output)
    LCD E RW RS     --  PortB 2 1 0   (always output)
*/

/* Lab 5-1 essential function for lcd_init(..) and in Lab 5-2 for lcd_put*(..) */
void lcd_put_nibble(lcd_destination_t mode, uint8_t data) {
    // prerequisite: E  <- 0

    // RS <- mode (0 command 1 data)
    if ( mode == LCD_CMNDR ) {

    } else {

    }

    // RW <- 0

    // set PortC 3210 as OUT (in case it was not yet)

    // D7654 <- PortC3210 <- data (leave the upper bits alone!)

    // wait tSP1 >= 30ns (1us is OK)

    // E  <- 1

    // wait max(tSP2 >=40ns, tPW >=800ns) (1us is OK)

    // E  <- 0

    // wait max(tHD1 >=10ns, tHD2 >=10ns) ( 1us is OK)
}

/* Lab 5-1 */
void lcd_init() {
    // Set up PortB 210 as OUT and set to 0
    DDRB  = DDRB  | 0b________;
    PORTB = PORTB & 0b________;

    // Set up PortC 3210 as OUT
    DDRC  = DDRC | 0b________;

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

void lcd_putstr(const char * str) {
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

// clears the display
void lcd_clear(void) {
    lcd_wait();
    // send clear display command - two nibbles or one command
    lcd_putcmd(0b________);
}

// returns to 0 position
void lcd_home (void) {
    lcd_wait();
    // send reset position display command - two nibbles or one command
    lcd_putcmd(0b________);
}

// show cursor: 0=off , 1=on
void lcd_cursor(uint8_t on) {
    lcd_wait();
    if (on) {
        // send command
        lcd_putcmd(0b________);
    } else {
        // send command
        lcd_putcmd(0b________);
    }
}

void  lcd_gotoXY(uint8_t r, uint8_t c) {

}



/* Lab 5-2 essential function for lcd_wait() */
uint8_t lcd_get_nibble(lcd_destination_t mode) {
    uint8_t data = 0;
    // ensure the prerequisite: E  <- 0
    // set PortC 3210 as IN
    // set PortC no pull up resistors
    if ( mode == LCD_CMNDR ) {
        // RS <- mode (0 command 1 data)
    } else {
        // RS <- mode (0 command 1 data)
    }
    // RW <- 1
    // wait tSP1 >= 30ns (1us is OK)
    // E  <- 1
    // wait tD >=1us
    // wait 3 more clock cycles (total wait of 2us is OK)
    // read data <- PinC3210 <- D7654
    // E  <- 0
    // wait tHD2 >=10ns ( 1us is OK)
    // RW <- 0
    // wait tHD1 >=10ns ( 1us is OK)
    // set PortC 3210 back as OUT
    return( data );
}

void lcd_wait(void) {
    // start with a few ms wait which is max something can take time to do
    //      e.g. clear() and home() take more than 1ms to complete
    // implement lcd_put*(..) first
    // but at some point implement checking the status and waiting as needed
    //      for the previous command to complete before sending the next one
    // after testing the proper lcd_wait() implement a 5 ms timeout in it

    _delay_ms(1); // <-- replace that with actual wait loop and then with timeout
}
