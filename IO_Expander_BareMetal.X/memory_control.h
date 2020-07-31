#ifndef MEMORY_CONTROL_H
#define	MEMORY_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "config.h"
    
    //Reserve the row that is erased
    const uint8_t data[32] __at(MEM_START) = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                                               17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initMemoryControl(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes the memory state machines.
     *  */
    void initMemoryControl(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> setMemoryOP(<FONT COLOR=BLUE>uint8_t</FONT> op)</B>
     * @param op (uint8_t) - Operation to set in the memory state machine
     * 
     * This function sets the memory operation to be performed.
     * Does not perform the operation.
     *  */
    void setMemoryOP(uint8_t op);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> writeKey(<FONT COLOR=BLUE>uint8_t</FONT> key)</B>
     * @param key (uint8_t) - key to enter into the state machine
     * 
     * This function writes the keys required to enable memory operations.
     * See constants.h for the key sequence.
     *  */
    void writeKey(uint8_t key);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearUnlock(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function resets the unlock status of the state machine.
     *  */
    void clearUnlock(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> isPendingMemoryOP(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Returns 1 if a memory OP was received, or 0 if no ops were sent.
     *  */
    uint8_t isPendingMemoryOP(void);
    
    /** 
     * <B><FONT COLOR=BLUE>uint8_t</FONT> isValidMemoryUnlock(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * Returns 1 if a memory OP is unlocked. Returns 0 if no op or if not unlocked.
     *  */
    uint8_t isValidMemoryUnlock(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> runMemoryOP(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function runs the memory operation. The function resets the "addr"
     * used by the I2C read to 0x00 for easy reading of status afterwards.
     * 
     * Verifies that the unlocking sequence has been followed. Throws an error
     * if this fails.
     *  */
    void runMemoryOP(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _erase_row(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function erases a row of memory. If an error is set in OPstatus, then
     * the function does not execute.
     *  */
    void _erase_row(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _write_row(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function programs a row of memory. If an error is set in status, then
     * the function does not execute.
     * 
     * Note: Does NOT erase.
     *  */
    void _write_row(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _read_row(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function reads a row of memory to the internal buffer. If an error
     * is set in status, then the function does not execute.
     *  */
    void _read_row(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _verify_row(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function verifies that a row of memory matches the internal buffer.
     * Sets an error if a mismatch is detected.
     *  */
    void _verify_row(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _apply_configuration(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function makes changes to the desired configuration in the internal buffers.
     * Does NOT write to PFM.
     *  */
    void _apply_configuration(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> _load_configuration(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function unpacks a desired configuration from internal buffers, then
     * verifies the checksum. If the checksum matches, then the configuration is
     * applied.
     * 
     * If the checksum is invalid, then an error is set, and no action occurs.
     *  */
    void _load_configuration(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> loadBootConfig(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function sets the I/O to config 0 on startup. 
     * 
     * If the checksum with config 0 is invalid, no action occurs, and an error
     * is set.
     *  */
    void loadBootConfig(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MEMORY_CONTROL_H */

