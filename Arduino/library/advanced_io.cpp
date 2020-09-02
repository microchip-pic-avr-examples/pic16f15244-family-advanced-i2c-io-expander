/*
 * advanced_io.cpp
 *
 * Arduino Library to interface with PIC16F15244 family devices running the
 * "Advanced I2C I/O Expander Code Example" (pic16f15244-family-advanced-io-expander)
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

#include "advanced_io.h"
#include "Wire.h"

#include <stdint.h>
#include "Arduino.h"

PIC16F1524x_IO::PIC16F1524x_IO()
{
	//Code Example Default
	addr = 0x60;
	isBusy = false;
	Wire.begin();
}

PIC16F1524x_IO::PIC16F1524x_IO(uint8_t i2cAddr)
{
	addr = i2cAddr;
	isBusy = false;
	Wire.begin();
}

void PIC16F1524x_IO::setI2CAddress(uint8_t nAddr)
{
	addr = nAddr;
}

void PIC16F1524x_IO::onInterrupt()
{
	isBusy = false;
}

PIC16F152x_ERRORS PIC16F1524x_IO::getError()
{
	return (PIC16F152x_ERRORS)_getField(0x00);
}

uint8_t PIC16F1524x_IO::getIOC()
{
	return _getField(0x01);
}

uint8_t PIC16F1524x_IO::getPORT()
{
	return _getField(0x02);
}

uint8_t PIC16F1524x_IO::getField(PIC16F1524x_IO_FIELDS field)
{
	return _getField(field);
}

void PIC16F1524x_IO::setField(PIC16F1524x_IO_FIELDS field, uint8_t data)
{
	_setField((uint8_t)field, data);
}

void PIC16F1524x_IO::setDirectionAndOutput(uint8_t pins, uint8_t value)
{
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(TRISx);
	Wire.write(pins);	//For TRIS, 1 = input, 0 = output
	Wire.write(value);
	Wire.endTransmission();

}

void PIC16F1524x_IO::setIOC(uint8_t risingEdges, uint8_t fallingEdges)
{
	while (isBusy) { ; }
	Wire.beginTransmission(IOCxP);
	Wire.write(risingEdges);	
	Wire.write(fallingEdges);
	Wire.endTransmission();
}

void PIC16F1524x_IO::togglePins(uint8_t pins)
{
	uint8_t field = _getField(LATx);
	field ^= pins;
	_setField(LATx, field);
}

bool PIC16F1524x_IO::digitalRead(uint8_t pin)
{
	if (pin >= 8)
		return false;
		
		//0x02 = PORTx location
	return _getFieldBit(0x02, pin);
}

void PIC16F1524x_IO::digitalWrite(uint8_t pin, bool value)
{
	if (pin >= 8)
		return;
		 
	bool isInput = _getFieldBit(TRISx, pin);
	if (isInput)
	{
		//Input Pin = Control Weak Pull-Ups
		_setFieldBit(WPUx, pin, value);
	}
	else
	{
		//Output = Set Output Value
		_setFieldBit(LATx, pin, value);
	}
}
	
void PIC16F1524x_IO::setPinsAsOutput(uint8_t pins)
{
	uint8_t field = _getField(TRISx);
	field &= (~pins);
	_setField(TRISx, field);
}

void PIC16F1524x_IO::setPinsAsInput(uint8_t pins)
{
	uint8_t field = _getField(TRISx);
	field |= pins;
	_setField(TRISx, field);
}

void PIC16F1524x_IO::setPinToOutput(uint8_t pinNum)
{
	uint8_t field = _getField(TRISx);
	field &= ~(0x01 << pinNum);
	_setField(TRISx, field);
}
	
void PIC16F1524x_IO::setPinToInput(uint8_t pinNum)
{
	uint8_t field = _getField(TRISx);
	field |= (0x01 << pinNum);
	_setField(TRISx, field);
}

PIC16F152x_ERRORS PIC16F1524x_IO::loadDefaults(bool blocking)
{
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(0x00);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}

PIC16F152x_ERRORS PIC16F1524x_IO::saveConfiguration(uint8_t config, bool blocking)
{
	if (config > 3)
	{
		return ARDUINO_ERROR_INVALID_OP;
	}
	uint8_t op = 0x10 | (config << 2);
	
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(op);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}

PIC16F152x_ERRORS PIC16F1524x_IO::loadConfiguration(uint8_t config, bool blocking)
{
	if (config > 3)
	{
		return ARDUINO_ERROR_INVALID_OP;
	}
	uint8_t op = 0x20 | config;
	
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(op);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}
PIC16F152x_ERRORS PIC16F1524x_IO::loadConfiguration(uint8_t config, PIC16F1524x_PIN_OVERRIDE pinOverride, bool blocking)
{
	if ((config > 3) || (pinOverride > 3))
	{
		return ARDUINO_ERROR_INVALID_OP;
	}
	uint8_t op = 0x20 | ((uint8_t)pinOverride << 6) | config;
	
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(op);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}

PIC16F152x_ERRORS PIC16F1524x_IO::saveAndLoadConfiguration(uint8_t saveConfig, uint8_t loadConfig, bool blocking)
{
	if ((saveConfig > 3) || (loadConfig > 3))
	{
		return ARDUINO_ERROR_INVALID_OP;
	}
	uint8_t op = 0x30 | (saveConfig << 2) | loadConfig;
	
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(op);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}

PIC16F152x_ERRORS PIC16F1524x_IO::saveAndLoadConfiguration(uint8_t saveConfig, uint8_t loadConfig, 
PIC16F1524x_PIN_OVERRIDE pinOverride, bool blocking)
{
	if ((saveConfig > 3) || (loadConfig > 3) || (pinOverride > 3) )
	{
		return ARDUINO_ERROR_INVALID_OP;
	}
	uint8_t op = 0x30 | ((uint8_t)pinOverride << 6) | (saveConfig << 2) | loadConfig;
	
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(0xA0);
	Wire.write(op);
	Wire.write(0xA5);
	Wire.write(0xF0);
	Wire.endTransmission();
	isBusy = true;
	if (blocking)
	{
		while (isBusy) { ; }
		return getError();
	}
	return ERROR_NONE;
}

uint8_t PIC16F1524x_IO::_getField(uint8_t field)
{
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(field);
	Wire.endTransmission();
	
	Wire.requestFrom(addr, 1);
	
	//If data was read, then return it
	if (Wire.available())
		return Wire.read();
	
	return 0x00;
	
}


void PIC16F1524x_IO::_setField(uint8_t field, uint8_t data)
{
	while (isBusy) { ; }
	Wire.beginTransmission(addr);
	Wire.write(field);
	Wire.write(data);
	Wire.endTransmission();
}

bool PIC16F1524x_IO::_getFieldBit(uint8_t field, uint8_t pin)
{
	return (bool) (( _getField(field) >> pin) & 0x01);
	
}

void PIC16F1524x_IO::_setFieldBit(uint8_t field, uint8_t pin, bool value)
{
	uint8_t n_value = _getField(field);
	uint8_t pinMask = 0x01 << pin;
	
	if (value)
	{
		//Set bit to 1
		n_value |= pinMask;
	}
	else
	{
		//Set bit to 0
		n_value &= (~pinMask);
	}
	
	_setField(field, n_value);
}

void PIC16F1524x_IO::_toggleFieldBit(uint8_t field, uint8_t pin)
{
	uint8_t n_value = _getField(field);
	uint8_t pinMask = 0x01 << pin;
	
	n_value ^= pinMask;
	
	_setField(field, n_value);
}