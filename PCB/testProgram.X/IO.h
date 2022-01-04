#ifndef IO_H
#define	IO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    //Init the GPIO
    void IO_init(void);
    
    //Enable Interrupt on Change Interrupts
    void IOC_enableISR(void);
    
    //Disable Interrupt on Change Interrupts
    void IOC_disableISR(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

