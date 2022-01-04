#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

/* 
 * Start of config memory row.
 * Defines are in the compiler settings
 * */
    
#ifdef PIC16F152_45
#define MEM_START 0x1F80
#elif PIC16F152_44
#define MEM_START 0xF80
#elif PIC16F152_43
#define MEM_START 0x780
#else
#error NO DEVICE SPECIFIED FOR MEM START     
#endif

    
//I2C Parameters
    
#define I2C_BASE_ADDRESS 0x60
#define _I2C_BASE_ADDR_VALUE (I2C_BASE_ADDRESS << 1)

//Comment out this define to disable TMR2 as an I2C Timeout
#define I2C_TIMEOUT_EN
    
//Comment out this define to disable address select lines for I2C    
#define ENABLE_ADDR_LINES
    
#ifdef ENABLE_ADDR_LINES
    
#define I2C_ADDR_PIN0_ANSEL     ANSELAbits.ANSA0
#define I2C_ADDR_PIN1_ANSEL     ANSELAbits.ANSA1
#define I2C_ADDR_PIN2_ANSEL     ANSELAbits.ANSA2
    
#define I2C_ADDR_PIN0_DIR       TRISAbits.TRISA0
#define I2C_ADDR_PIN1_DIR       TRISAbits.TRISA1
#define I2C_ADDR_PIN2_DIR       TRISAbits.TRISA2
    
#define I2C_ADDR_PIN0           PORTAbits.RA0
#define I2C_ADDR_PIN1           PORTAbits.RA1
#define I2C_ADDR_PIN2           PORTAbits.RA2
#endif

//Controlled PORT
    
//PORT 1
#define PORTx           PORTC
#define LATx            LATC
#define TRISx           TRISC
#define ANSELx          ANSELC
#define ODCONx          ODCONC
#define SLRCONx         SLRCONC
#define INLVLx          INLVLC
#define WPUx            WPUC
    
//PORT 1 IOC Parameters
#define IOCxF           IOCCF
#define IOCxP           IOCCP
#define IOCxN           IOCCN

//Set the value below to load config 0 on startup, rather than defaults.
//NOTE: IF THE CONFIG FAILS CRC, THE DEFAULTS WILL BE LOADED!
#define CONFIG_ON_BOOT
    
//Default Settings for PORT 1
#define DEFAULT_TRISx       0xFF
#define DEFAULT_LATx        0x00
#define DEFAULT_WPUx        0xFF
#define DEFAULT_INLVLx      0x00
#define DEFAULT_SLRCONx     0xFF
#define DEFAULT_ODCONx      0x00
    
//Default IOC settings
//May trigger interrupt due to PORT capacitance and charge time delays on startup
#define DEFAULT_IOCxP       0x00
#define DEFAULT_IOCxN       0x00
    
//INT Pin Settings
    
//Comment out this line to disable OPEN DRAIN for INT PIN
#define ENABLE_OPEN_DRAIN
    
#define INT_LAT     LATA4
#define INT_TRIS    TRISA4
#define INT_ODCON   ODCA4


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIGURATION_H */

