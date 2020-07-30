#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"

//Error code lookup    
#define ERROR_NONE              0
#define ERROR_READ_OVERRUN      1
#define ERROR_WRITE_OVERRUN     2
#define ERROR_INVALID_READ_OP   3
#define ERROR_INVALID_WRITE_OP  4  
#define ERROR_NON_ADDRESS       5
#define ERROR_INVALID_ACCESS    6  
#define ERROR_ILLEGAL_MEM_OP    7
#define ERROR_MEM_OP_FAILED     8
#define ERROR_MEM_OP_ABORTED    9
#define ERROR_MEM_INVALID_SEQ   10
#define ERROR_CRC_FAILED        11
#define ERROR_WRITE_VERIFY      12
    
    
#define MEM_UNLOCK_KEY1         0xA5
#define MEM_UNLOCK_KEY2         0xF0
    
//Get ONLY parameters
#define GET_STATUS   0
#define GET_IOCx    GET_STATUS + 1
#define GET_PORT    GET_IOCx + 1
    
//Shared (GET / SET) Operations
    
//PORT 1 (0x03)
#define ACCESS_TRIS1    GET_PORT + 1
#define ACCESS_LAT1     ACCESS_TRIS1 + 1
#define ACCESS_IOCxP    ACCESS_LAT1 + 1
#define ACCESS_IOCxN    ACCESS_IOCxP + 1
    
//PORT 1 - Special I/O Controls. Offset by 1 to prevent accidental writes
#define ACCESS_WPU1     ACCESS_IOCxN + 2
#define ACCESS_INLVL1   ACCESS_WPU1 + 1
#define ACCESS_ODCON1   ACCESS_INLVL1 + 1
#define ACCESS_SLRCON1   ACCESS_ODCON1 + 1

//Memory settings
#define MEM_CONFIG     0xA0
#define UNLOCK1_MEM    MEM_CONFIG + 1
#define UNLOCK2_MEM    UNLOCK1_MEM + 1
    
//Update I2C Address
#define I2C_UPDATE_ADDR         0xB0
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

