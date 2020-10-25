//* testing TIMER1 interrupt with sequence generation - main.c *
#include <stdint.h>
#include <avr/interrupt.h>

#include "bios_timer_int.h"
#include "bios_leds.h"
#include "bios_keys.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
//#define INT_FREQUENCY   72000L // For IR remote
//#define SEQ_STEP 3800

#define INT_FREQUENCY      2000L // For 1kHz modulated Morse code signals
#define STEP_SIZE 400       // 0.2s per 1 unit of the array



//------------------------------------------------------------------------------------
// HAL functions for the interrupts
//------------------------------------------------------------------------------------
inline void T1intON()  { TIMSK1 = (1<<OCIE1A); }
inline void T1intOFF() { TIMSK1 = 0; }

// Passive Buzzer Morse Code / IR Remote
//inline void sendON()        { }
//inline void sendOFF()       { PORTB = PORTB & 0b11111110; }
//inline void sendStartONE()  { PORTB = PORTB | 0b00000001; }
//inline void sendStartTWO()  { PORTB = PORTB & 0b11111110; }
//inline void sendGoOnONE()   { PORTB = PORTB ^ 0b00000001; }
//inline void sendGoOnTWO()   { PORTB = PORTB & 0b11111110; }

// On-off Morse code
inline void sendON()        { }
inline void sendOFF()       { PORTB = PORTB & 0b11111110; }
inline void sendStartONE()  { PORTB = PORTB | 0b00000001; }
inline void sendStartTWO()  { PORTB = PORTB & 0b11111110; }
inline void sendGoOnONE()   { }
inline void sendGoOnTWO( )  { }

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static const uint16_t* data_next_pointer = 0;

void sendMessage( const uint16_t to_send[] ) {
    if ( 0 == data_next_pointer ) {
        data_next_pointer = to_send;
        T1intON();
    }
}


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // sender incoming data cache
    static const uint16_t* data_cur_pointer = 0;
    // sender state machine
    static uint32_t step_counter;
    static uint8_t  data_step_nr;
    static uint8_t  data_digit;

    if ( 0 == data_cur_pointer ) {
        if ( 0 != data_next_pointer ) {
            // accepting the next sequence
            data_cur_pointer = data_next_pointer;
            data_next_pointer = 0;
            // starting a new sequence
            data_step_nr = 0;
            step_counter = STEP_SIZE * (uint32_t)data_cur_pointer[data_step_nr];
            data_digit = 1;
            sendON();
            sendStartONE();
        } else {
            // disable the timer interrupt
            // so that it does not continue until enabled again
            sendOFF();
            T1intOFF();
        }
    } else {
        if ( 0 < step_counter) {
            step_counter--;
            if (data_digit) sendGoOnONE(); else sendGoOnTWO(); // keep transmitting
        } else {
            data_step_nr++;
            if ( 0 != data_cur_pointer[data_step_nr] ) {
                step_counter = STEP_SIZE * (uint32_t)data_cur_pointer[data_step_nr];
                data_digit = ! data_digit;
                if (data_digit) sendStartONE(); else sendStartTWO(); // keep transmitting
            } else {
                data_digit = 0;
                data_cur_pointer = 0;
                sendOFF();
            }
        }
    }

}



int main(void)
{
    const static uint16_t morseA[] = { 1, 1, 3, 7, 0 };
    const static uint16_t morseO[] = { 3, 1, 3, 1, 3, 7, 0 };
    const static uint16_t morseSOS[] = { 1, 1, 1, 1, 1, 4, 3, 1, 3, 1, 3, 4, 1, 1, 1, 1, 1, 7, 0 };

    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1 );

    sei();

    while(1) {
        uint8_t but_cur = keys_get();
        if ( (but_cur & B_K4) !=0 )      sendMessage(morseA);
        else if ( (but_cur & B_K5) !=0 ) sendMessage(morseO);
        else if ( (but_cur & B_K6) !=0 ) sendMessage(morseSOS);
    }

    return(0);
}
