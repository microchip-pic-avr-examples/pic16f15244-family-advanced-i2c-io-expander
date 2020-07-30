#include "data_control.h"
#include "constants.h"
#include "io_control.h"
#include "status.h"
#include "interrupt.h"
#include "memory_control.h"
#include "i2c_slave.h"

void write_data(uint8_t pos, uint8_t data)
{
    switch (pos)
    {
        case ACCESS_TRIS1:  //Begin PORT 1
            setPORT_TRIS(data);
            break;
        case ACCESS_LAT1:
            setPORT_LAT(data);
            break;
        case ACCESS_IOCxP:
            enableIOCinterrupts_PORT_pos_pins(data);
            break;
        case ACCESS_IOCxN:
            enableIOCinterrupts_PORT_neg_pins(data);
            break;
        case ACCESS_WPU1:
            setPORT_WPU(data);
            break;
        case ACCESS_INLVL1:
            setPORT_INLVL(data);
            break;
        case ACCESS_ODCON1:
            setPORT_ODCON(data);
            break;
        case ACCESS_SLRCON1:
            setPORT_SLRCON(data);
            break;
        case MEM_CONFIG:
            setMemoryOP(data);
            break;            
        case UNLOCK1_MEM:
        case UNLOCK2_MEM:
            writeKey(data);
            break;
        case I2C_UPDATE_ADDR:   //This occurs on STOP
            data = 0x00;
            break;
        default:
        {
            setErrorCode(ERROR_WRITE_OVERRUN);
        }

    }
}

uint8_t read_data(uint8_t pos)
{
    //return 0xAA;
    switch (pos)
    {
        case GET_STATUS:
            return getStatus();
        case GET_IOCx:
            //Release the Interrupt Line (if asserted)
            release_INT();
            return getIOC_PORT_flags();
        case GET_PORT: //Begin PORT 1 functions
            return getPORT_PORT();
        case ACCESS_TRIS1:
            return getPORT_TRIS();
        case ACCESS_LAT1:
            return getPORT_LAT();
        case ACCESS_IOCxP:
            return getIOCinterrupts_PORT_pos_pins();
        case ACCESS_IOCxN:
            return getIOCinterrupts_PORT_neg_pins();
        case ACCESS_WPU1:
            return getPORT_WPU();
        case ACCESS_INLVL1:
            return getPORT_INLVL();
        case ACCESS_ODCON1:
            return getPORT_ODCON();
        case ACCESS_SLRCON1:
            return getPORT_SLRCON();
    }
    setErrorCode(ERROR_READ_OVERRUN);
    return 0x00;
}