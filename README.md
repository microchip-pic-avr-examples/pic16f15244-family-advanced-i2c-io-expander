<!-- Please do not change this html logo with link -->
<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# Advanced I<sup>2</sup>C 8-Bit I/O Expander with PIC16F15245
One of the biggest benefits of I<sup>2</sup>C is the simple wiring required to create a full-featured serial bus. This code example leverages this fact to create an advanced 8-bit I/O extender using the MSSP module, along with powerful other features such as Interrupt on Change (IOC) and the Program Flash Memory (PFM) to store and switch between different I/O configurations.

## Related Documentation

* <a href="https://www.microchip.com/wwwproducts/en/PIC16F15245">PIC16F15245 Documentation</a>
* Code Example: Simple I/O Expander with the PIC16F15245

## Software Used

* <a href="http://www.microchip.com/mplab/mplab-x-ide">MPLAB® IDE 5.40 or newer</a>
* <a href="https://www.microchip.com/mplab/compilers">Microchip XC8 Compiler 2.20 or newer</a>
* PIC16F1xxxx_DFP v1.4.119

## Hardware Used

### Common Components

* <a href="https://www.microchip.com/developmenttools/ProductDetails/PartNO/ADM00559"> I<sup>2</sup>C Master Device (or for easy testing, an MCP2221A USB-UART/I<sup>2</sup>C breakout module, PN: ADM00559)</a>

### With the PIC16F15245 on a Curiosity LPC Board

* <a href="https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164137"> Microchip Low Pin Count Curiosity, PN: DM164137</a><br><br>

**Important:**
1. RC0 is connected to a potentiometer on RC0. This may cause the pin to be stuck at Vdd/Vss.<br>
2. RC1 is connected to the mTouch(TM) button. It is possible to couple enough charge through this button to trigger the IOC circuit on this pin.

### With the Curiosity Nano (PIC16F15244)

* <a href="#"> Microchip Curiosity Nano, PN: ???</a>

**Important setup instructions:**
1. Change the build configuration to the appropriate configuration ending with PIC16F15244.<br>

<img src="images/configurationChange.PNG"><br>

2. In config.h, change the macro `#define MEM_START 0x1F80` to `#define MEM_START 0xF80`.
3. Run Build and Clean on the project. (Hammer + Broom on the toolbar).  

## Table of Contents

