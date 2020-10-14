//* testing TIMER1 interrupt with sequence generation - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "bios_leds.h"
#include "bios_keys.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
//#define INT_FREQUENCY   38000L
#define INT_FREQUENCY      2500L
#define MAIN_LOOP_FREQUENCY 100

#define SEQ_CNT     2   // how many sequences
#define SEQ_LEN    10   // maximum length of a sequence (otherwise, 0-terminated)
//#define SEQ_STEP 3800
#define SEQ_STEP 250   // how many interrupts for one sequence
#define SEQ_NONE SEQ_CNT


typedef uint16_t udata_t;

const udata_t ARRAY[SEQ_CNT][SEQ_LEN] = {
        { 1, 1, 3, 4, 0, 0, 0, 0, 0, 0 },   // 'A'
        { 3, 1, 3, 1, 3, 4, 0, 0, 0, 0 }    // 'O'
    };

inline void sendON()   { PORTB = PORTB ^ 0b00000001; }
inline void sendOFF()  { PORTB = PORTB & 0b10000000; }
inline void T1intON()  { TIMSK1 = (1<<OCIE1A); }
inline void T1intOFF() { TIMSK1 = 0; }

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t data_next = SEQ_NONE;

void send( uint8_t to_send ) {
    if ( SEQ_NONE == data_next ) {
        data_next = to_send;
        T1intON();
    }
}

//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    static udata_t loop_counter;
    static udata_t step_counter;

    static uint8_t data_this = SEQ_NONE;
    static uint8_t data_step;
    static uint8_t data_on;

    if ( loop_counter!=0 ) {
        loop_counter--;
    } else {
        loop_counter = SEQ_STEP;

        if ( SEQ_NONE == data_this ) {
            if ( SEQ_NONE != data_next ) {
                data_this = data_next;
                data_next = SEQ_NONE;
                data_step = 0;
                step_counter = ARRAY[data_this][data_step];
                step_counter--;
                data_on = 1;
            } else {
                data_on = 0;
                // disable the timer interrupt
                // so that it does not continue
                // until enabled again
                T1intOFF();
            }
            } else {
            if ( 0 < step_counter) {
                step_counter--;
                } else {
                data_step++;
                if ( 0 != ARRAY[data_this][data_step] ) {
                    data_on = ! data_on;
                    step_counter = ARRAY[data_this][data_step];
                    step_counter--;
                } else {
                    data_on = 0;
                    data_this = SEQ_NONE;
                }
            }
        }
    }

    if (data_on) sendON();
    else sendOFF();
}



int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1 );

    sei();

    uint8_t but_prev = 0;
    while(1) {
        uint8_t but_cur = keys_get();
        uint8_t but_chg = ( but_cur^but_prev ) & but_cur;
        but_prev = but_cur; // important! update what is now current will be past next time

        if ( (but_chg & B_K4) !=0 ) send(0);
        if ( (but_chg & B_K5) !=0 ) send(1);
    }

    return(0);
}
