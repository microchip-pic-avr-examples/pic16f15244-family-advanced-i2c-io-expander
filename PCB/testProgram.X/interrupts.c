#include "interrupts.h"

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

void Interrupts_init(void)
{
    //Disable all interrupts
    PIE0 = 0x00;
    PIE1 = 0x00;
    PIE2 = 0x00;

    //Clear all flags
    PIR0 = 0x00;
    PIR1 = 0x00;
    PIR2 = 0x00;
    
    //Clear INTCON
    INTCON = 0x00;
}

void Interrupts_enableGlobal(void)
{
    INTCONbits.GIE = 0b1;
}

void Interrupts_enablePeripheral(void)
{
    INTCONbits.PEIE = 0b1;
}

static void __interrupt() ISR(void)
{
    if (PIR0bits.TMR0IF)
    {
        //Clear Timer 0 Flag
        PIR0bits.TMR0IF = 0b0;
    }
    if (PIR1bits.TMR1IF)
    {
        //Clear Timer 1 Flag
        PIR1bits.TMR1IF = 0b0;
    }
    if (PIR1bits.TMR2IF)
    {
        //Clear Timer 2 Flag
        PIR1bits.TMR2IF = 0b0;
    }
    if (PIR1bits.ADIF)
    {
        //ADC Result Ready
        PIR1bits.ADIF = 0b0;
    }
    if (PIR0bits.IOCIF)
    {
        //Clear Flag in RA2
        IOCAF = 0x00;
        
        //Interrupt on Change
        PIR0bits.IOCIF = 0b0;
    }
}
