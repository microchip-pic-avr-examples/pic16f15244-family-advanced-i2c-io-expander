#ifndef IO_CONTROL_H
#define	IO_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
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
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_TRIS(<FONT COLOR=BLUE>uint8_t</FONT> input)</B>
     * @param input (uint8_t) - Value to load into TRIS
     * 
     * This function configures TRIS on the configured PORTx.
     *  */
    inline void setPORT_TRIS(uint8_t input);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_TRIS(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the TRIS register on the configured PORTx.
     *  */
    inline uint8_t getPORT_TRIS(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_LAT(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into LAT
     * 
     * This function configures LAT on the configured PORTx.
     *  */
    inline void setPORT_LAT(uint8_t value);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_LAT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the LAT register on the configured PORTx.
     *  */
    inline uint8_t getPORT_LAT(void);
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_WPU(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into WPU
     * 
     * This function configures WPU on the configured PORTx.
     *  */
    inline void setPORT_WPU(uint8_t value);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_WPU(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the WPU register on the configured PORTx.
     *  */
    inline uint8_t getPORT_WPU(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_INLVL(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into INLVL
     * 
     * This function configures INLVL on the configured PORTx.
     *  */
    inline void setPORT_INLVL(uint8_t value);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_INLVL(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the INLVL register on the configured PORTx.
     *  */
    inline uint8_t getPORT_INLVL(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_ODCON(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into ODCON
     * 
     * This function configures ODCON on the configured PORTx.
     *  */
    inline void setPORT_ODCON(uint8_t value);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_ODCON(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the ODCON register on the configured PORTx.
     *  */
    inline uint8_t getPORT_ODCON(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setPORT_SLRCON(<FONT COLOR=BLUE>uint8_t</FONT> pins)</B>
     * @param value (uint8_t) - Value to load into SLRCON
     * 
     * This function configures SLRCON on the configured PORTx.
     *  */
    inline void setPORT_SLRCON(uint8_t value);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_SLRCON(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the SLRCON register on the configured PORTx.
     *  */
    inline uint8_t getPORT_SLRCON(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getPORT_PORT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function reads the PORT register in PORTx.
     *  */
    inline uint8_t getPORT_PORT(void);
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> assert_INT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function asserts the INT line if an IOC occurs.
     *  */
    inline void assert_INT(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> release_INT(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function releases the INT line if an IOC occurs.
     *  */
    inline void release_INT(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IO_CONTROL_H */

