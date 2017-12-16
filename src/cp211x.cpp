// Copyright 2017 loblab
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string>
#include "cp211x.h"
#include "debug.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define ERROR_LEVEL_SUCCESS             0
#define ERROR_LEVEL_INVALID_ARG         1
#define ERROR_LEVEL_API_CODE            2

/////////////////////////////////////////////////////////////////////////////
// Static Global Functions
/////////////////////////////////////////////////////////////////////////////

// Convert HID_UART_STATUS value to a printable string
static string DecodeHidUartStatus(HID_UART_STATUS status)
{
    string text;
    char temp[100];
    
    switch (status)
    {
    case HID_UART_SUCCESS:                        text = "HID_UART_SUCCESS"; break;
    case HID_UART_DEVICE_NOT_FOUND:             text = "HID_UART_DEVICE_NOT_FOUND"; break;
    case HID_UART_INVALID_HANDLE:                 text = "HID_UART_INVALID_HANDLE"; break;
    case HID_UART_INVALID_DEVICE_OBJECT:        text = "HID_UART_INVALID_DEVICE_OBJECT"; break;
    case HID_UART_INVALID_PARAMETER:            text = "HID_UART_INVALID_PARAMETER"; break;
    case HID_UART_INVALID_REQUEST_LENGTH:         text = "HID_UART_INVALID_REQUEST_LENGTH"; break;
    case HID_UART_READ_ERROR:                     text = "HID_UART_READ_ERROR"; break;
    case HID_UART_WRITE_ERROR:                    text = "HID_UART_WRITE_ERROR"; break;
    case HID_UART_READ_TIMED_OUT:                 text = "HID_UART_READ_TIMED_OUT"; break;
    case HID_UART_WRITE_TIMED_OUT:                text = "HID_UART_WRITE_TIMED_OUT"; break;
    case HID_UART_DEVICE_IO_FAILED:             text = "HID_UART_DEVICE_IO_FAILED"; break;
    case HID_UART_DEVICE_ACCESS_ERROR:            text = "HID_UART_DEVICE_ACCESS_ERROR"; break;
    case HID_UART_DEVICE_NOT_SUPPORTED:         text = "HID_UART_DEVICE_NOT_SUPPORTED"; break;
    case HID_UART_UNKNOWN_ERROR:                text = "HID_UART_UNKNOWN_ERROR"; break;
    default:
        sprintf(temp, "Error Status (0x%02X)", status);
        text = temp;
        break;
    }
    
    return text;
}

// Convert Get/SetUartConfig() parity value to a printable string
static string DecodeParity(BYTE parity)
{
    string text;
    
    switch (parity)
    {
    case HID_UART_NO_PARITY:    text = "N"; break;
    case HID_UART_ODD_PARITY:     text = "O"; break;
    case HID_UART_EVEN_PARITY:    text = "E"; break;
    case HID_UART_MARK_PARITY:    text = "M"; break;
    case HID_UART_SPACE_PARITY: text = "S"; break;
    default:                    text = "?"; break;
    }
    
    return text;
}

// Convert Get/SetUartConfig() flow control value to a printable string
static string DecodeFlowControl(BYTE flowControl)
{
    string text;
    
    switch (flowControl)
    {
    case HID_UART_NO_FLOW_CONTROL:        text = "None";            break;
    case HID_UART_RTS_CTS_FLOW_CONTROL: text = "RTS/CTS";         break;
    }
    
    return text;
}

void CP211x:: ListSerial(int pid, int vid)
{
    HID_UART_STATUS status;
    DWORD numDevices = 0;
    HID_UART_DEVICE_STR serial;
    
    status = HidUart_GetNumDevices(&numDevices, vid, pid);
    
    if (status == HID_UART_SUCCESS)
    {
        printf("List of CP211x Serial Strings\n");
        printf("=============================\n");
        
        for (DWORD i = 0; i < numDevices; i++)
        {
            if (HidUart_GetString(i, vid, pid, serial, HID_UART_GET_SERIAL_STR) == HID_UART_SUCCESS)
            {
                printf("%s\n", serial);
            }
        }
    }
}

