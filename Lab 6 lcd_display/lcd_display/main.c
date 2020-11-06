//* testing LCD display initialization and basic functionality *
#include <stdint.h>
#include "lab_lcd.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

/* begin testing stub */
#include <avr/io.h>

#define L5 (1<<5)
inline void L5init()   { DDRB = DDRB | L5;    }
inline void L5off()    { PORTB = PORTB & ~L5; }
inline void L5on()     { PORTB = PORTB | L5;  }
inline void L5toggle() { PORTB = PORTB ^ L5;  }
/* end testing stub */


// this test allows you to do the final tweak of the delay function
int main()
{
    lcd_init();
    lcd_putchr('A');
    lcd_putstr(" Testing... ");

    L5init();

    char c = ' ';
    while(1)
    {
        _delay_ms(1000);
        L5toggle();

        lcd_putchr(c);
        if (c<'z') c++; else c= ' ';
    }

    return(0);
}
