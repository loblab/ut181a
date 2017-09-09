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

#pragma once

#include <cstddef>

const int SILABS_VID = 0x10c4;
const int CP2110_PID = 0xea80;
const int CP2114_PID = 0xeab0;

#include "SLABHIDtoUART.h"

class CP211x
{
public:
    static void ListSerial(int pid=CP2110_PID, int vid=SILABS_VID);

public:
    CP211x(int pid=CP2110_PID, int vid=SILABS_VID);
    bool Open(LPCSTR serial=NULL);
    void Close();
    void ShowDeviceInfo();
    void ShowUartConfig();
    bool SetUartConfig(DWORD baudRate, BYTE dataBits, BYTE parity, BYTE stopBits, BYTE flowControl);
    bool GetUartConfig(DWORD& baudRate, BYTE& dataBits, BYTE& parity, BYTE& stopBits, BYTE& flowControl);
    bool SetTimeouts(int msRead, int msWrite);
    bool FlushBuffers();
    DWORD Read(BYTE* buffer, DWORD size);
    DWORD Write(const void* buffer, DWORD size);
    bool ReadLatch(WORD& latchValue);
    bool WriteLatch(WORD latchValue, WORD latchMask);

protected:
    int FindDevice(LPCSTR serial);

private:
    int m_pid;
    int m_vid;
    HID_UART_DEVICE m_hDevice;
};

