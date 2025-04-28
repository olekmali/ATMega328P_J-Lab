//* testing PINC restarting TIMER1 interrupt  - main.c *
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bios_leds.h"
#include "bios_keys.h"
#include "bios_timer_int.h"
#include "bios_pinc_int.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------

#define INT_FREQUENCY      1000L

//------------------------------------------------------------------------------------
// HAL functions for the interrupts
//------------------------------------------------------------------------------------
inline void T1intON()  { TIMSK1 = (1<<OCIE1A); }
inline void T1intOFF() { TIMSK1 = 0; }
inline void sleep_On() { set_sleep_mode(SLEEP_MODE_PWR_SAVE); sleep_enable(); sleep_cpu(); }
inline void sleep_Off(){ sleep_disable(); }

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t need_to_sleep = 0;
static volatile uint8_t t1_reactivate = 0;


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    static uint16_t counter = 0;
    if ( counter == 0 ) {
        if ( t1_reactivate!=0 ) {
            leds_set( leds_get() | B_L0 ); // this line is for demo purposes
            t1_reactivate = 0;
            counter = 1024;
        } else {
            leds_set( leds_get() & ~B_L0 ); // this line is for demo purposes
            T1intOFF();
            need_to_sleep = 1; // shut down the activity
            // DO NOT power down inside an interrupt as new interrupts are disabled
            // and there will be no wakeup due to a triggered interrupt
        }
    } else {
        counter--;
    }

}

void MyInterruptPinChangedFunction () {
    t1_reactivate = 1;
    T1intON();
    need_to_sleep = 0;
    sleep_Off();
}

void setup_PC012() {
    DDRC =  DDRC  & 0b11111000; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins are input/zero
    PORTC = PORTC | 0b00000111; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins has pull up resistors/one

    // enable the interrupt on the lowest 3 pins of PORTC
    PCINT_PINC_initialize( 0b00000111, &MyInterruptPinChangedFunction );   // enable pin change interrupt on PINC.0
}

void setup_Unused_Pins() {
    // see example power down mode with pin changed interrupt for explanation
    DDRC  &= 0b11000111;
    PORTC |= 0b00111000;
    DDRD  &= 0b00000011;
    PORTD |= 0b11111100;
}

int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();
    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1 );
    setup_PC012();
    setup_Unused_Pins();

    sei();
    need_to_sleep = 1;
    while(1) {
        if ( need_to_sleep!=0 ) {
            need_to_sleep = 0;
            sleep_On();
        }

        {   // the code fragment below is for illustration purposes only
            // it indicates that the main loop is running
            cli(); leds_set( leds_get() | B_L4 ); sei();
            for (volatile uint32_t i =0; i<30000; i++) ;
            cli(); leds_set( leds_get() & ~B_L4 ); sei();
            for (volatile uint32_t i =0; i<30000; i++) ;
        }

    }

    return(0);
}
