#include "io_control.h"
#include "constants.h"
#include "interrupt.h"
#include "status.h"
#include "memory_control.h"
#include <xc.h>

void initIO(void)
{
#ifdef CONFIG_ON_BOOT
    
    loadBootConfig();
    if (!OPERATION_SUCCESS())
    {
        resetIO();
    }
#else
    resetIO();
#endif
    
    //Digital I/O
    ANSELx = 0x00;
        
    //Init the Interrupt Pin
    
#ifdef ENABLE_OPEN_DRAIN
    //Open Drain, Output
    INT_ODCON = 1;
#endif
    
//INT pin is always active low (even when Open Drain is disabled)
    INT_TRIS = 0;
    INT_LAT = 1;
}

void resetIO(void)
{
    //Disable the IOC Interrupts
    resetIOC_pins();
    
    //Load Defaults for PORT
    LATx = DEFAULT_LATx;
    TRISx = DEFAULT_TRISx;
    WPUx = DEFAULT_WPUx;
    SLRCONx = DEFAULT_SLRCONx;
    ODCONx = DEFAULT_ODCONx;
    INLVLx = DEFAULT_INLVLx;
}

inline void setPORT_TRIS(uint8_t input)
{
    TRISx = input;
}

inline uint8_t getPORT_TRIS(void)
{
    return TRISx;
}

inline void setPORT_LAT(uint8_t value)
{
    LATx = value;
}

inline uint8_t getPORT_LAT(void)
{
    return LATx;
}

inline void setPORT_WPU(uint8_t value)
{
    WPUx = value;
}

inline uint8_t getPORT_WPU(void)
{
    return WPUx;
}
    
inline void setPORT_INLVL(uint8_t value)
{
    INLVLx = value;
}

inline uint8_t getPORT_INLVL(void)
{
    return INLVLx;
}

inline void setPORT_ODCON(uint8_t value)
{
    ODCONx = value;
}

inline uint8_t getPORT_ODCON(void)
{
    return ODCONx;
}

inline void setPORT_SLRCON(uint8_t value)
{
    SLRCONx = value;
}

inline uint8_t getPORT_SLRCON(void)
{
    return SLRCONx;
}
    
inline uint8_t getPORT_PORT(void)
{
    return PORTx;    
}

inline void assert_INT(void)
{
    INT_LAT = 0;
}

inline void release_INT(void)
{
    INT_LAT = 1;
}