/*
 * advanced_io.h
 *
 * This library is used to establish communication with a PIC16F15244 family device that is running
 * the "Advanced I2C Code Example", available on Github at the link below:

 * https://github.com/microchip-pic-avr-examples/pic16f1524x-advanced-i2c-io-expander.git

 * The most up-to date version of this library and sample code may be obtained from this link as well.
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

#ifndef ADVANCED_IO_H
#define ADVANCED_IO_H

#include <stdint.h>

//This enumeration contains all of the possible error codes the device can output, plus a few that are associated
//with an incorrect operation in this library (these start with ARDUINO).
enum PIC16F152x_ERRORS{
	ERROR_NONE = 0x00, ERROR_READ_OVERRUN, ERROR_WRITE_OVERRUN, ERROR_INVALID_READ_OP,
	ERROR_INVALID_WRITE_OP, ERROR_UNUSED, ERROR_INVALID_ACCESS, ERROR_ILLEGAL_MEM_OP, ERROR_MEM_OP_ABORTED,
	ERROR_MEM_INVALID_SEQ, ERROR_CRC_FAILED, ERROR_WRITE_VERIFY, ARDUINO_ERROR_INVALID_OP = 0xFF
	};

/*
This enumeration contains all of the bitfields that are Read and Write in the Code Example.
For full explanations of each, please consult the device datasheet for the PIC16F524x.

Note: For all registers, the "x" refers to the PORT, which is assigned by the developer.
For the Advanced I2C I/O Expander, PORTC is used. Thus, a value (such as TRISx or IOCxP) is
actually referring to TRISC and IOCCP.

Descriptions
TRIS	= Direction Control. If set to 1, then the pin is an INPUT. If set to 0, the pin is an OUTPUT.
LAT		= Output Latch. This is the logic level the device outputs. If the pin is an INPUT,
		then this value is assigned, but unused.
IOCxP	= Interrupt on Change rising edges. If this bit is set to 1, then a rising edge (1 -> 0) on the associated pin will
		assert the INT line, and sets the related bit in the virtualized IOC register.
IOCxN	= Interrupt on Change falling edges. If this bit is set to 1, then a falling edge (1 -> 0) on the associated pin will
		assert the INT line, and sets the related bit in the virtualized IOC register.
WPU		= Weak Pull-Up Control. If this bit is set to 1, then weak pull-ups are used on the associated pin. If the pin is an output,
		the weak pull-ups are disabled.
INLVL	= Input Threshold Control. If this bit is set, then the threshold for the associated pin is set to schmitt-trigger CMOS,
		rather than TTL. See the device datasheet for more information.
SLRCON	= Slew Rate Control. If this bit is set, then the slew rate of the associated pin is limited, which may be beneficial
		for EMI reasons. If this bit is clear, then the associated pin switches as fast as possible.
ODCON	= Open-Drain Control. If this bit is set, the high-side output driver for the pin is disabled, meaning the associated pin
		can only sink current. If this bit is clear, the pin becomes a PUSH-PULL output.
*/
enum PIC16F1524x_IO_FIELDS{
  TRISx = 0x03, LATx, IOCxP, IOCxN, WPUx = 0x08, INLVLx, ODCONx, SLRCONx
} ;

/*
This enumeration contains the possible override settings to apply to the I/O during a configuration LOAD.

NO_CHANGE	= Does not override the current settings on the I/O line.
OUTPUT_LOW	= Forces all pins to be output low.
OUTPUT_HIGH	= Forces all pins to be output high.
TRISTATED	= Forces all pins to be tristated, with weak-pull-ups off.
*/
enum PIC16F1524x_PIN_OVERRIDE{
	NO_CHANGE = 0x00, OUTPUT_LOW, OUTPUT_HIGH, TRISTATED
	};

/*
This class instantiates a driver which enables easy communication with a PIC16F1524x,
that is running the "advanced" I2C I/O Expander Code Example (8-bit). This class
starts the I2C driver for Arduino.

To download the full repository (which includes the most up-to-date version of this library),
and which includes the code required for the PIC16F5244 family devices (specifically 20-pin versions,
eg: PIC16F15243, PIC16F15244, PIC16F15245).
*/
class PIC16F1524x_IO
{
public:

	//Starts an instance of this class, inits I2C, and uses the address of 0x60 for I2C communications.
	PIC16F1524x_IO();

	//Starts an instance of this class, inits I2C, and uses a custom I2C address (i2cAddr).
	PIC16F1524x_IO(uint8_t i2cAddr);

	//This function changes the I2C Address the library uses to communicate.
	void setI2CAddress(uint8_t nAddr);

	//Call this function when this device asserts the INT line.
	//Releases the "isBusy" flag if the device was in a memory OP.
	void onInterrupt();

