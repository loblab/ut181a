/////////////////////////////////////////////////////////////////////////////
// SLABHIDtoUART.h
// For SLABHIDtoUART.dll version 2.1
// and Silicon Labs CP2110/CP2114 HID to UART
/////////////////////////////////////////////////////////////////////////////

#ifndef SLAB_HID_TO_UART_H
#define SLAB_HID_TO_UART_H

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "Types.h"

/////////////////////////////////////////////////////////////////////////////
// Return Code Definitions
/////////////////////////////////////////////////////////////////////////////

// HID_UART_STATUS
typedef int HID_UART_STATUS;

// Return Codes
#define HID_UART_SUCCESS                                                0x00
#define HID_UART_DEVICE_NOT_FOUND                                       0x01
#define HID_UART_INVALID_HANDLE                                         0x02
#define HID_UART_INVALID_DEVICE_OBJECT                                  0x03
#define HID_UART_INVALID_PARAMETER                                      0x04
#define HID_UART_INVALID_REQUEST_LENGTH                                 0x05

#define HID_UART_READ_ERROR                                             0x10
#define HID_UART_WRITE_ERROR                                            0x11
#define HID_UART_READ_TIMED_OUT                                         0x12
#define HID_UART_WRITE_TIMED_OUT                                        0x13
#define HID_UART_DEVICE_IO_FAILED                                       0x14
#define HID_UART_DEVICE_ACCESS_ERROR                                    0x15
#define HID_UART_DEVICE_NOT_SUPPORTED                                   0x16
#define HID_UART_INVALID_CONFIG_VERSION                                 0x17

#define HID_UART_UNKNOWN_ERROR                                          0xFF

/////////////////////////////////////////////////////////////////////////////
// String Definitions
/////////////////////////////////////////////////////////////////////////////

// Product String Types
#define HID_UART_GET_VID_STR                                            0x01
#define HID_UART_GET_PID_STR                                            0x02
#define HID_UART_GET_PATH_STR                                           0x03
#define HID_UART_GET_SERIAL_STR                                         0x04
#define HID_UART_GET_MANUFACTURER_STR                                   0x05
#define HID_UART_GET_PRODUCT_STR                                        0x06

// String Lengths
#define HID_UART_DEVICE_STRLEN                                          512

// HID_UART_DEVICE_STR
typedef char HID_UART_DEVICE_STR[HID_UART_DEVICE_STRLEN];

/////////////////////////////////////////////////////////////////////////////
// UART Definitions
/////////////////////////////////////////////////////////////////////////////

// Error Status
#define HID_UART_PARITY_ERROR                                           0x01
#define HID_UART_OVERRUN_ERROR                                          0x02

// Line Break Status
#define HID_UART_LINE_BREAK_INACTIVE                                    0x00
#define HID_UART_LINE_BREAK_ACTIVE                                      0x01

// Data Bits
#define HID_UART_FIVE_DATA_BITS                                         0x00
#define HID_UART_SIX_DATA_BITS                                          0x01
#define HID_UART_SEVEN_DATA_BITS                                        0x02
#define HID_UART_EIGHT_DATA_BITS                                        0x03

// Parity
#define HID_UART_NO_PARITY                                              0x00
#define HID_UART_ODD_PARITY                                             0x01
#define HID_UART_EVEN_PARITY                                            0x02
#define HID_UART_MARK_PARITY                                            0x03
#define HID_UART_SPACE_PARITY                                           0x04

// Stop Bits
// Short = 1 stop bit
// Long  = 1.5 stop bits (5 data bits)
//       = 2 stop bits (6-8 data bits)
#define HID_UART_SHORT_STOP_BIT                                         0x00
#define HID_UART_LONG_STOP_BIT                                          0x01

// Flow Control
#define HID_UART_NO_FLOW_CONTROL                                        0x00
#define HID_UART_RTS_CTS_FLOW_CONTROL                                   0x01

// Read/Write Limits
#define HID_UART_MIN_READ_SIZE                                          1
#define HID_UART_MAX_READ_SIZE                                          32768
#define HID_UART_MIN_WRITE_SIZE                                         1
#define HID_UART_MAX_WRITE_SIZE                                         4096

/////////////////////////////////////////////////////////////////////////////
// Part Number Definitions
/////////////////////////////////////////////////////////////////////////////

// Part Numbers
#define HID_UART_PART_CP2110                                            0x0A
#define HID_UART_PART_CP2114                                            0x0E

/////////////////////////////////////////////////////////////////////////////
// User Customization Definitions
/////////////////////////////////////////////////////////////////////////////

// User-Customizable Field Lock Bitmasks
#define HID_UART_LOCK_PRODUCT_STR_1                                     0x0001
#define HID_UART_LOCK_PRODUCT_STR_2                                     0x0002
#define HID_UART_LOCK_SERIAL_STR                                        0x0004
#define HID_UART_LOCK_PIN_CONFIG                                        0x0008
#define HID_UART_LOCK_VID                                               0x0100
#define HID_UART_LOCK_PID                                               0x0200
#define HID_UART_LOCK_POWER                                             0x0400
#define HID_UART_LOCK_POWER_MODE                                        0x0800
#define HID_UART_LOCK_RELEASE_VERSION                                   0x1000
#define HID_UART_LOCK_FLUSH_BUFFERS                                     0x2000
#define HID_UART_LOCK_MFG_STR_1                                         0x4000
#define HID_UART_LOCK_MFG_STR_2                                         0x8000

