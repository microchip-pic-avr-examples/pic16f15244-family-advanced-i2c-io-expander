#include "i2c_fsm.h"
#include "i2c_device.h"
#include "io_control.h"
#include "interrupt.h"
#include "status.h"
#include "data_control.h"
#include "memory_control.h"
#include "timer2.h"

#include "constants.h"
#include <xc.h>
#include <stdbool.h>


static volatile uint8_t bCount;
static volatile uint8_t addr;

static volatile bool busStarted;
static volatile bool dataStarted;

void init_I2C_FSM(void)
{
    busStarted = false;
    dataStarted = false;
    bCount = 0x00;
    addr = 0x00;
}

void handle_I2C_ISR(void)
{            
    //Normally ACK
    SSP1CON2bits.ACKDT = 0;
    
    if (SSP1STATbits.P)
    {        
#ifdef I2C_TIMEOUT_EN
        //Stop I2C Monitor (TMR2)
        stopTMR2();
#endif
        //Stop Interrupt
        //bCount = 0;
        
        reset_I2C_FSM();
        
        //status.field = 0x00;

        //Release the Interrupt Line (if asserted)
        release_INT();
                
        if (addr == I2C_UPDATE_ADDR)
        {
            //Refresh Addressing Pins
            updateI2CAddress();
            
            //Move address to 0
            addr = 0x00;
        }
    }
    else if ((dataStarted) && (!SSP1CON3bits.ACKTIM) && (!SSP1STATbits.RW))
    {
        //When being written to, ignore the 2nd interrupt after ACK
        dataStarted = false;
    }
    else if ((SSP1STATbits.S) && (!SSP1STATbits.BF) && ((!SSP1CON3bits.ACKTIM) && (!dataStarted)))
    {
        //Start is enabled, no buffered data, no ACK/NACK yet, and no STOP
#ifdef I2C_TIMEOUT_EN
        //Start I2C Monitor (TMR2)
        startTMR2();
#endif
                
        //Start Condition
        busStarted = true;
        bCount = 0;
    }
    else if (SSP1STATbits.BF)
    {
        dataStarted = true;
        //Clear any former errors
        
        uint8_t rx = SSP1BUF;
        //Buffer Full
        if (bCount == 0)
        {
            CLEAR_OP_SUCCESS();
            
            if (SSP1STATbits.RW)
            {
                //Test read of data - will set an error if it fails
                if (!((addr <= ACCESS_IOCxN) || 
                        ((addr >= ACCESS_WPU1) && (addr <= ACCESS_SLRCON1))))
                    setErrorCode(ERROR_INVALID_READ_OP);
            }
            
            SSP1CON2bits.ACKDT = !OPERATION_SUCCESS(); //NACK if an error occurred 
        }
        else if (!SSP1STATbits.RW)
        {
            //Writes
            
#ifdef I2C_TIMEOUT_EN
            //Clear the timeout monitor
            clearTMR2();
#endif
            
            if (bCount == 1)
            {
                //Address
                addr = rx;
                if ((addr == UNLOCK1_MEM) || (addr == UNLOCK2_MEM))
                {
                    setErrorCode(ERROR_INVALID_ACCESS);
                }
            }
            else
            {
                //Pass to the write handler
                write_data(addr, rx);
                if (OPERATION_SUCCESS())
                {
                    addr += 1;
                }
                else if (bCount == 2)
                {
                    //Initial Bad Write ADDR
                    //After this byte, the error is an overrun
                    setErrorCode(ERROR_INVALID_WRITE_OP);
                }
            }
            SSP1CON2bits.ACKDT = !OPERATION_SUCCESS();
        }
        
        //Release the clock
        SSP1CON1bits.CKP = 1;
        //Increment the Byte Count
        bCount += 1;
    }
    else if ((busStarted) && (SSP1STATbits.RW) && (!SSP1CON2bits.ACKSTAT))
    {
#ifdef I2C_TIMEOUT_EN
        //Clear the timeout monitor
        clearTMR2();
#endif
        
        //Reads
        SSP1BUF = read_data(addr);
        if (OPERATION_SUCCESS())
        {
            addr += 1;
        }
        SSP1CON1bits.CKP = 1;
        bCount += 1;
    }
}

uint8_t isActive_n(void)
{
    return SSP1STATbits.P;
}

void reset_I2C_addr(void)
{
    addr = 0x00;
}