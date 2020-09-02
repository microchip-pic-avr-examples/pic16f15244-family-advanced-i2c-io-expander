/** 
 * \123_gettingStarted.ino
 *
 * This sketch uses the PIC16F15244 family parts running the "Advanced I2C I/O Expander" to
 * display "1 2 3" on the 7 segment display.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

#include <Arduino.h>
#include <advanced_io.h>

PIC16F1524x_IO device(0x60);
uint8_t counter = 1;
bool blinky = false;

//Hex Values that corrospond to the segments that are turned on
const uint8_t digitsActiveLow[10] =   {0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001, 0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000};
const uint8_t digitsActiveHigh[10] =  {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111};

/*
 * This function sets a 7 segment display + 1 Decimal Point (DP)
 * to show the specified number in digit, assuming it is between
 * 0 and 9.
 * 
 * Designed to be used with Active LOW (Common Anode)
 * LED Displays.
 */
void set7SegmentActiveLow(uint8_t digit, bool decimal)
{
  uint8_t LED_out = 0x00;
  if (digit >= 10)
  {
      //Error Character = E
      LED_out = 0b0000110;
  }
  else
  {
      LED_out = (digitsActiveLow[digit]);  
  }
  
  LED_out |= ((!decimal) << 7);
  device.setField(LATx, LED_out);
}

/*
 * This function sets a 7 segment display + 1 Decimal Point (DP)
 * to show the specified number in digit, assuming it is between
 * 0 and 9.
 * 
 * Designed to be used with Active HIGH (Common Cathode)
 * LED Displays.
 */
void set7SegmentActiveHigh(uint8_t digit, bool decimal)
{
  uint8_t LED_out = 0x00;
  if (digit >= 10)
  {
    //Error Character = E
      LED_out = 0b1111001;
  }
  else
  {
      LED_out = (digitsActiveHigh[digit]);    
  }
  
  LED_out |= (decimal << 7);

  device.setField(LATx, LED_out);
}


void setup() {

  // -- I/O Expander Setup --
  
  //Set all I/O pins to be outputs
  device.setField(TRISx, 0x00);

  //Clear the latches (if ACTIVE LOW, then set this to 0xFF instead)
  device.setField(LATx, 0x00);

  //Disable all interrupt on change events
  device.setField(IOCxN, 0x00);
  device.setField(IOCxP, 0x00);

  //Disable Open Drain Mode (if it was set)
  device.setField(ODCONx, 0x00);
}

void loop() {

  //Set the output to the next number (counter)
  set7SegmentActiveLow(counter, blinky);
  counter += 1;

  blinky = !blinky;
  if (counter == 4)
    counter = 1;

  //Wait 1 second
  delay(1000);
}