	//This function returns the virtual register "ERROR" on the device.
	//If this register is non-zero, then an ERROR has occurred.
	PIC16F152x_ERRORS getError();

	//This function returns a bit map of any pins which have experienced an "IOC" event.
	//Internally, this returns the virtual register "IOCx".
	uint8_t getIOC();

	//This function returns a bitmap of all the the logic levels at the PORT.
	//Pins that are outputs (and not shorted to the opposite logic level) should return their output.
	uint8_t getPORT();

	//GET or SET one of the read/write fields
	uint8_t getField(PIC16F1524x_IO_FIELDS field);
	void setField(PIC16F1524x_IO_FIELDS field, uint8_t data);

	//Sets the pin direction register (0 = Output, 1 = input) sets the output driver level (if enabled)
	void setDirectionAndOutput(uint8_t pins, uint8_t value);

	//Sets the Interrupt-On-Change (IOC) for the pins in each bitmap
	void setIOC(uint8_t risingEdges, uint8_t fallingEdges);

	//Toggles the output of the selected pins
	void togglePins(uint8_t pins);

	//Reads a single pin in the I/O Expander. (Returns the PORT value of this pin)
	//PIN must be less than 8 to execute.
	bool digitalRead(uint8_t pin);

	//If the pin is an output, this function sets the output level to value.
	//If this pin is an input, the weak pull-up internally is set according to value.
	//PIN must be less than 8 to execute.
	void digitalWrite(uint8_t pin, bool value);

	//This function sets the pins in the I/O expander to be an output if the associated bit in (pins) is set.
	//Does not set the output value.
	void setPinsAsOutput(uint8_t pins);

	//This function sets the pins in the I/O expander to be an input if the associated bit in (pin) is set.
	void setPinsAsInput(uint8_t pins);

	//This function sets a pin (at pinNum) to an output.
	//Does not set the output value.
	void setPinToOutput(uint8_t pinNum);

	//This function sets a pin (at pinNum) to an input.
	void setPinToInput(uint8_t pinNum);

	/*
	This function causes the device to load the default settings from compile time (DEFAULT_x) in config.h

	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS loadDefaults(bool blocking = true);

	/*
	This function causes the device to save the current I/O settings to one of the available CONFIGs.
	CONFIG must be equal to or between 0 and 3 for this function to execute.

	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS saveConfiguration(uint8_t config, bool blocking = true);

	/*
	This function causes the device to load the new I/O settings to one of the available CONFIGs.
	CONFIG must be equal to or between 0 and 3 for this function to execute.

	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS loadConfiguration(uint8_t config, bool blocking = true);

	/*
	This function causes the device to load the new I/O settings to one of the available CONFIGs.
	CONFIG must be equal to or between 0 and 3 for this function to execute.
	PINOVERRIDE defines the output of all pins while the LOAD occurs.

	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS loadConfiguration(uint8_t config, PIC16F1524x_PIN_OVERRIDE pinOverride, bool blocking = true);

	/*
	This function causes the device to save the current I/O settings to saveConfig and to load new ones from
	loadConfig.

	LOADCONFIG must be equal to or between 0 and 3 for this function to execute.
	SAVECONFIG must be equal to or between 0 and 3 for this function to execute.
=
	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS saveAndLoadConfiguration(uint8_t saveConfig, uint8_t loadConfig, bool blocking = true);

	/*
	This function causes the device to save the current I/O settings to saveConfig and to load new ones from
	loadConfig.

	LOADCONFIG must be equal to or between 0 and 3 for this function to execute.
	SAVECONFIG must be equal to or between 0 and 3 for this function to execute.
	PINOVERRIDE defines the output of all pins while the LOAD occurs.

	Blocking causes the function to wait until the operation has completed, where it returns the STATUS
	register. If blocking is not enabled, then the function returns 0x00.
	*/
	PIC16F152x_ERRORS saveAndLoadConfiguration(uint8_t saveConfig, uint8_t loadConfig,
	PIC16F1524x_PIN_OVERRIDE pinOverride, bool blocking = true);
protected:
	//Internal Function - Returns a value read at the address (field)
	uint8_t _getField(uint8_t field);

	//Internal Function - Sets the address to field, then writes data.
	void _setField(uint8_t field, uint8_t data);

	//Internal Function - returns a single bit from the field at pin
	bool _getFieldBit(uint8_t field, uint8_t pin);

	//Internal Function - sets a single bit in the field at bit [pin] to value
	void _setFieldBit(uint8_t field, uint8_t pin, bool value);

	//Internal Function - flips a single bit in the field at bit [pin]
	void _toggleFieldBit(uint8_t field, uint8_t pin);
private:
	//This value keep track if the device is currently running a memory OP.
	volatile bool isBusy;

	//I2C Address the device is at.
	uint8_t addr;
};

#endif
