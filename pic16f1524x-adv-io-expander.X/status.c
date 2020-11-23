#include "status.h"
#include "constants.h"

#include <xc.h>
#include <stdint.h>

//Status of the Device
static volatile uint8_t status;
static volatile uint8_t opStat;

//IOC flags
static volatile uint8_t IOC_flags1;

void initStatus(void)
{
    status = ERROR_NONE;
    opStat = ERROR_NONE;
    
    //IOC Bit Flags
    IOC_flags1 = 0x00;
}

void setErrorCode(uint8_t error)
{
    status = error;
    setOPStatus(error);
}

uint8_t getStatus(void)
{
    uint8_t temp = status;
    status = ERROR_NONE;
    return temp;
}

void setOPStatus(uint8_t error)
{
    opStat = error;
}

uint8_t getOPStatus(void)
{
    return opStat;
}

void setIOC_PORT_flags(void)
{
    IOC_flags1 |= IOCxF;
}

uint8_t getIOC_PORT_flags(void)
{
    uint8_t temp = IOC_flags1;
    IOC_flags1 = 0x00;
    return temp;
}

void clearIOC_PORT_flags(void)
{
    IOC_flags1 = 0x00;
}