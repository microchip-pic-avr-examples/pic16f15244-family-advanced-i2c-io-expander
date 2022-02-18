#include "timer2.h"

#include "config.h"
#include "interrupt.h"

#include <xc.h>
#include <stdint.h>

void initTMR2(void)
{
    //Timer off, 1:16 Prescaler, 1:1 Postscaler
    T2CON = 0x00;
    T2CONbits.CKPS = 0b100;
    
    //Sync to Fosc/4, Rising Edge, ON not synced, One-Shot Mode (SW Start)
    T2HLT = 0x00;
    T2HLTbits.MODE = 0b01000;
    T2HLTbits.PSYNC = 1;
    
    //Select LFINTOSC
    T2CLKCON = 0b100;
    
    //~25ms timeout
    T2PR = 52; //103;
}

void startTMR2(void)
{
    //Clear Timer 2 Counter
    T2TMR = 0x00;
    
    //Start Timer 2
    T2CONbits.ON = 1;
}

void stopTMR2(void)
{
    T2CONbits.ON = 0;
}

void clearTMR2(void)
{
    //If PIR is set, clear it
    clearTMR2interrupt();
    
    //Clear Count
    T2TMR = 0x00;
}