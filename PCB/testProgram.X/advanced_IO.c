#include "advanced_IO.h"
#include "mssp1_host.h"

#include <xc.h>
#include <stdint.h>

//Location of the Memory OP Register
#define MEM_OP_ADDR 0xA0

//Unlocking Keys for Advanced IO Expander Memory Operations
#define MEM_UNLOCK_1 0xA5
#define MEM_UNLOCK_2 0xF0

static volatile uint8_t memBlock[4];

void advancedIO_init(void)
{
    //Init I/O Settings
    initI2CPins();
    
    //Init MSSP Module in I2C Host Mode
    MSSP_HostInit();
}

void advancedIO_setRegister(ADVANCED_IO_REGISTER reg, uint8_t value)
{
    //1st Byte is address inside the expander
    memBlock[0] = reg;
    
    //2nd Byte is value to write
    memBlock[1] = value;
    
    //Send I2C
    MSSP_WriteBlock(ADVANCED_IO_I2C_ADDR, &memBlock[0], 2);
}

uint8_t advancedIO_getRegister(ADVANCED_IO_REGISTER reg)
{    
    MSSP_RegisterSelectAndRead(ADVANCED_IO_I2C_ADDR, reg, &memBlock[0], 1);
    return memBlock[0];
}

uint8_t advancedIO_getPinState(void)
{
    MSSP_RegisterSelectAndRead(ADVANCED_IO_I2C_ADDR, ADV_IO_PORTx, &memBlock[0], 1);
    return memBlock[0];
}

void advancedIO_toggleBitsInRegister(ADVANCED_IO_REGISTER reg, uint8_t mask)
{
    uint8_t value = advancedIO_getRegister(reg);
    
    value ^= mask;
    
    advancedIO_setRegister(reg, value);
}

void advancedIO_setOutputsHigh(uint8_t mask)
{
    uint8_t value = advancedIO_getRegister(ADV_IO_LATx);
    
    value |= mask;
    
    advancedIO_setRegister(ADV_IO_LATx, value);
}

void advancedIO_setOutputsLow(uint8_t mask)
{
    uint8_t value = advancedIO_getRegister(ADV_IO_LATx);
    
    value &= ~mask;
    
    advancedIO_setRegister(ADV_IO_LATx, value);
}


void advancedIO_setPinsAsInputs(uint8_t mask)
{
    uint8_t value = advancedIO_getRegister(ADV_IO_TRISx);
    
    value |= mask;
    
    advancedIO_setRegister(ADV_IO_TRISx, value);
}

void advancedIO_setPinsAsOutputs(uint8_t mask)
{
    uint8_t value = advancedIO_getRegister(ADV_IO_TRISx);
    
    value &= ~mask;
    
    advancedIO_setRegister(ADV_IO_TRISx, value);
}

void advancedIO_resetToDefault(void)
{
    //Setup Command
    memBlock[0] = MEM_OP_ADDR;
    memBlock[1] = 0x00;     //Reset to default
    memBlock[2] = MEM_UNLOCK_1;
    memBlock[3] = MEM_UNLOCK_2;
    
    //Send I2C Command
    MSSP_WriteBlock(ADVANCED_IO_I2C_ADDR, &memBlock[0], 4);
    
    //Clear Memory Block (to prevent accidental double send)
    memBlock[0] = 0x00;
    memBlock[1] = 0x00;
    memBlock[2] = 0x00;
    memBlock[3] = 0x00;
}

void advancedIO_performMemoryOP(ADVANCED_IO_MEMORY_OP op)
{
    //Setup Command
    memBlock[0] = MEM_OP_ADDR;
    memBlock[1] = op.opCode;
    memBlock[2] = MEM_UNLOCK_1;
    memBlock[3] = MEM_UNLOCK_2;
    
    //Send I2C Command
    MSSP_WriteBlock(ADVANCED_IO_I2C_ADDR, &memBlock[0], 4);
    
    //Clear Memory Block (to prevent accidental double send)
    memBlock[0] = 0x00;
    memBlock[1] = 0x00;
    memBlock[2] = 0x00;
    memBlock[3] = 0x00;
}