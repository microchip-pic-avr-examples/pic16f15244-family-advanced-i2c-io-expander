#ifndef I2C_FSM_H
#define	I2C_FSM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> init_I2C_FSM(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the I2C state machines which enable proper
     * reception and transmission of I2C traffic. 
     *  */
    void init_I2C_FSM(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> reset_I2C_FSM(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the I2C state machines which enable proper
     * reception and transmission of I2C traffic. 
     *  */
    void reset_I2C_FSM(void);

    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> handle_I2C_ISR(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function handles all I2C related interrupts from the MSSP module.
     * Invoked from the interrupt handler.
     *  */
    void handle_I2C_ISR(void); 
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> reset_I2C_addr(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the address being accessed to 0x00.
     * Called by memory write operations.
     *  */
    void reset_I2C_addr(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> isActive_n(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Returns 1 if the i2c module is not active, otherwise returns 0.
     *  */
    uint8_t isActive_n(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_FSM_H */

