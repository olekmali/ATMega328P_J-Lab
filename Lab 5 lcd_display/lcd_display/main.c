//* blink and toggle in c *
#include <stdint.h>
#include "lab_lcd.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

// this test allows you to do the final tweak of the delay function
int main()
{
    lcd_init();
    lcd_putchr('*');
    lcd_putstr("Test");

    while(1)
    {
        _delay_ms(1000);
    }

    return(0);
}
