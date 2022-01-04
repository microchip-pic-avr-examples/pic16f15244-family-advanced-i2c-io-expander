#ifndef ADVANCED_IO_H
#define	ADVANCED_IO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
    //Available registers to access in the Advanced IO Expander
    typedef enum {
        ADV_IO_ERROR = 0x00, ADV_IO_IOCx, ADV_IO_PORTx, 
        ADV_IO_TRISx, ADV_IO_LATx, ADV_IO_IOCxP, ADV_IO_IOCxN,
        ADV_IO_WPUx = 0x08, ADV_IO_INLVLx, ADV_IO_ODCONx, ADV_IO_SLRCONx
    } ADVANCED_IO_REGISTER;
    
    typedef union {
        struct{
            unsigned SRC : 2;
            unsigned DST : 2;
            unsigned OP : 2;
            unsigned BH : 2;
        };
        
        uint8_t opCode;
    } ADVANCED_IO_MEMORY_OP;
    
#define ADV_IO_BH_NO_CHANGE     0b00
#define ADV_IO_BH_OUTPUT_LOW    0b01
#define ADV_IO_BH_OUTPUT_HIGH   0b10
#define ADV_IO_BH_TRI_STATE     0b11

#define ADV_IO_OP_DEFAULT       0b00
#define ADV_IO_OP_SAVE          0b01
#define ADV_IO_OP_LOAD          0b10
#define ADV_IO_OP_SAVE_LOAD     0b11

//I2C Address to Use
#define ADVANCED_IO_I2C_ADDR 0x60
        
    //Init the IO Expander
    void advancedIO_init(void);
    
    //Set a register in the IO Expander
    void advancedIO_setRegister(ADVANCED_IO_REGISTER reg, uint8_t value);
    
    //Get a register in the IO Expander
    uint8_t advancedIO_getRegister(ADVANCED_IO_REGISTER reg);
    
    //Invert the bits that are set in the mask within the IO Expander
    //Example: Original = 0xFF, Mask = 0xAA
    //Result = 0x55
    void advancedIO_toggleBitsInRegister(ADVANCED_IO_REGISTER reg, uint8_t mask);
    
    //Get the current pin states (high / low)
    uint8_t advancedIO_getPinState(void);
    
    //Sets all masked values high
    void advancedIO_setOutputsHigh(uint8_t mask);
    
    //Sets all masked values low
    void advancedIO_setOutputsLow(uint8_t mask);
    
    //Sets all masked values as inputs
    void advancedIO_setPinsAsInputs(uint8_t mask);
    
    //Sets all masked values as outputs
    void advancedIO_setPinsAsOutputs(uint8_t mask);
    
    //Resets the IO Expander to defaults.
    //Warning: !INT monitoring must be used with this function 
    void advancedIO_resetToDefault(void);
    
    //Performs a memory operation on the IO Expander (see README)
    //Warning: !INT monitoring must be used with this function 
    void advancedIO_performMemoryOP(ADVANCED_IO_MEMORY_OP op);
    
#ifdef	__cplusplus
}
#endif

#endif	/* ADVANCED_IO_H */

