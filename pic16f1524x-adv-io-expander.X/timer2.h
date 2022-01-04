#ifndef TIMER2_H
#define	TIMER2_H

#ifdef	__cplusplus
extern "C" {
#endif

    /** 
     * <B><FONT COLOR=BLUE>void</FONT> initTMR2(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function initializes Timer 2, but DOES NOT start it. 
     *  */
    void initTMR2(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> startTMR2(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function starts Timer 2.
     *  */
    void startTMR2(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> stopTMR2(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function stops Timer 2.
     *  */
    void stopTMR2(void);
    
    /** 
     * <B><FONT COLOR=BLUE>void</FONT> clearTMR2(<FONT COLOR=BLUE>void</FONT>)</B>
     * 
     * This function clears TMR2's count.
     *  */
    void clearTMR2(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER2_H */

