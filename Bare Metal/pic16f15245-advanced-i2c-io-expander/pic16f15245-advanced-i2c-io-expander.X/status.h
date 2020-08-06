#ifndef STATUS_H
#define	STATUS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
    /** 
     * <B><FONT COLOR=BLUE>CLEAR_DEVICE_STATUS()</FONT></B>
     * 
     * This function clears the device status by setting it to ERROR_NONE.
     *  */
#define CLEAR_DEVICE_STATUS() setErrorCode(ERROR_NONE)
    
    /** 
     * <B><FONT COLOR=BLUE>CLEAR_OP_SUCCESS()</FONT></B>
     * 
     * This function clears the operation status flag by setting it to ERROR_NONE.
     *  */
#define CLEAR_OP_SUCCESS()  setOPStatus(ERROR_NONE)
    
    /** 
     * <B><FONT COLOR=BLUE>OPERATION_SUCCESS()</FONT></B>
     * 
     * This function returns 1 if no error occurred, and 0 if an error occurred.
     *  */
#define OPERATION_SUCCESS() (getOPStatus() == ERROR_NONE ? 1 : 0)
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initStatus(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function clears the status and the operation success registers.
     *  */
    void initStatus(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setErrorCode(<FONT COLOR=BLUE>uint8_t</FONT> error)</B>
     * 
     * This function sets an error in the status and operation success bytes.
     *  */
    void setErrorCode(uint8_t error);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getStatus(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the error code (representing status), if an error 
     * occurred.
     *  */
    uint8_t getStatus(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setOPStatus(<FONT COLOR=BLUE>uint8_t</FONT> error)</B>
     * @param error (uint8_t) - error code to set
     * 
     * This function sets the error byte of the operation status register.
     * Does not set the global status byte.
     *  */
    void setOPStatus(uint8_t error);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getOPStatus(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Returns whether a failure occurred during an operation.
     *  */
    uint8_t getOPStatus(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setIOC_PORT_flags(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Sets a byte that marks which bits in PORTx have triggered an IOC.
     *  */
    void setIOC_PORT_flags(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> assert_INT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Returns an internal variable that marks which bits in PORTx have 
     * triggered an IOC.
     *  */
    uint8_t getIOC_PORT_flags(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearIOC_PORT_flags(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Clears the byte representing which bits in PORTx have triggered
     * an IOC event.
     *  */
    void clearIOC_PORT_flags(void);
        
#ifdef	__cplusplus
}
#endif

#endif	/* STATUS_H */

