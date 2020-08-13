#include "memory_control.h"
#include "constants.h"
#include "interrupt.h"
#include "io_control.h"
#include "status.h"
#include "CRC.h"
#include "i2c_fsm.h"

#include <xc.h>
#include <stdint.h>

//Variable for Unlocking Sequence
static volatile uint8_t unlockSeq = 0x00;

//Memory Block (for Saves and Loads)
static uint16_t memBlock[32];

//Memory OP - Decoded Field
static volatile union
{
    struct{ //LSB to MSB
        unsigned SRC : 2; 
        unsigned DST : 2;
        unsigned OP : 2;
        unsigned BEHAVIOR : 2;
    };
    uint8_t field;
} MEMORY_TARGET;

void initMemoryControl(void)
{
    MEMORY_TARGET.field = 0x00;
    unlockSeq = 0x00;
}

void writeKey(uint8_t key)
{
    if (unlockSeq == 1)
    {
        if (key == MEM_UNLOCK_KEY1)
        {
            unlockSeq = 2;
        }
        else
        {
            unlockSeq = INVALID_UNLOCK;
            setErrorCode(ERROR_MEM_INVALID_SEQ);
        }
    }
    else if (unlockSeq == 2)
    {
        if (key == MEM_UNLOCK_KEY2)
        {
            unlockSeq = 3;
        }
        else
        {
            unlockSeq = INVALID_UNLOCK;
            setErrorCode(ERROR_MEM_INVALID_SEQ);
        }
    }
    else
    {
        unlockSeq = INVALID_UNLOCK; 
        setErrorCode(ERROR_MEM_INVALID_SEQ);
    }
}

void setMemoryOP(uint8_t op)
{
    MEMORY_TARGET.field = op;
    unlockSeq = 1;
}

void clearUnlock(void)
{
    unlockSeq = 0x00;
}

uint8_t isPendingMemoryOP(void)
{
    if (unlockSeq > 0)
    {
        return 1;
    }
    
    return 0;
}

uint8_t isValidMemoryUnlock(void)
{
    if (unlockSeq == 3)
    {
        //If an error occurred on device, abort the write, even if unlocked
        if (!OPERATION_SUCCESS())
        {
            setErrorCode(ERROR_MEM_OP_ABORTED);
            clearUnlock();

            //Assert the interrupt to indicate that it is done
            assert_INT();
            return 0;
        }
        return 1;
    }
    setErrorCode(ERROR_ILLEGAL_MEM_OP);
    clearUnlock();
    return 0;
}

void runMemoryOP(void)
{
    disableInterrupts();
    release_INT();
    reset_I2C_addr();
    
    if (!isValidMemoryUnlock())
    {
        clearI2Cinterrupt();
        enableInterrupts();
        assert_INT();
        return;
    }
    
    //Clear any prev. device errors.
    CLEAR_DEVICE_STATUS();
    
    //Clear the unlock to keep it from rewriting
    clearUnlock();
    
    //If not resetting config - load a copy from memory.
    if (MEMORY_TARGET.OP != 0b00)
    {
        //If not resetting, then cache the memory row for read/writes
        _read_row();
    }
    
    switch(MEMORY_TARGET.OP)
    {
        case 0b00:  //Reset to defaults
        {
            resetIO();
            break;
        }
        case 0b01:  //Save
        {
            //Make Changes
            _apply_configuration();
            
            //Erase memory
            _erase_row();
            
            //Apply changes
            _write_row();
            
            break;
        }
        case 0b10:  //Load
        {
            //Set the pin state
            _setPinState();
            
            //Load Configuration
            _load_configuration();
            
            break;
        }
        case 0b11: //Save and Load
        {
            //Make Changes
            _apply_configuration();
            
            //Set the pin state
            _setPinState();
            
            //Erase memory
            _erase_row();
            
            //Apply changes
            _write_row();
            
            //Load Configuration
            _load_configuration();
                        
            break;
        }
    }
        
    clearI2Cinterrupt();
    enableInterrupts();
    
    //Indicate Completion 
    assert_INT();
}

void _apply_configuration(void)
{
    uint8_t index = (MEMORY_TARGET.DST << 3);
    
    uint8_t temp[8];
    temp[0] = getPORT_TRIS();
    temp[1] = getPORT_LAT();
    temp[2] = getIOCinterrupts_PORT_pos_pins();
    temp[3] = getIOCinterrupts_PORT_neg_pins();
    temp[4] = getPORT_WPU();
    temp[5] = getPORT_INLVL();
    temp[6] = getPORT_ODCON();
    temp[7] = getPORT_SLRCON();
    
    //Reset the CRC
    clearCRC();
    
    uint8_t t_index = 0;
    
    //Feed the data into the CRC and reset memBlock
    while (t_index < 8)
    {
        memBlock[t_index + index] = 0x0000;
        feedNumber(temp[t_index]);
        t_index++;
    }
    
    uint8_t checksum = getRemainder();
    
    //Load the checksum into fragments
    memBlock[index] = ((checksum & 0xC0) << 2);
    memBlock[index + 1] = ((checksum & 0x30) << 4);
    memBlock[index + 2] = ((checksum & 0x0C) << 6);
    memBlock[index + 3] = ((checksum & 0x03) << 8);

    t_index = 0;
    
    //Load in parameters
    while (t_index < 8)
    {
        memBlock[t_index + index] |= temp[t_index];
        t_index++;
    }
}

