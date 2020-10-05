#ifndef I2C_SLAVE_H
#define	I2C_SLAVE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initI2C(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the MSSP module in I2C Slave mode.
     *  */
    void initI2C(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setupI2Cpins(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This functions configures the appropriate I/O pins for I2C operation.
     *  */
    void setupI2Cpins(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getI2CAddress(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This functions returns the current I2C address.
     *  */
    uint8_t getI2CAddress(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> _setI2CAddress(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function updates the I2C address and samples the I/O pins.
     * MSSP module must be stopped prior to calling this function.
     *  */
    void _setI2CAddress(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getI2CAddress(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function stops the MSSP module and updates the I2C address.
     * Can be called by external functions.
     *  */
    void updateI2CAddress(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