* [Setup](#setup)
  * [Wiring](#wiring)
  * [Default I<sup>2</sup>C Settings](#default-i2c-settings)
* [Operation](#operation)
  * [Startup](#startup)
* [I<sup>2</sup>C Configuration](#i2c-configuration)
* [I<sup>2</sup>C Communication](#i2c-communication)
  * [Writing to the Device](#writing-to-the-device)
  * [Reading from the Device](#reading-from-the-device)
  * [Command Ordering and Permissions](#command-ordering-and-permissions)
* [Memory Operations](#memory-operations)
    * [Setting up Memory Operations](#setting-up-memory-operations)
    * [Memory Operation Byte](#memory-operation-byte)
    * [Memory Commands](#memory-commands)
      * [Load Default](#load-default)
      * [Save Configuration](#save-configuration)
      * [Load Configuration](#load-configuration)
      * [Save and Load Configuration](#save-and-load-configuration)
    * [Memory Storage](#memory-storage)
* [Error Handling](#error-handling)
* [Summary](#summary)

<br>

## Setup

### Wiring
<img src="images/schematic.png" width="500px"><br>
*Figure 1 - Electrical Configuration of device. Shown with Address Lines tied to GND.*<br>

On the PIC16-152, the default positions for the pins and ports are:

| Function  | Pin or PORT
| --------- | ----
| SDA       | RB4
| SCL       | RB6
| INT       | RB5
| ADDR0     | RA0
| ADDR1     | RA1
| ADDR2     | RA2
| I/O       | PORTC

*Note: Non-I<sup>2</sup>C locations can be changed in config.h*

### Default I<sup>2</sup>C settings

**Speed:** 100kHz<br>
**Base Address:** 0x60<br>


### Default (Non Pin) Settings

| Setting           | Default Value | Description
| ----------------- | ------------  | --------
| MEM_START         | 0x1F80        | Location of where the configs are stored in PFM. Must be changed for other memory cuts.
| I2C_BASE_ADDRESS  | 0x60          | Base address of I<sup>2</sup>C communication. The lower 3-bits should be clear to enable ADDR lines to function.
| ENABLE_ADDR_LINES | DEFINED       | When defined, the addressing lines are sampled to determine the lower 3-bits of the I<sup>2</sup>C address.
| CONFIG_ON_BOOT    | DEFINED       | Enables whether the device will attempt to load CONFIG 0 on startup. If not enabled, the device starts with the default settings.
| ENABLE_OPEN_DRAIN | DEFINED       | If defined, the INT line is an open drain line. A resistor is needed to pull-up the line to Vdd. If not defined, the line is push-pull.

### Default Pin Settings

| Setting           | Default Value | Description
| ----------------- | ------------  | --------
| DEFAULT_TRISx     | 0xFF          | Default value of TRIS on the I/O port.
| DEFAULT_LATx      | 0x00          | Default value in LAT on the I/O port.
| DEFAULT_WPUx      | 0xFF          | Default value of WPU on the I/O port.
| DEFAULT_INLVLx    | 0x00          | Default value of INLVL on the I/O port.
| DEFAULT_SLRCONx   | 0xFF          | Default value of SLRCON on the I/O port.
| DEFAULT_ODCONx    | 0x00          | Default value of ODCON on the I/O port.

## Operation

### Startup
The I<sup>2</sup>C I/O expander can be set to initialize in 2 states - a hardcoded default setup which sets all pins as inputs and disables all IOC functions, or configuration 0, which is stored in PFM. For more information about PFM configurations, see *Memory Operations*. <br>

### I<sup>2</sup>C Configuration
This example uses a 7-bit I<sup>2</sup>C address, with it defined **default 0x60** with the ***lower 3-bits set by the address lines***. On startup, the lines are polled to determine which address to use. After startup, the command Address Update (0xB0) can be executed to poll and change the address based on the current logic levels on the address lines.

### I<sup>2</sup>C Communication
For reference with this section, please consult *Command Ordering and Permissions* to see valid commands and the allowed operations associated.

#### Writing to the Device
After addressing the device, the device will always ACK. The 1st data byte sent is the starting address byte. In the case of a read (or select only command), this is the only byte sent. All successive bytes are passed to the commands as parameters, with the 2nd byte going into the command at the address, the 3rd going into the command at address + 1, etc. After each successful written byte, the internal address counter increments. The image in Figure 2 shows an example write to address and address + 1. <br>

**Important! IF data is written to a non-writable location (Read Only, Invalid, or Select Only regions), then the device will NACK and set an error code in the STATUS register.**

<img src="images/I2C_write.png"><br>
*Figure 2 - I<sup>2</sup>C Example Write*<br>

#### Reading from the Device
To read from the device, 2 I<sup>2</sup>C transactions must be executed, a write and a read. The write command sends only a single data byte to indicate the starting address of the read. The I<sup>2</sup>C bus is stopped, and the device is re-addressed. A valid read location will cause the device to ACK.<br>

**Important! IF the address set in the device is invalid, the device will NACK and set an error code in STATUS.**<br>

Each byte read from the device is the value in the associated register. If the read was successful, then the address is incremented.

**Important! IF the address enters a section of memory where reads are not allowed (Invalid, Write Only, Write Only, or Select Only), then the device sets an error code in STATUS and returns only 0x00 until the end of the transaction. The address is not incremented.**

Figure 3 (below) shows a sequential read of 2 addresses.

<img src="images/I2C_read.png"><br>
*Figure 3 - I<sup>2</sup>C Example Read*<br>


#### Command Ordering and Permissions
Figure 4 (below) shows the order of commands on the device and the associated permissions. <br>
<img src="images/CommandSet.png" width="500px"><br>
*Figure 4 - Command Order and Allowed Operations*<br>

##### Permission Descriptions

| Permission                           | Description
|------------------------------------- | -------
| Read Only (RO)                       | Commands that only return a value and cannot be set directly, such as error codes.
| Read and Write (RW)                  | General purpose commands that can be written to and read from.
| Write Only (WO)                      | Internal commands where the data stored is only for the device.
| Write Only, Indirect Access (WO, ID) | Internal commands that cannot be directly selected. Access is available only by sequential writes.
| Invalid (IV)                         | Unimplemented command. Also used as padding to prevent inadvertent activation of some commands.
| Select Only (SO)                     | This address can be selected by the 1st address byte, but reads and writes to this location are invalid. Used as a special function.

##### Command Descriptions

| Command     | Address | Permission | Function
| ----------- | ------- | ---------- | ---------
| STATUS      | 0x00    | RO         | Returns the status of the device. Cleared on read.
| IOCx        | 0x01    | RO         | Returns where an IOC event has occurred. Cleared on read. Pins must have been set as IOC for this bit to be set.
| PORTx       | 0x02    | RO         | Returns the current digital value on the I/O. (If set as an output, then this is normally LATx.)
| TRISx       | 0x03    | RW         | Read/Writes the value of TRISx on the configured port.
| LATx        | 0x04    | RW         | Read/Writes the value of LATx on the configured port.
| IOCxP       | 0x05    | RW         | Read/Writes the pin configuration for rising edges on PORTx.
| IOCxN       | 0x06    | RW         | Read/Writes the pin configuration for falling edges on PORTx.
| WPUx        | 0x08    | RW         | Enables internal weak pull-up resistors for the I/O port.
| INLVLx      | 0x09    | RW         | Sets the input level threshold for the I/O port.
| ODCONx      | 0x0A    | RW         | Enables open-drain outputs on the I/O port.
| SLRCONx     | 0x0B    | RW         | Controls whether or not the slew rate of the I/O port is run at full-speed or reduced-speed. Reduced speed slew rates are beneficial for reducing crosstalk and other sources of electromagnetic noise.
| MEM OP      | 0xA0    | WO         | Sets the memory operation to execute on the next STOP condition. *See Memory Operations for more details.*
| UNLOCK1     | N/A     | WO, ID     | Protection sequence to prevent accidental memory operations. Only accessed via sequential writes to MEM OP. **Write 0xA5**.
| UNLOCK2     | N/A     | WO, ID     | Protection sequence to prevent accidental memory operations. Only accessed via sequential writes to MEM OP. **Write 0xF0**, then STOP.
| ADR UPDATE  | 0xB0    | SO         | Updates the I<sup>2</sup>C address by repolling the address select lines. Select this register then STOP to update address.

## Memory Operations
This code example supports the ability to save and load I/O configurations to it's own internal PFM. 4 types of operations are supported:

1. Load Defaults
2. Save Configuration
3. Load Configuration
4. Save and Load Configuration

During memory operations, all interrupts are disabled and I<sup>2</sup>C will not be acknowledged.

### Setting up Memory Operations
For all memory operations, the following sequence must be followed:

 1. Select the address 0xA0
 2. Write the memory operation byte
 3. Write 0xA5
 4. Write 0xF0
 5. Stop the I<sup>2</sup>C bus
 6. Wait for INT to be asserted

 **Important! If I<sup>2</sup>C communication with the device is started during a memory write, the bus may become stuck due to the driver missing an interrupt. This is especially important if using the MCP2221A device, which cannot sense INT. In the I<sup>2</sup>C terminal, you must set a delay (100ms or more is recommended) to ensure enough time is given for the operation. The device can be recovered by pulling the MCLR (reset) line low.**

 When followed, the device will execute the programmed memory operation (see Memory Operation Byte for details) and assert the INT line when it has completed. The address selected at the completion is STATUS (0x00). A read operation can be run immediately following the memory write without the need to set the address.<br><br>

 **Important! If an error occurs before the memory operation, then the operation will not occur. The INT line will still be asserted and the address selected will be set to STATUS (0x00).**<br>

If a memory operation fails, then the STATUS register will be updated and the operation will be terminated. The exact behavior is defined per operation.
<br>

### Memory Operation Byte

<img src="images/memoryOP.png" width="500px"><br>
*Figure 5 - Memory Operation Byte*<br>

The formatting of the memory operation byte is shown in figure 5. Some configurations do not use some fields in the byte , as shown below. In this case, the unused fields can be set to any value.

| Operation     | Fields Used
| ------------- | ---------
| Load Defaults | OP
| Save          | DST, OP
| Load          | SRC, OP, BH
| Save and Load | SRC, DST, OP, BH

<br>

### Memory Commands

#### Load Defaults
This memory operation resets the current volatile settings to the defaults.<br>

**Fields Used:** OP<br>
**Failure Handling:** None. This routine invokes `void resetIO(void)` in io_control.h, which is part of the compiled code, and is not modified at runtime. It is assumed this routine cannot fail.<br>

#### Save Configuration
This memory operation writes the current volatile I/O settings to the selected configuration DST.

**Fields Used:** DST, OP<br>
**Failure Handling:** The configuration that is written to memory is verified against the copy of the data in memory. If the values mismatch, an error is set in the program. Loading mismatched memory should fail due to CRC checksum embedded with it.<br>

#### Load Configuration
This memory operation discards the current I/O settings and loads new settings from non-volatile memory. Prior to loading the configuration, the I/O pins can be set to remain in a specific state (defined by BH) until the operation has completed.

**Fields Used:** DST, OP, BH<br>
**Failure Handling:** When the memory is loaded, the CRC value is unpacked and used to verify the settings. If the CRC validation fails, then the pins will remain at the settings defined by BH, and an error will be set in STATUS.<br>

#### Save and Load Configuration
This memory operation saves the current volatile settings to DST, then loads new settings from SRC.

Note: This function does not validate that SRC and DST are different. Setting these to the same config will save and load to the same area in memory.

**Fields Used:** SRC, DST, OP, BH<br>
**Failure Handling:** Any failure in saving or loading will cause the pins to remain at BH. An error will be set in STATUS.<br>

### Memory Storage
On the PIC16-152, PFM is 14-bits wide. There are 4 fields that are saved, representing direction (TRISx), output value (LATx), and IOC (IOCxP and IOCxN). For loading the values, a simple CRC-8 checksum is calculated and packed in the high-byte, lower 2-bits of each word as shown in Figure 6. When loading, this checksum is used to determine if the data is valid and uncorrupted.<br>

There are 4 configurations possible, using 32 words worth of memory. As apart of the writing process, the entire row (32 words) must be erased, however. The save functions cache the entire row before erase, apply any changes, then rewrite all of the row.

<img src="images/memoryPacking.png" width="500px"><br>
*Figure 6 - Memory Formatting and Packing*<br>

## Error Handling

| Error Code            | Value    | Description
| --------------------- | -------- | -----------
| ERROR_NONE            | 0x00     | No error has occurred. Default Condition.
| ERROR_READ_OVERRUN    | 0x01     | Attempted read at an invalid address, but started on a valid address.
| ERROR_WRITE_OVERRUN   | 0x02     | Attempted write at an invalid location, but started on a valid address.
| ERROR_INVALID_READ_OP | 0x03     | Attempted read at an invalid address.
| ERROR_INVALID_WRITE_OP| 0x04     | Attempted write at an invalid address.
| ERROR_UNUSED          | 0x05     | Not used currently.
| ERROR_INVALID_ACCESS  | 0x06     | Attempted to directly access UNLOCK1/2.  
| ERROR_ILLEGAL_MEM_OP  | 0x07     | A memory OP was set, but not properly unlocked.
| ERROR_MEM_OP_ABORTED  | 0x08     | `isPendingMemoryOP()` detected an error was set after unlocking and aborted the memory OP.
| ERROR_MEM_INVALID_SEQ | 0x09     | An invalid sequence was provided to unlock the memory.
| ERROR_CRC_FAILED      | 0x0A     | The memory that was loaded did not match the CRC value.
| ERROR_WRITE_VERIFY    | 0x0B     | The memory written to the row does not match the internal copy of the memory to write.

## Summary
The PIC16-152 is perfect for building intelligent and flexible I/O expanders that enable more feature rich systems that can do more with fewer parts.