inline void __unlockMemory(void)
{
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    
    NVMCON1bits.WR = 1;
}

void _erase_row(void)
{
    if (!OPERATION_SUCCESS())
        return;
    
    //Erases the memory row
    NVMCON1 = 0x00;
    NVMCON1bits.NVMREGS = 0b0;  //Select User Memory
    
    //Select the base address
    NVMADR = MEM_START;
    
    NVMCON1bits.FREE = 1;
    NVMCON1bits.WREN = 1;
    
    __unlockMemory();
    //Erase happens after unlock
    
    //Clear Write Enable
    NVMCON1bits.WREN = 0;
}

void _write_row(void)
{
    if (!OPERATION_SUCCESS())
        return;
        
    NVMCON1 = 0x00;
    NVMCON1bits.NVMREGS = 0;

    NVMADR = MEM_START;
    
    NVMCON1bits.FREE = 0;
    NVMCON1bits.LWLO = 1;
    NVMCON1bits.WREN = 1;
    
    for (uint8_t i = 0; i < 32; ++i)
    {
        NVMDAT = memBlock[i];
        
        if (i == 31)
        {
            NVMCON1bits.LWLO = 0;
        }

        __unlockMemory();
        
        if (i != 31)
        {
            NVMADRL++;
        }
    }
    
    NVMCON1bits.LWLO = 0;
    
    __unlockMemory();
    
    //Lock Writes
    NVMCON1bits.WREN = 0;
    
    _verify_row();
}

void _read_row(void)
{
    if (!OPERATION_SUCCESS())
        return;
    
    NVMCON1 = 0x00;
    NVMCON1bits.NVMREGS = 0;    //Select User Memory
    
    NVMADR = MEM_START;
    
    for (uint8_t i = 0; i < 32; ++i)
    {
        NVMCON1bits.RD = 1;
        
        //Read the data
        memBlock[i] = NVMDAT;
        
        NVMADR += 1;
    }
}

void _verify_row(void)
{
    NVMCON1 = 0x00;
    NVMCON1bits.NVMREGS = 0;    //Select User Memory
    
    NVMADR = MEM_START;
    
    for (uint8_t i = 0; i < 32; ++i)
    {
        NVMCON1bits.RD = 1;
        
        if (memBlock[i] != NVMDAT)
        {
            setErrorCode(ERROR_WRITE_VERIFY);
            return;
        }
        
        NVMADR += 1;
    }
}

void _load_configuration(void)
{   
    uint8_t checksum = 0x00;
    uint8_t index = (MEMORY_TARGET.SRC << 3);
    uint8_t t_index = 0;
    
    uint8_t memoryTemp [8];
    
    //Reset CRC
    clearCRC();
    
    while (t_index < 4)
    {
        checksum = checksum << 2;
        checksum |= memBlock[index + t_index] >> 8;
        memoryTemp[t_index] = memBlock[index + t_index] & 0xFF;
        
        //Push the 1st 4 elements into the CRC
        feedNumber(memoryTemp[t_index]);
        
        t_index++;
    }
    
    while (t_index < 8)
    {
        memoryTemp[t_index] = memBlock[index + t_index] & 0xFF;

        //Push the last 4 elements into the CRC
        feedNumber(memoryTemp[t_index]);
        t_index++;
    }
    
    //CRC Good?
    if (getRemainder() == checksum)
    {
        //Data validated
        //Load it into registers
        
        setPORT_TRIS(memoryTemp[0]);
        setPORT_LAT(memoryTemp[1]);
        enableIOCinterrupts_PORT_pos_pins(memoryTemp[2]);
        enableIOCinterrupts_PORT_neg_pins(memoryTemp[3]);
        setPORT_WPU(memoryTemp[4]);
        setPORT_INLVL(memoryTemp[5]);
        setPORT_ODCON(memoryTemp[6]);
        setPORT_SLRCON(memoryTemp[7]);
        
        //Clear any flags that may have been set during init
        clearIOCinterrupt();
        
        //Clear any older flags that were cached before this
        clearIOC_PORT_flags();
    }
    else
    {
        //Failed
        setErrorCode(ERROR_CRC_FAILED);
    }   
}

void _setPinState(void)
{
   //Set the pin state while loading memory
   if (MEMORY_TARGET.BEHAVIOR == 0b01)
   {
       //Output Low

       setPORT_TRIS(0x00);
       setPORT_LAT(0x00);

   }
   else if (MEMORY_TARGET.BEHAVIOR == 0b10)
   {
       //Output High

       setPORT_TRIS(0x00);
       setPORT_LAT(0xFF);
   }
   else if (MEMORY_TARGET.BEHAVIOR == 0b11)
   {
       //Tri-State
       setPORT_TRIS(0xFF);

        //Shutdown Weak Pullups
       setPORT_WPU(0x00);
   }
}

void loadBootConfig(void)
{
    MEMORY_TARGET.field = 0x00;
    
    _read_row();
    _load_configuration();
}