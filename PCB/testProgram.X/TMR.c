#include "TMR.h"

#include <xc.h>
#include <stdint.h>

void TMR0_init(void)
{
    //Disabled, 8-bit Mode, 1:1 Postscaler
    T0CON0 = 0x00;
    
    //LFINTOSC, Not Synced to Fosc/4, 1:2048 prescaler
    T0CON1 = 0x00;
    T0CON1bits.CS = 0b100;
    T0CON1bits.ASYNC = 0b1;
    T0CON1bits.CKPS = 0b1011;
    
    //Period Value for Timer 0 (0.5s)
    TMR0H = 8;
}

void TMR0_start(void)
{
    T0CON0bits.T0EN = 0b1;
}

void TMR0_stop(void)
{
    T0CON0bits.T0EN = 0b0;
}

void TMR0_enableISR(void)
{
    PIE0bits.TMR0IE = 1;
}

void TMR0_disableISR(void)
{
    PIE0bits.TMR0IE = 0;
}

void TMR1_init(void)
{
    //1:1 pre-scaler, async mode, 16-bit read
    T1CON = 0x00;
    T1CONbits.NOT_SYNC = 1;
    T1CONbits.RD16 = 1;
    
    //Timer is always counting (no gate)
    T1GCON = 0x00;
    
    //Select LFINTOSC (~2s period)
    T1CLK = 0b00100;
    
    //T1GATE = PPS
    T1GATE = 0x00;
}

void TMR1_start(void)
{
    T1CONbits.ON = 0b1;
}

void TMR1_stop(void)
{
    T1CONbits.ON = 0b0;
}

void TMR1_enableISR(void)
{
    PIE1bits.TMR1IE = 1;
}

void TMR1_disableISR(void)
{
    PIE1bits.TMR1IE = 0;
}

void TMR2_init(void)
{
    //1:1 pre-scaler, 1:1 post-scaler
    T2CON = 0x00;
    
    //No Sync to Fosc/4, Rising Edge, Mode = Free-Running
    T2HLT = 0x00;
    
    //LFINTOSC
    T2CLKCON = 0b100;
    
    //PPS Selected
    T2RST = 0x00;
    
    //Period of Clock (~7.7 ms)
    T2PR = 0xFF;
}

void TMR2_start(void)
{
    T2CONbits.ON = 0b1;
}

void TMR2_stop(void)
{
    T2CONbits.ON = 0b0;
}

void TMR2_enableISR(void)
{
    PIE1bits.TMR2IE = 1;
}

void TMR2_disableISR(void)
{
    PIE1bits.TMR2IE = 0;
}