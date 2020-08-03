#ifndef DATA_CONTROL_H
#define	DATA_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
        
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> write_data(<FONT COLOR=BLUE>uint8_t</FONT> pos, <FONT COLOR=BLUE>uint8_t</FONT> data)</B>
     * @param pos (uint8_t) - targeted location for writing data to
     * @param data (uint8_t) - data to write
     * 
     * This function wraps the core I/O expander functionality.
     * I2C data sent to be written to this device enters this function,
     * and is directed to the appropriate locations and functions.
     *  */
    void write_data(uint8_t pos, uint8_t data);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> read_data(<FONT COLOR=BLUE>uint8_t</FONT> pos)</B>
     * @param pos (uint8_t) - Location of data to read from
     * 
     * This function wraps the core I/O expander functionality.
     * I2C requests for data to send enter this function,
     * which returns the appropriate byte to send.
     *  */
    uint8_t read_data(uint8_t pos);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> isValidAddress(<FONT COLOR=BLUE>uint8_t</FONT> pos)</B>
     * @param pos (uint8_t) - Address to check
     * 
     * This function wraps the core I/O expander functionality by
     * validating if a read is at a proper address. If it is not,
     * the global error and op error are set. 
     * Used to validate if the device should ACK when being asked to TX. 
     *  */
    uint8_t isValidAddress(uint8_t addr);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_COMS_H */

