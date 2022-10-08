//*  i2c library use example for BMP 280 *
#include "bios_uart0.h"
#include "bios_twi_master.h"
#include "lib_bmp280.h"

#include <stdlib.h>

#define F_CPU 16000000UL
#include <util/delay.h>



int main(void)
{
    uart0_initialize(9600);
    tw_init(TW_FREQ_400K, true); // set I2C Frequency, enable internal pull-up
    bmp_init();
    bmp_data_t readings;

	uart0_puts("Read accelerometer data.\r\n");
    while (1)
    {
        char buffer[15]; // make sure it is big enough!
		bmp_get_data(&readings);
        dtostrf(readings.pressure, 8, 2, buffer);
        uart0_puts(buffer);
        uart0_puts(" hPa ");
        dtostrf(readings.fTemp, 8, 2, buffer);
        uart0_puts(buffer);
        uart0_puts(" F\r\n");
		_delay_ms(200);
    }
}

