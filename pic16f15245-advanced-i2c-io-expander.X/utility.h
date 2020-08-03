#ifndef UTILITY_H
#define	UTILITY_H

#ifdef	__cplusplus
extern "C" {
#endif

/** 
 * <B><FONT COLOR=BLUE>SET_OSC_FREQ(FREQ)</FONT></B>
 * @param FREQ - new value for the oscillator frequency. Must be 3 bits
 * 
 * This function sets the oscillator frequency for the system.
 * Consult the datasheet for the valid values. 
 *  */
#define SET_OSC_FREQ(FRQ) OSCFRQ = FRQ


#ifdef	__cplusplus
}
#endif

#endif	/* UTILITY_H */

