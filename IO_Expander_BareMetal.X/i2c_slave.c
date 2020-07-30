#include "i2c_slave.h"
#include "interrupt.h"
#include "constants.h"

#include <xc.h>

void initI2C(void)
{
    //Disable I2C Interrupts
    disableI2Cinterrupt();
    
    //Clear Configuration and Turn off I2C
    SSP1CON1 = 0x00;
    
    //Setup IO
    setupI2Cpins();
    
    SSP1STAT = 0x00;
    SSP1STATbits.SMP = 1;       //Disable Slew Rate Control (100kHz)
    SSP1STATbits.CKE = 0;       //Disable SMBUS inputs
    
    SSP1CON1bits.SSPM = 0b0110; //I2C 7-bit Address Mode
    
    SSP1CON2 = 0x00;
    SSP1CON2bits.GCEN = 0;      //Disable General Call
    SSP1CON2bits.ACKDT = 0;     //Polarity of ACK
    SSP1CON2bits.ACKEN = 1;     //Enable ACK
    SSP1CON2bits.RCEN = 1;      //Enable Rx
    SSP1CON2bits.SEN = 0;       //Disable Clock Stretching
    
    SSP1CON3 = 0x00;
    SSP1CON3bits.PCIE = 1;      //Enable STOP Interrupt
    SSP1CON3bits.SCIE = 1;      //Enable START Interrupts
    SSP1CON3bits.SDAHT = 0;     //100ns hold time
    SSP1CON3bits.AHEN = 1;      //Enable Address Hold
    SSP1CON3bits.DHEN = 1;      //Enable Data Hold
    
    _setI2CAddress();           //Sets the I2C Address
    
    SSP1MSK = 0xFE;             //Check 7-bit of the address
        
    //Clear any old ISRs + Enable I2C Interrupt
    clearI2Cinterrupt();
    enableI2Cinterrupt();
    
    //Turn on the Module
    SSP1CON1bits.SSPEN = 1;
    
}

void setupI2Cpins(void)
{
    //RC3 for SCL1
    //RC4 for SDA1
    
    //Pins are Inputs
    TRISB6 = 1;
    TRISB4 = 1;
    
    //Digital Inputs
    ANSELBbits.ANSB6 = 0;
    ANSELBbits.ANSB4 = 0;
    
    //Assign PPS for Outputs
    RB4PPS = 0x08;      //SDA
    RB6PPS = 0x07;      //SCL
   
    RB4I2C = 0b00000001;    //Standard GPIO, I2C thresholds
    RB6I2C = 0b00000001;    //Standard GPIO, I2C Thresholds
    
    //Assign PPS for Inputs
    SSP1DATPPS = 0b001100; // RB4
    SSP1CLKPPS = 0b001110; // RB6

#ifdef ENABLE_ADDR_LINES
    //I2C Address Lines as Inputs
    I2C_ADDR_PIN0_DIR = 1;
    I2C_ADDR_PIN1_DIR = 1;
    I2C_ADDR_PIN2_DIR = 1;
    
    //I2C Address Lines as Digital
    I2C_ADDR_PIN0_ANSEL = 0;
    I2C_ADDR_PIN1_ANSEL = 0;
    I2C_ADDR_PIN2_ANSEL = 0;
#endif
}

uint8_t getI2CAddress(void)
{
    return SSP1ADD;
}

void _setI2CAddress(void)
{
    
#ifdef ENABLE_ADDR_LINES
    SSP1ADD = _I2C_BASE_ADDR_VALUE | (I2C_ADDR_PIN0 << 1) | (I2C_ADDR_PIN1 << 2) 
            | (I2C_ADDR_PIN2 << 3);
#else
    SSP1ADD = _I2C_BASE_ADDR_VALUE;
#endif
    
}

void updateI2CAddress(void)
{
    SSP1CON1bits.SSPEN = 0;
    _setI2CAddress();
    SSP1CON1bits.SSPEN = 1;
}