// Field Lock Bit Values
#define HID_UART_LOCK_UNLOCKED                                          1
#define HID_UART_LOCK_LOCKED                                            0

// Power Max Value (500 mA)
#define HID_UART_BUS_POWER_MAX                                          0xFA

// Power Modes
#define HID_UART_BUS_POWER                                              0x00
#define HID_UART_SELF_POWER_VREG_DIS                                    0x01
#define HID_UART_SELF_POWER_VREG_EN                                     0x02

// Flush Buffers Bitmasks
#define HID_UART_FLUSH_TX_OPEN                                          0x01
#define HID_UART_FLUSH_TX_CLOSE                                         0x02
#define HID_UART_FLUSH_RX_OPEN                                          0x04
#define HID_UART_FLUSH_RX_CLOSE                                         0x08

// USB Config Bitmasks
#define HID_UART_SET_VID                                                0x01
#define HID_UART_SET_PID                                                0x02
#define HID_UART_SET_POWER                                              0x04
#define HID_UART_SET_POWER_MODE                                         0x08
#define HID_UART_SET_RELEASE_VERSION                                    0x10
#define HID_UART_SET_FLUSH_BUFFERS                                      0x20

// USB Config Bit Values
#define HID_UART_SET_IGNORE                                             0
#define HID_UART_SET_PROGRAM                                            1

// String Lengths
#define HID_UART_MFG_STRLEN                                             62
#define HID_UART_PRODUCT_STRLEN                                         62
#define HID_UART_SERIAL_STRLEN                                          30

// HID_UART_MFG_STR
typedef char HID_UART_MFG_STR[HID_UART_MFG_STRLEN];

// HID_UART_PRODUCT_STR
typedef char HID_UART_PRODUCT_STR[HID_UART_PRODUCT_STRLEN];

// HID_UART_SERIAL_STR
typedef char HID_UART_SERIAL_STR[HID_UART_SERIAL_STRLEN];

/////////////////////////////////////////////////////////////////////////////
// Pin Definitions
/////////////////////////////////////////////////////////////////////////////

// Pin Config Modes
#define HID_UART_GPIO_MODE_INPUT                                        0x00
#define HID_UART_GPIO_MODE_OUTPUT_OD                                    0x01
#define HID_UART_GPIO_MODE_OUTPUT_PP                                    0x02
#define HID_UART_GPIO_MODE_FUNCTION1                                    0x03
#define HID_UART_GPIO_MODE_FUNCTION2                                    0x04

// Suspend Value Bit Values
#define HID_UART_VALUE_SUSPEND_LO                                       0
#define HID_UART_VALUE_SUSPEND_HI                                       1

// Suspend Mode Bit Values
#define HID_UART_MODE_SUSPEND_OD                                        0
#define HID_UART_MODE_SUSPEND_PP                                        1

// RS485 Active Levels
#define HID_UART_MODE_RS485_ACTIVE_LO                                   0x00
#define HID_UART_MODE_RS485_ACTIVE_HI                                   0x01

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef void* HID_UART_DEVICE;
typedef unsigned char U8;
typedef unsigned short U16;

/////////////////////////////////////////////////////////////////////////////
// Exported Library Functions
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// HidUart_GetNumDevices
HID_UART_STATUS HidUart_GetNumDevices(DWORD* numDevices, WORD vid, WORD pid);

// HidUart_GetString
HID_UART_STATUS HidUart_GetString(DWORD deviceNum, WORD vid, WORD pid, char* deviceString, DWORD options);

// HidUart_GetOpenedString
HID_UART_STATUS HidUart_GetOpenedString(HID_UART_DEVICE device, char* deviceString, DWORD options);

// HidUart_GetIndexedString
HID_UART_STATUS HidUart_GetIndexedString(DWORD deviceNum, WORD vid, WORD pid, DWORD stringIndex, char* deviceString);

// HidUart_GetOpenedIndexedString
HID_UART_STATUS HidUart_GetOpenedIndexedString(HID_UART_DEVICE device, DWORD stringIndex, char* deviceString);

// HidUart_GetAttributes
HID_UART_STATUS HidUart_GetAttributes(DWORD deviceNum, WORD vid, WORD pid, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber);

// HidUart_GetOpenedAttributes
HID_UART_STATUS HidUart_GetOpenedAttributes(HID_UART_DEVICE device, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber);

// HidUart_Open
HID_UART_STATUS HidUart_Open(HID_UART_DEVICE* device, DWORD deviceNum, WORD vid, WORD pid);

// HidUart_Close
HID_UART_STATUS HidUart_Close(HID_UART_DEVICE device);

