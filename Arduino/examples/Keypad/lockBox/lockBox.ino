/** 
 * \lockbox.ino
 *
 * This sketch uses the PIC16F15244 family parts running the "Advanced I2C I/O Expander" to
 * implement a reprogrammable keypad with a "lock" LED.
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

const char buttons[4][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};

//Max number of digits allowed to be entered
const uint8_t KEY_SIZE = 10;

//Buffer to hold keycode
char keyCode[KEY_SIZE + 1];

//Enumeration to hold states for the keypad
enum LockboxStateMachine{
  LOCKED = 0, UNLOCKED, NEW_CODE
};

//Current state of the lock
LockboxStateMachine lockState;

//Defines how long to allow a line to recharge before continuing
#define WAIT_TIME 1

//Serial.print wrapper for debugging 
void printField(const char* str, uint8_t field)
{
    Serial.print(str);
    Serial.print(": 0x");
    Serial.println(field, HEX);
    Serial.print("\n");
}

//Returns a number between 0 and 7 if a bit is set, otherwise returns 0xFF.
uint8_t bitSearch(uint8_t search)
{
  uint8_t mask = 0x01;
  uint8_t hit = 0xFF;
  uint8_t counter = 0;
  
  do
  {
    if ((search & mask) != 0x00)
    {
        //The current bit is set in search
        
        if (hit == 0xFF)
        {
            //No hits found yet, log the position
            hit = counter;
        }
        else
        {
            //Already found a hit in this word
            return 0xFF;
        }
    }
    mask = mask << 1;
    counter++;
  } while (counter != 8);
  
  return hit;
}

//Returns a character from buttons[][] if a match is found.
//Otherwise returns 0x00 ('\0') on an error
char pinSearch()
{
    uint8_t pinState = device.getPORT();
    uint8_t event = bitSearch(~(pinState | 0xF0));
    uint8_t index = 0;
    char c = '\0';

    //Button has already been released, 2+ lines are down, or an error occured in bitSearch
    //No way to figure out which button was pressed
    if ((pinState == 0x0F) || (event > 4))
    {
        return '\0';
    }

    //1st row search - set it to logic 1
    
    for (uint8_t t_pin = 4; t_pin < 7; t_pin++)
    {
      //To figure out which pin, temporarily set each line an input,
      //and use the internal weak pull-ups to go to a logic level of '1'
      device.setPinToInput(t_pin);

      //WAIT_TIME may need to be changed if there is too much capacitance on the I/O line
      delay(WAIT_TIME);

      uint8_t t_state = device.getPORT();

      //Mask the LAT pins
      if ((0x0F & t_state) != (0x0F & pinState))
      {
          //We found a changed pin!
          if (c != '\0')
          {
              //Two keys were down - can't distinguish
              t_pin = 8;
              c = '\0';
          }
          else
          {
              //Assign the detected button to c
              c = buttons[event][t_pin - 4];

              //Update the pin state
              pinState = t_state;
          }
      }
    }

    //Revert the device to normal settings
    device.setPinsAsOutput(0x70);

    //Just a little bit of time to allow the lines to recharge
    delay(WAIT_TIME);
    
    //If an interrupt ocurs, clear it
    readPins = false;
    return c;
}

/*
 * Finite State Machine which accepts the entered key
 * and uses it to determine if the correct opening
 * sequence has been activated.
*/
void secureCodeDetector(char key)
{
    static uint8_t place = 0;
    //Blink the light to show a key was pressed
    device.togglePins(0x80);
    delay(10);
    device.togglePins(0x80);
    delay(10);
  
    switch(lockState)
    {

      //Locked State
      case LOCKED:

        //Incorrect key or '*' was pressed - reset to 0
        if ((key == '*') || (keyCode[place] != key))
        {
            place = 0;
        }
        else if (keyCode[place] == key)
        {
            //Key matches sequence, increment the counter
            place++;           
        }

        //If the next key is 0x00 ('\0'), then the sequence is complete
        if (keyCode[place] == 0x00)
        {
            //"Unlock" the Box
            lockState = UNLOCKED;
            device.digitalWrite(7, 1);
            place = 0;
        }   
        break;
      case UNLOCKED:

        //If '*' is pressed, then "lock" the box
        if (key == '*')
        {
          lockState = LOCKED;
          device.digitalWrite(7, 0);
        }
        else if (key == '#')
        {
          //Enter new code sequence
          lockState = NEW_CODE;
  
          //Clear old combination
          for (uint8_t i = 0; i < KEY_SIZE; ++i)
          {
              keyCode[i] = 0x00;
          }
  
          //Turn the lock LED off to indicate the mode
          device.digitalWrite(7, 0);
        }
        break;
      case NEW_CODE:

        device.togglePins(0x80);
      
        //'*' is a special character - do not add it to the keyCode
        if (key != '*')
        {
          keyCode[place] = key;
          place++;
        }
  
        //If '*' is pressed, or out of memory, exit this mode.
        if ((key == '*') || ((place == KEY_SIZE)))
        {
            lockState = LOCKED;
            device.digitalWrite(7, 0);
            place = 0;
            
        }
        break;
    }
}

void setup() {
  Serial.begin(9600);

  //Create default keyCode
  //Default: 15244#
  for (int i = 0; i < (KEY_SIZE + 1); ++i)
  {
      if (i == 0)
      {
        keyCode[i] = '1';
      }
      else if (i == 1)
      {
        keyCode[i] = '5';
      }
      else if (i == 2)
      {
        keyCode[i] = '2';
      }
      else if ((i == 3) || (i == 4))
      {
        keyCode[i] = '4';
      }
      else if (i == 5)
      {
        keyCode[i] = '#';
      }
      else
      {
        keyCode[i] = 0x00;
      }
  }
  
  // -- Arduino Setup --
  //Set up INT line
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), interrupt, FALLING);

  // -- I/O Expander Setup --

  //Load Default Compiled Settings
  //Will set off an interrupt
  device.loadDefaults(true);

  //Set pins 6:0 to outputs, 7 to input
  device.setField(TRISx, 0x0F);

  //Setup Weak Pull-Ups on Inputs
  device.setField(WPUx, 0x7F);

  //Clear Latches
  device.setField(LATx, 0x00);

  //Enable Schmitt Trigger CMOS Levels
  device.setField(INLVLx, 0x0F);

  //Setup a negative edge Interrupt
  device.setField(IOCxN, 0x0F);

  //No positive edge interrupts
  device.setField(IOCxP, 0x00);

  //Clear the counter and disable readpins
  readPins = false;
}

void loop() {

  if (readPins)
  {
    char key = pinSearch();
    if (key != '\0')
    {
        //Check to see if the key is apart of the key sequence
        secureCodeDetector(key);

        //Debugging Statement
        
        /*Serial.print("Key ");
        Serial.print(key);
        Serial.print(" was pressed.\n");*/

        //Read the field again due to the extra edge of the IOCxN line
        device.getIOC();
    }

    //Clear the flag
    readPins = false;
  }
  delay(10);
}

//On a keypress or a "load defaults"
void interrupt()
{
  device.onInterrupt();
  readPins = true;
}