CP211x::CP211x(int pid, int vid)
: m_pid(pid),
    m_vid(vid)
{
    m_hDevice = NULL;
}

int CP211x::FindDevice(LPCSTR serial)
{
    HID_UART_STATUS status;
    DWORD numDevices;
    string s0 = serial;

    status = HidUart_GetNumDevices(&numDevices, m_vid, m_pid);
    if (status != HID_UART_SUCCESS)
        return -1;
    
    for (DWORD i = 0; i < numDevices; i++)
    {
        HID_UART_DEVICE_STR s;
        if (HidUart_GetString(i, m_vid, m_pid, s, HID_UART_GET_SERIAL_STR) == HID_UART_SUCCESS)
        {
            if (s0 == s)
                return i;
        }
    }
    return -1;
}

bool CP211x::Open(LPCSTR serial)
{
    HID_UART_STATUS status;
    int index = serial ? FindDevice(serial) : 0;
    if (index < 0)
        return false;
    
    status = HidUart_Open(&m_hDevice, index, m_vid, m_pid);
    if (status != HID_UART_SUCCESS)
        return false;

    status = HidUart_SetTimeouts(m_hDevice, 256, 1000);
    if (status != HID_UART_SUCCESS)
        return false;
    
    return true;
}

void CP211x::Close()
{
    if (m_hDevice)
    {
        HidUart_Close(m_hDevice);
        m_hDevice = NULL;
    }
}

bool CP211x::SetTimeouts(int msRead, int msWrite)
{
    HID_UART_STATUS status;
    status = HidUart_SetTimeouts(m_hDevice, msRead, msWrite);
    if (status != HID_UART_SUCCESS)
        return false;

    return true;
}

bool CP211x::FlushBuffers()
{
    HID_UART_STATUS status;
    status = HidUart_FlushBuffers (m_hDevice, TRUE, TRUE);
    if (status != HID_UART_SUCCESS)
        return false;

    return true;
}

void CP211x::ShowDeviceInfo()
{
    HID_UART_STATUS status;
    
    WORD vid, pid, releaseNumber;
    HID_UART_DEVICE_STR serial;
    HID_UART_DEVICE_STR path;
    BYTE partNumber, version;
    HID_UART_DEVICE_STR mfr;
    HID_UART_DEVICE_STR product;
    
    printf("\n");
    printf("Device Information\n");
    printf("==================\n");
    
    status = HidUart_GetOpenedAttributes(m_hDevice, &vid, &pid, &releaseNumber);
    if (status == HID_UART_SUCCESS)
        printf("VID: 0x%04X PID: 0x%04X Device Release Number: 0x%04X\n", vid, pid, releaseNumber);
    else
        printf("VID: PID: m_hDevice Release Number: %s\n", DecodeHidUartStatus(status).c_str());
    
    status = HidUart_GetOpenedString(m_hDevice, serial, HID_UART_GET_SERIAL_STR);
    if (status == HID_UART_SUCCESS)
        printf("Serial: %s\n", serial);
    else
        printf("Serial: %s\n", DecodeHidUartStatus(status).c_str());
    
    status = HidUart_GetOpenedString(m_hDevice, path, HID_UART_GET_PATH_STR);
    if (status == HID_UART_SUCCESS)
        printf("Path: %s\n", path);
    else
        printf("Path: %s", DecodeHidUartStatus(status).c_str());
    
    status = HidUart_GetPartNumber(m_hDevice, &partNumber, &version);
    if (status == HID_UART_SUCCESS)
        printf("Part Number: %u Version %u\n", partNumber, version);
    else
        printf("Part Number: Version: %s\n", DecodeHidUartStatus(status).c_str());
    
    status = HidUart_GetOpenedString(m_hDevice, mfr, HID_UART_GET_MANUFACTURER_STR);
    if (status == HID_UART_SUCCESS)
        printf("Manufacturer: %s\n", mfr);
    else
        printf("Manufacturer: %s\n", DecodeHidUartStatus(status).c_str());
    
    status = HidUart_GetOpenedString(m_hDevice, product, HID_UART_GET_PRODUCT_STR);
    if (status == HID_UART_SUCCESS)
        printf("Product: %s\n", product);
    else
        printf("Product: %s\n", DecodeHidUartStatus(status).c_str());
}


