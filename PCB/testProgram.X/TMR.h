#ifndef TMR_H
#define	TMR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void TMR0_init(void);
    void TMR0_start(void);
    void TMR0_stop(void);
    void TMR0_enableISR(void);
    void TMR0_disableISR(void);
    
    void TMR1_init(void);
    void TMR1_start(void);
    void TMR1_stop(void);
    void TMR1_enableISR(void);
    void TMR1_disableISR(void);

    void TMR2_init(void);
    void TMR2_start(void);
    void TMR2_stop(void);
    void TMR2_enableISR(void);
    void TMR2_disableISR(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TMR0_H */

