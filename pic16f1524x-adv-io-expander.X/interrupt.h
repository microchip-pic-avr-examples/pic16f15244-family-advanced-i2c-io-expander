#ifndef INTERRUPT_H
#define	INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
    /** 
     * <B><FONT COLOR=BLUE>void __interrupt()</FONT> ISR(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function receives all interrupts and dispatches them to the appropriate
     * handlers. 
     *  */
    void __interrupt() ISR(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> enableInterrupts(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function enables all global interrupts.
     *  */
    void enableInterrupts(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> disableInterrupts(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function disables all global interrupts.
     *  */
    void disableInterrupts(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearI2Cinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function clears the I2C interrupt flags.
     *  */
    void clearI2Cinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> enableI2Cinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function enables the I2C interrupt.
     *  */
    void enableI2Cinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> disableI2Cinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function disables the I2C interrupt.
     *  */
    void disableI2Cinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearIOCinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function clears (set) IOC flags and the interrupt.
     *  */
    void clearIOCinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> enableIOCinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function enables IOC interrupts.
     *  */
    void enableIOCinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> disableIOCinterrupt(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function disables IOC interrupts.
     *  */
    void disableIOCinterrupt(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> isIOC_enabled(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns 1 if IOC interrupts are enabled, or 0 if they are not.
     *  */
    uint8_t isIOC_enabled(void);
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> resetIOC_pins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * clearIOC_pins - clears all IOC settings for the pins and disables IOC
     *  */
    void resetIOC_pins(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> enableIOCinterrupts_PORT_pos_pins(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param pins (uint8_t) - locations to enable / disable
     * 
     * This function enables positive edge interrupts for PORTx pins using the
     * masked value.
     *  */
    void enableIOCinterrupts_PORT_pos_pins(uint8_t pins);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getIOCinterrupts_PORT_pos_pins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns which pins in PORTx have a positive edge 
     * interrupt enabled.
     *  */
    uint8_t getIOCinterrupts_PORT_pos_pins(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> enableIOCinterrupts_PORT_neg_pins(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param pins (uint8_t) - locations to enable / disable
     * 
     * This function enables negative edge interrupts for PORTx pins using the
     * masked value.
     *  */
    void enableIOCinterrupts_PORT_neg_pins(uint8_t pins);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getIOCinterrupts_PORT_neg_pins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns which pins in PORTx have a negative edge 
     * interrupt enabled.
     *  */
    uint8_t getIOCinterrupts_PORT_neg_pins(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* GENERAL_H */

