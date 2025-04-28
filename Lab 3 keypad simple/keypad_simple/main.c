//* simplified keypad test - main.c *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_kpads.h"

int main(void)
{
	leds_init();

	while(1)
	{
		leds_set( keypressed() );

	}
	return(0);
}
