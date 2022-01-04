/* 
 * File:   ADC.h
 * Author: C62081
 *
 * Created on December 20, 2021, 4:29 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
typedef enum {
    ADC_TRIG_OFF = 0x00, ADC_PPS, ADC_TMR0, ADC_TMR1, ADC_TMR2, 
        ADC_CCP1, ADC_CCP2, ADC_PWM3, ADC_PWM4, ADC_IOC, ADC_SW_ADRESH = 0b1101
} ADC_Triggers;

//Initialize the ADC
void ADC_init(void);

//Enable ADC Interrupts
void ADC_enableISR(void);

//Disable ADC Interrupts
void ADC_disableISR(void);

//Set Auto-Conversion Trigger
void ADC_setTrigger(ADC_Triggers trigger);

//Immediately start a conversion
void ADC_start(void);

//Get the result
uint16_t ADC_getResult(void);

//Get the high byte of the result (ADRESH)
uint8_t ADC_getHighResult(void);

//Get the low byte of the result (ADRESL)
uint8_t ADC_getLowResult(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

