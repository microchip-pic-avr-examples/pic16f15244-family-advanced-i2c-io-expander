#ifndef CRC_H
#define	CRC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initCRC(<FONT COLOR=BLUE>uint8_t</FONT> poly)</B>
     * @param poly (uint8_t) - 8-bit Polynomial to use
     * 
     * This function initializes a software based CRC engine
     * which is used to validate the configurations in memory.
     *  */
    void initCRC(uint8_t poly);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearCRC(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the CRC shifters. 
     *  */
    void clearCRC(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> feedNumber(<FONT COLOR=BLUE>uint8_t</FONT> input)</B>
     * @param input (uint8_t) - 8-bit value to feed in
     * 
     * This function loads an 8-bit value into the CRC "shifter"
     * and runs the CRC.
     *  */
    void feedNumber(uint8_t input);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> getRemainder(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function returns the remainder from the CRC engine.
     *  */
    uint8_t getRemainder(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CRC_H */

