
#include <pic16f15244.h>

#include "IO.h"

#include <xc.h>
#include <stdint.h>

void IO_init(void)
{
    //RA2 is the INT line (enable weak pull-up)
    TRISA2 = 1;
    ANSELA = 0x00;    
    WPUA2 = 1;
    
    //Clear Flag and Configure IOC for RA2
    IOCAF = 0x00;
    IOCAN = 0b00000100;
    
    //RA4 is the RESET output
    LATA4 = 0;
    TRISA4 = 0;
    
    //RC0 and RC2 are Analog Inputs
    //RC0 is the potentiometer input (connected to RC2)
    //RC2 is the ADC Input
    TRISC0 = 1;
    TRISC2 = 1;
    ANSELCbits.ANSC0 = 1;
    ANSELCbits.ANSC2 = 1;
}

void IOC_enableISR(void)
{
    PIE0bits.IOCIE = 0b1;
}

void IOC_disableISR(void)
{
    PIE0bits.IOCIE = 0b0;
}

