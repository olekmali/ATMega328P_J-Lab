//* blink and toggle in c *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

// this test allows you to do the final tweak of the delay function
int main(void)
{
    leds_init();
    keys_init();

    while(1)
    {
        uint8_t bit5 = leds_get() & 0b00100000;
        bit5 = bit5 ^ 0b00100000;
        leds_set( keys_get() | bit5 );
        _delay_ms(100);
    }

    return(0);
}
