#include "interrupt.h"
#include "i2c_fsm.h"
#include "constants.h"
#include "io_control.h"
#include "status.h"

#include <xc.h>

void enableInterrupts(void)
{
    INTCON = 0x00;
    
    INTCONbits.GIE = 1;     //Enable Interrupts
    INTCONbits.PEIE = 1;    //Enable Peripheral Interrupts
}

void disableInterrupts(void)
{
    INTCONbits.GIE = 0;     //Disable Interrupts
    INTCONbits.PEIE = 0;    //Disable Peripheral Interrupts
}

void clearI2Cinterrupt(void)
{
    PIR1bits.SSP1IF = 0;
}

void enableI2Cinterrupt(void)
{
    //Enable Interrupt
    PIE1bits.SSP1IE = 1;
}

void disableI2Cinterrupt(void)
{
    //Disable ISR
    PIE1bits.SSP1IE = 0;
}

void enableIOCinterrupt(void)
{    
    //Enable Interrupt
    PIE0bits.IOCIE = 1;
}

void disableIOCinterrupt(void)
{
    //Disable Interrupt
    PIE0bits.IOCIE = 0;
}

void clearIOCinterrupt(void)
{
    //Clear the edges (only with pins not triggered)
    IOCxF &= ~IOCxF;
    
    //Clear the Interrupt Flag
    PIR0bits.IOCIF = 0;
}

void resetIOC_pins(void)
{
    //Disable IOC interrupts
    disableIOCinterrupt();
    
    //Clear Pin Assignments
    IOCxP = 0x00;
    IOCxN = 0x00;
    
    //Clear pending flags
    IOCxF = 0x00;
    
    //Clear cached IOC pins
    clearIOC_PORT_flags();
}

void enableIOCinterrupts_PORT_pos_pins(uint8_t pins)
{    
    //Enable Positive Edge IOC for selected pins
    IOCxP = pins;
    
    if ((!isIOC_enabled()) && ((IOCxP != 0x00) || (IOCxN != 0x00)))
    {
        enableIOCinterrupt();
    }
    else if ((isIOC_enabled()) && (IOCxP == 0x00) && (IOCxN == 0x00))
    {
        disableIOCinterrupt();
    }
}

uint8_t getIOCinterrupts_PORT_pos_pins(void)
{
    return IOCxP;
}

void enableIOCinterrupts_PORT_neg_pins(uint8_t pins)
{
    //Enable Negative Edge IOC for selected pins
    IOCxN = pins;
    
    if ((!isIOC_enabled()) && ((IOCxP != 0x00) || (IOCxN != 0x00)))
    {
        enableIOCinterrupt();
    }
    else if ((isIOC_enabled()) && (IOCxP == 0x00) && (IOCxN == 0x00))
    {
        disableIOCinterrupt();
    }
}

uint8_t getIOCinterrupts_PORT_neg_pins(void)
{
    return IOCxN;
}

uint8_t isIOC_enabled(void)
{
    return (PIE0bits.IOCIE);
}

void __interrupt() ISR(void)
{
    if (PIR0bits.IOCIF)         //IOC
    {
        //Set the IOC flags
        setIOC_PORT_flags();   
        clearIOCinterrupt();
        
        assert_INT();                 //Assert the INT signal
    }
    if (PIR1bits.SSP1IF)   //I2C
    {
        //Clear the interrupt
        clearI2Cinterrupt();
        handle_I2C_ISR();
    }
    
}