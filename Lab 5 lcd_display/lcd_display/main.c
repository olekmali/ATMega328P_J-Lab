//* blink and toggle in c *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "lab_lcd.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

// this test allows you to do the final tweak of the delay function
int main()
{
    leds_init();
    keys_init();

    lcd_init();

    while(1)
    {
        leds_set( leds_get() ^ B_L5 );
        _delay_ms(1000);
    }

    return(0);
}
