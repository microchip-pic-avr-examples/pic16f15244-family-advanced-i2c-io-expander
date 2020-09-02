/** 
 * \active_keypad.ino
 *
 * This sketch uses the PIC16F15244 family parts running the "Advanced I2C I/O Expander" to
 * implement an active keypad scanner (polling based).
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
volatile bool readPins = false;
uint8_t column = 4;
char keys[4];

const char buttons[4][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};

#define WAIT_TIME 1
#define COL0_OFFSET 4

void printField(const char* str, uint8_t field)
{
    Serial.print(str);
    Serial.print(": 0x");
    Serial.println(field, HEX);
    Serial.print("\n");
}

/*
 * This function scans a column (col) by pulling it from a logic '1' to a
 * logic '0'. 
 */
uint8_t scanColumn(uint8_t col)
{
    //If not a valid column, then return 0
    if (col > 2)
      return 0;
      
    uint8_t count = 0;

    //Turn off the column
    device.setPinToOutput(col + COL0_OFFSET);

    //Wait for a moment (give time for capacitors to discharge)
    delay(WAIT_TIME);

    uint8_t portStatus = device.getPORT();

    if ((portStatus & 0x0F) != 0x0F)
    {
        //1 or more keys is pressed
        uint8_t mask = 0x01;
        uint8_t index = 0;

        //Invert the logic on portStatus for scanning
        portStatus = ~portStatus;
        
        //Scan all key rows
        while (index != 4)
        {
            if ((portStatus & mask) != 0x00)
            {
                //Key was pressed!
                keys[count] = buttons[index][col];
                count++;
            }
            index++;
            mask = mask << 1;
        }
    }

    //All columns back on    
    device.setPinToInput(col + COL0_OFFSET);
    
    return count;
}

void setup() {
  Serial.begin(9600);
  
  // -- Arduino Setup --
  //Set up INT line
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), interrupt, FALLING);

  // -- I/O Expander Setup --

  //Load Default Compiled Settings
  //Will set off an interrupt
  device.loadDefaults(true);

  //Set pins 6:0 to inputs, 7 to output
  device.setField(TRISx, 0x7F);

  //Setup Weak Pull-Ups on Inputs
  device.setField(WPUx, 0x7F);

  //Turn off all latches
  device.setField(LATx, 0x00);
  column = 0;

  //Enable Schmitt Trigger CMOS Levels
  device.setField(INLVLx, 0x0F);

  //No Interrupts
  device.setField(IOCxN, 0x00);
  device.setField(IOCxP, 0x00);

  //Clear readpins
  readPins = false;
}

void loop() {
  uint8_t keyCount = scanColumn(column);

  //If a key was found...
  if (keyCount > 0)
  {
    uint8_t counter = 0;
    do
    {
      Serial.print("Key ");
      Serial.print(keys[counter]);
      Serial.print(" is pressed.\n");
      counter++;
    } while (counter != keyCount);
  }

  //Move to the next column to scan
  column += 1;
  if (column == 4)
  {
    column = 0;
  }
  
  delay(20);
}

void interrupt()
{
  device.onInterrupt();

  //For this example, readpins is not needed
  readPins = true;
}
