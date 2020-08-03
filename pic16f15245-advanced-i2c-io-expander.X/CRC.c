#include "CRC.h"

#include <xc.h>

static volatile uint16_t CRCBuffer;
static volatile uint8_t polynomial;

void initCRC(uint8_t poly)
{
    CRCBuffer = 0x0000;
    polynomial = poly; 
}

void clearCRC(void)
{
    CRCBuffer = 0x0000;
}

void feedNumber(uint8_t input)
{
    uint16_t poly = 0x8000 | (polynomial << 7);
    uint8_t counter = 15;
    //Apply remainder to input
    CRCBuffer = ((input ^ (CRCBuffer & 0xFF)) << 8);
    
    while ((CRCBuffer >> 8) != 0x00)
    {
        if (CRCBuffer >> counter != 0)
        {
            //1 in the 1st position
            CRCBuffer ^= poly;
        }
        
        //Shift Right by 1
        poly = poly >> 1;
        counter--;
    }
}

uint8_t getRemainder(void)
{
    return CRCBuffer & 0xFF;
}