void CP211x::ShowUartConfig()
{
    HID_UART_STATUS status;
    DWORD baudRate;
    BYTE dataBits;
    BYTE parity;
    BYTE stopBits;
    BYTE flowControl;
    
    printf("\n");
    printf("Get UART Configuration\n");
    printf("======================\n");
    
    status = HidUart_GetUartConfig(m_hDevice, &baudRate, &dataBits, &parity, &stopBits, &flowControl);
    if (status == HID_UART_SUCCESS)
    {
        printf("Baud Rate: %u\n", (unsigned int)baudRate);
        printf("Data Bits: %u\n", dataBits + 5);
        printf("Parity: %s\n", DecodeParity(parity).c_str());
        printf("Stop Bits: %s\n", stopBits == HID_UART_SHORT_STOP_BIT ? "1" : "1.5/2");
        printf("Flow Control: %s\n", DecodeFlowControl(flowControl).c_str());
    }
    else
    {
        fprintf(stderr, "Error retrieving UART config: %s\n", DecodeHidUartStatus(status).c_str());
    }
}

bool CP211x::GetUartConfig(DWORD& baudRate, BYTE& dataBits, BYTE& parity, BYTE& stopBits, BYTE& flowControl)
{
    HID_UART_STATUS status;
    status = HidUart_GetUartConfig(m_hDevice, &baudRate, &dataBits, &parity, &stopBits, &flowControl);
    if (status != HID_UART_SUCCESS)
    {
        fprintf(stderr, "Error retrieving UART config: %s\n", DecodeHidUartStatus(status).c_str());
        return false;
    }
    
    return true;
}

bool CP211x::SetUartConfig(DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits, BYTE flowControl)
{
    HID_UART_STATUS status;
    status = HidUart_SetUartConfig(m_hDevice, baudRate, dataBits, parity, stopBits, flowControl);
    if (status != HID_UART_SUCCESS)
    {
        fprintf(stderr, "Failed setting UART config: %s\n", DecodeHidUartStatus(status).c_str());
        return false;
    }
    
    return true;
}

int CP211x::Read(BYTE* buffer, DWORD size)
{
    DWORD actual;
    HID_UART_STATUS status;
    status = HidUart_Read(m_hDevice, buffer, size, &actual);
    if (status == HID_UART_SUCCESS)
    {
        //dumpBin("Input", buffer, actual);
        return (int)actual;
    }
    if (status == HID_UART_READ_TIMED_OUT)
    {
        fprintf(stderr, "Read timeout: %d\n", actual);
        return actual > 0 ? actual : -1;
    }
    return 0;
}

int CP211x::Write(const void* buffer, DWORD size)
{
    DWORD actual;
    HID_UART_STATUS status;
    status = HidUart_Write(m_hDevice, (BYTE*)buffer, size, &actual);
    if (status == HID_UART_SUCCESS)
    {
        //dumpBin("Output", buffer, actual);
        return (int)actual;
    }
    if (status == HID_UART_WRITE_TIMED_OUT)
    {
        fprintf(stderr, "Write timeout: %d\n", actual);
        return actual > 0 ? actual : -1;
    }
    return 0;
}

bool CP211x::ReadLatch(WORD& latchValue)
{
    HID_UART_STATUS status;
    
    status = HidUart_ReadLatch(m_hDevice, &latchValue);
    if (status != HID_UART_SUCCESS)
    {
        fprintf(stderr, "Failed to read latch: %s\n", DecodeHidUartStatus(status).c_str());
        return false;
    }

    printf("Value: 0x%04X\n", latchValue);
    return true;
}

bool CP211x::WriteLatch(WORD latchValue, WORD latchMask)
{
    HID_UART_STATUS status;
    
    status = HidUart_WriteLatch(m_hDevice, latchValue, latchMask);
    if (status != HID_UART_SUCCESS)
    {
        fprintf(stderr, "Failed to write latch: %s\n", DecodeHidUartStatus(status).c_str());
        return false;
    }
    return true;
}