// HidUart_IsOpened
HID_UART_STATUS HidUart_IsOpened(HID_UART_DEVICE device, BOOL* opened);

// HidUart_SetUartEnable
HID_UART_STATUS HidUart_SetUartEnable(HID_UART_DEVICE device, BOOL enable);

// HidUart_GetUartEnable
HID_UART_STATUS HidUart_GetUartEnable(HID_UART_DEVICE device, BOOL* enable);

// HidUart_Read
HID_UART_STATUS HidUart_Read(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToRead, DWORD* numBytesRead);

// HidUart_Write
HID_UART_STATUS HidUart_Write(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToWrite, DWORD* numBytesWritten);

// HidUart_FlushBuffers
HID_UART_STATUS HidUart_FlushBuffers(HID_UART_DEVICE device, BOOL flushTransmit, BOOL flushReceive);

// HidUart_CancelIo
HID_UART_STATUS HidUart_CancelIo(HID_UART_DEVICE device);

// HidUart_SetTimeouts
HID_UART_STATUS HidUart_SetTimeouts(HID_UART_DEVICE device, DWORD readTimeout, DWORD writeTimeout);

// HidUart_GetTimeouts
HID_UART_STATUS HidUart_GetTimeouts(HID_UART_DEVICE device, DWORD* readTimeout, DWORD* writeTimeout);

// HidUart_GetUARTStatus
HID_UART_STATUS HidUart_GetUartStatus(HID_UART_DEVICE device, WORD* transmitFifoSize, WORD* receiveFifoSize, BYTE* errorStatus, BYTE* lineBreakStatus);

// HidUart_SetUARTConfig
HID_UART_STATUS HidUart_SetUartConfig(HID_UART_DEVICE device, DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits, BYTE flowControl);

// HidUart_GetUARTConfig
HID_UART_STATUS HidUart_GetUartConfig(HID_UART_DEVICE device, DWORD* baudRate, BYTE* dataBits, BYTE* parity, BYTE* stopBits, BYTE* flowControl);

// HidUart_StartBreak
HID_UART_STATUS HidUart_StartBreak(HID_UART_DEVICE device, BYTE duration);

// HidUart_StopBreak
HID_UART_STATUS HidUart_StopBreak(HID_UART_DEVICE device);

// HidUart_Reset
HID_UART_STATUS HidUart_Reset(HID_UART_DEVICE device);

// HidUart_ReadLatch
HID_UART_STATUS HidUart_ReadLatch(HID_UART_DEVICE device, WORD* latchValue);

// HidUart_WriteLatch
HID_UART_STATUS HidUart_WriteLatch(HID_UART_DEVICE device, WORD latchValue, WORD latchMask);

// HidUart_GetPartNumber
HID_UART_STATUS HidUart_GetPartNumber(HID_UART_DEVICE device, BYTE* partNumber, BYTE* version);

// HidUart_GetLibraryVersion
HID_UART_STATUS HidUart_GetLibraryVersion(BYTE* major, BYTE* minor, BOOL* release);

// HidUart_GetHidLibraryVersion
HID_UART_STATUS HidUart_GetHidLibraryVersion(BYTE* major, BYTE* minor, BOOL* release);

// HidUart_GetHidGuid
HID_UART_STATUS HidUart_GetHidGuid(void* guid);

// HidUart_SetLock
HID_UART_STATUS HidUart_SetLock(HID_UART_DEVICE device, WORD lock);

// HidUart_GetLock
HID_UART_STATUS HidUart_GetLock(HID_UART_DEVICE device, WORD* lock);

// HidUart_SetUsbConfig
HID_UART_STATUS HidUart_SetUsbConfig(HID_UART_DEVICE device, WORD vid, WORD pid, BYTE power, BYTE powerMode, WORD releaseVersion, BYTE flushBuffers, BYTE mask);

// HidUart_GetUsbConfig
HID_UART_STATUS HidUart_GetUsbConfig(HID_UART_DEVICE device, WORD* vid, WORD* pid, BYTE* power, BYTE* powerMode, WORD* releaseVersion, BYTE* flushBuffers);

// HidUart_SetManufacturingString
HID_UART_STATUS HidUart_SetManufacturingString(HID_UART_DEVICE device, char* manufacturingString, BYTE strlen);

// HidUart_GetManufacturingString
HID_UART_STATUS HidUart_GetManufacturingString(HID_UART_DEVICE device, char* manufacturingString, BYTE* strlen);

// HidUart_SetProductString
HID_UART_STATUS HidUart_SetProductString(HID_UART_DEVICE device, char* productString, BYTE strlen);

// HidUart_GetProductString
HID_UART_STATUS HidUart_GetProductString(HID_UART_DEVICE device, char* productString, BYTE* strlen);

// HidUart_SetSerialString
HID_UART_STATUS HidUart_SetSerialString(HID_UART_DEVICE device, char* serialString, BYTE strlen);

// HidUart_GetSerialString
HID_UART_STATUS HidUart_GetSerialString(HID_UART_DEVICE device, char* serialString, BYTE* strlen);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SLAB_HID_TO_UART_H
