#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdbool.h>
    
    //Init interrupt control
    void Interrupts_init(void);
    
    //Enables global interrupts
    void Interrupts_enableGlobal(void);
    
    //Enables peripheral interrupts
    void Interrupts_enablePeripheral(void);

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

