
// PIC16F15245 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_32MHZ// Power-up Default Value for COSC bits (HFINTOSC (32 MHz))
#pragma config CLKOUTEN = ON   // Clock Out Enable bit (CLKOUT function is disabled; I/O function on RA4)
#pragma config VDDAR = HI       // VDD Range Analog Calibration Selection bit (Internal analog systems are calibrated for operation between VDD = 2.3V - 5.5V)

// CONFIG2
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR)
#pragma config PWRTS = PWRT_OFF // Power-up Timer Selection bits (PWRT is disabled)
#pragma config WDTE = SWDTEN    // WDT Operating Mode bits (WDT enabled/disabled by SEN bit)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection bit (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config PPS1WAY = ON     // PPSLOCKED One-Way Set Enable bit (The PPSLOCKED bit can be set once after an unlocking sequence is executed; once PPSLOCKED is set, all future changes to PPS registers are prevented)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block is disabled)
#pragma config SAFEN = ON       // SAF Enable bit (SAF is enabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block is not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block is not write-protected)
#pragma config WRTC = OFF       // Configuration Registers Write Protection bit (Configuration Registers are not write-protected)
#pragma config WRTSAF = OFF     // Storage Area Flash (SAF) Write Protection bit (SAF is not write-protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored.)

// CONFIG5
#pragma config CP = OFF         // User Program Flash Memory Code Protection bit (User Program Flash Memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include "i2c_fsm.h"
#include "i2c_slave.h"
#include "io_control.h"
#include "interrupt.h"
#include "status.h"
#include "memory_control.h"
#include "CRC.h"
#include "utility.h"

void main(void) {
    
    //Run at 4MHz for 100kHz I2C
    SET_OSC_FREQ(0b010);

    //Init Device Status + Error Registers
    initStatus();
    
    //Configure the CRC for validating the stored configurations
    initCRC(0x07);
    
    //Init Memory Control
    initMemoryControl();
    
    //Init IO for Port Expansion
    initIO();
    
    //Init I2C for Communication
    initI2C();
    
    //Init FSM
    init_I2C_FSM();
    
    //Enable Interrupt on Change
    enableIOCinterrupt();
        
    //Enable Interrupts
    enableInterrupts();
                
    /*When the 1st I2C activity occurs, it will enter while(1). When the STOP
     event occurs in I2C, the STOP bit will be set, which will enable power
     saving sleep all the time. */
    
    SLEEP();
    __asm("NOP");
    
    while (1)
    {
        //I2C Idle
        
        if (isActive_n())
        {
            if (isPendingMemoryOP())
            {
                //If a memory operation is pending, then execute here.
                //Will disable all interrupts. Asserts INT when completed

                runMemoryOP();
            }
            
            //Sleep
            SLEEP();
            __asm("NOP");        
        }
        
    }    
    return;
}
