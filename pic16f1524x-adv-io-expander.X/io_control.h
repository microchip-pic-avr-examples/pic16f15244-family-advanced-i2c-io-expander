#ifndef IO_CONTROL_H
#define	IO_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "config.h"
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initIO(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the I/O that is "expanded".
     *  */
    void initIO(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> resetIO(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the PORTx IO.
     *  */
    void resetIO(void);
    
    /** 
     * <B><FONT COLOR=BLUE> setPORT_TRIS</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> input)</B>
     * @param input (uint8_t) - Value to load into TRIS
     * 
     * This function configures TRIS on the configured PORTx.
     *  */
#define setPORT_TRIS(input) TRISx = input
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_TRIS(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the TRIS register on the configured PORTx.
     *  */
#define getPORT_TRIS() TRISx
    
    /** 
     * <B><FONT COLOR=BLUE> setPORT_LAT</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into LAT
     * 
     * This function configures LAT on the configured PORTx.
     *  */
#define setPORT_LAT(value) LATx = value
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_LAT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the LAT register on the configured PORTx.
     *  */
#define getPORT_LAT() LATx
        
    /** 
     * <B><FONT COLOR=BLUE>setPORT_WPU</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into WPU
     * 
     * This function configures WPU on the configured PORTx.
     *  */
#define setPORT_WPU(config) WPUx = config
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_WPU(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the WPU register on the configured PORTx.
     *  */
#define getPORT_WPU() WPUx
    
    /** 
     * <B><FONT COLOR=BLUE>setPORT_INLVL</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into INLVL
     * 
     * This function configures INLVL on the configured PORTx.
     *  */
#define setPORT_INLVL(config) INLVLx = config
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_INLVL(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the INLVL register on the configured PORTx.
     *  */
#define getPORT_INLVL() INLVLx
    
    /** 
     * <B><FONT COLOR=BLUE>setPORT_ODCON</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into ODCON
     * 
     * This function configures ODCON on the configured PORTx.
     *  */
#define setPORT_ODCON(config) ODCONx = config
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_ODCON(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the ODCON register on the configured PORTx.
     *  */
#define getPORT_ODCON() ODCONx
    
    /** 
     * <B><FONT COLOR=BLUE>setPORT_SLRCON</FONT>(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into SLRCON
     * 
     * This function configures SLRCON on the configured PORTx.
     *  */
#define setPORT_SLRCON(config) SLRCONx = config
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_SLRCON(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the SLRCON register on the configured PORTx.
     *  */
#define getPORT_SLRCON() SLRCONx
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_PORT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function reads the PORT register in PORTx.
     *  */
#define getPORT_PORT() PORTx
        
    /** 
     * <B><FONT COLOR=BLUE>assert_INT</FONT>(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function asserts the INT line if an IOC occurs.
     *  */
#define assert_INT() INT_LAT = 0
    
    /** 
     * <B><FONT COLOR=BLUE>release_INT</FONT>(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function releases the INT line if an IOC occurs.
     *  */
#define release_INT() INT_LAT = 1;

#ifdef	__cplusplus
}
#endif

#endif	/* IO_CONTROL_H */

