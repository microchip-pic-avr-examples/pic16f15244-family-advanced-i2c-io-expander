#include "ADC.h"

#include <xc.h>
#include <stdint.h>

void ADC_init(void)
{
    //Sample RC2, ADC is OFF
    ADCON0 = 0x00;
    ADCON0bits.CHS = 0b010010;
    
    //Left-Justified, ADCRC, VDD is VREF
    ADCON1 = 0x00;
    ADCON1bits.CS = 0b111;
    
    //Clear the trigger
    ADACT = 0x00; 
    
    //Turn on the ADC
    ADCON0bits.ON = 1;
}

void ADC_enableISR(void)
{
    PIE1bits.ADIE = 1;
}

void ADC_disableISR(void)
{
    PIE1bits.ADIE = 0;
}

void ADC_start(void)
{
    ADCON0bits.GO = 1;
}

void ADC_setTrigger(ADC_Triggers trigger)
{
    ADACT = trigger;
}

uint16_t ADC_getResult(void)
{
    return ADRES;
}
//Get the high byte of the result (ADRESH)
uint8_t ADC_getHighResult(void)
{
    return ADRESH;
}

//Get the low byte of the result (ADRESL)
uint8_t ADC_getLowResult(void)
{
    return ADRESL;
}
