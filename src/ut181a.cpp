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
#include <unistd.h>
#include <fstream>
#include <time.h>
#include "ut181a.h"
#include "debug.h"
#include "packet.h"
#include "reader.h"
#include "writer.h"

using namespace UT181A;

const int BUFFER_SIZE = 8192;

bool Device::SendPacket(Packet& packet)
{
    BYTE buffer[BUFFER_SIZE];
    if (!packet.Dump(buffer, BUFFER_SIZE))
        return false;
    DWORD dataSize = packet.GetSize();
    DWORD wrSize = m_tx.Write(buffer, dataSize);
    if (wrSize == dataSize)
    {
#ifdef _DEBUG
        dumpBin("Output", buffer, dataSize, false);
#endif //_DEBUG
        return true;
    }
    dumpBin("Failed to output", buffer, dataSize, false);
    return false;
}

void Device::Test()
{
    m_tx.ListSerial();
    m_tx.Open();
    usleep(2000);
    //m_tx.ShowDeviceInfo();

    //m_tx.SetUartConfig(9600, 3, HID_UART_NO_PARITY, HID_UART_SHORT_STOP_BIT, HID_UART_NO_FLOW_CONTROL);
    m_tx.ShowUartConfig();

    /*
    DWORD baudRate;
    BYTE dataBits;
    BYTE parity;
    BYTE stopBits;
    BYTE flowControl;
    m_tx.GetUartConfig(baudRate, dataBits, parity, stopBits, flowControl);
    baudRate = 9600;
    m_tx.SetUartConfig(baudRate, dataBits, parity, stopBits, flowControl);
    m_tx.ShowUartConfig();
    */

    StartMonitor();
    const DWORD blockSize = 37;
    BYTE buffer[blockSize];
    for (int i = 0; i < 20; i++)
    {
        m_tx.Read(buffer, blockSize);
        //usleep(10000);
    }

    StopMonitor();
    m_tx.Close();
}

Device::Device(LPCSTR serial)
: m_serial(serial)
{
}

bool Device::Open()
{
    if (! m_tx.Open(m_serial))
        return false;
    if (!m_tx.SetUartConfig(9600, 3, HID_UART_NO_PARITY, HID_UART_SHORT_STOP_BIT, HID_UART_NO_FLOW_CONTROL))
    {
        Close();
        return false;
    }
    usleep(3000);
    return true;
}

void Device::Close()
{
    m_tx.Close();
}

bool Device::StartMonitor()
{
    m_tx.FlushBuffers();
    m_tx.SetTimeouts(1, 1000);
    
    MonitorCommand cmd(1);
    if (!SendPacket(cmd))
        return false;

    //ignore remaining data in the buffer
    const DWORD respSize = 10;
    BYTE buffer[respSize];
    m_tx.Read(buffer, respSize);

    return m_tx.SetTimeouts(256, 1000);
}

bool Device::StopMonitor()
{
    MonitorCommand cmd(0);
    if (!SendPacket(cmd))
        return false;
    return true;
}

int Device::ReadPacket(BYTE* buffer, int size)
{
    int len = 0;
    BYTE* p;
    while (len == 0)
    {
        p = buffer;
        if (!m_tx.Read(p, 1))
            continue;
        if (*p != Packet::START_BYTE1)
            continue;
        p++;
        if (!m_tx.Read(p, 1))
            continue;
        if (*p != Packet::START_BYTE2)
            continue;
        p++;
        if (!m_tx.Read(p, 2))
            continue;
        WORD t1 = *p++;
        WORD t2 = *p++;
        len = t1 + (t2 << 8);
        if (len + 4 > size)
        {
            printf("Data (%d) > buffer (%d)\n", len + 4, size);
            return 0;
        }
    }
    DWORD remain = len;
    while (remain > 0) {
        DWORD actual = m_tx.Read(p, remain);
        if (actual == 0)
        {
            printf("Could not read data, expected %d bytes\n", remain);
            return 0;
        }
        p += actual;
        //printf("Remain: %d - Actual: %d\n", remain, actual);
        remain -= actual;
    }
    WORD endword = *(--p);
    endword <<= 8;
    endword += *(--p);
    WORD checksum = Packet::CheckSum(buffer + 2, len);
    if (endword != checksum)
    {
        printf("Checksum error: %04X != %04X\n", endword, checksum);
        return 0;
    }
    return len + 4;
}

bool Device::Monitor(int loop)
{
    if (!StartMonitor())
        return false;

    BYTE buffer[BUFFER_SIZE];
    MonitorPacket packet;
    for (int i = 0; i < loop; i++)
    {
        int len = ReadPacket(buffer, BUFFER_SIZE);
        if (len < 1)
            continue;
        dumpBin("Input", buffer, len, false);
        if (packet.Load(buffer, len))
            packet.Show();
    }

    return StopMonitor();
}

WORD Device::GetRecordCount()
{
    //m_tx.FlushBuffers();
    Command cmd(Command::CMD_REC_COUNT);
    if (!SendPacket(cmd))
        return 0;

    //m_tx.SetTimeouts(256, 1000);

    const DWORD respSize = 12;
    BYTE buffer[respSize];
    int actual    = ReadPacket(buffer, respSize);
#ifdef _DEBUG
    dumpBin("Input", buffer, actual, false);
#endif //_DEBUG

    RecordCountPacket packet;
    if (!packet.Load(buffer, actual))
        return 0;
    WORD c = packet.m_count;

    if (c > 20) {
        printf("Seems incorrect: %d records\n", c);
        return 0;
    }

    return c;
}

DWORD Device::ShowRecordInfo(WORD index)
{
    RecInfoCommand cmd(index);
    if (!SendPacket(cmd))
        return 0;

    BYTE buffer[BUFFER_SIZE];
    int actual    = ReadPacket(buffer, BUFFER_SIZE);
#ifdef _DEBUG
    dumpBin("Input", buffer, actual, true);
#endif //_DEBUG
    RecInfoPacket packet;
    bool b = packet.Load(buffer, actual);
#ifndef _DEBUG
    printf("%02d - ", index);
    if (b)
        packet.Show();
#endif //_DEBUG
    return b ? packet.m_samples : 0;
}

int Device::ListRecord()
{
    WORD total = GetRecordCount();
    printf("Total: %d record%c\n", total, (total > 1 ? 's' : ' '));

    for (int i = 1; i <= total; i++)
    {
        if (!ShowRecordInfo(i))
            return -1;
    }
    return total;
}

bool Device::ReceiveRecord(WORD index)
{
    printf("Receive record #%d...\n", index);
    DWORD total = ShowRecordInfo(index);
    BYTE buffer[BUFFER_SIZE];
    char filename[16];
    sprintf(filename, "%02d.csv", index);
    std::ofstream fs;
    fs.open(filename);
    fs << "Index, Date, Time, Value" << std::endl;

    time_t t1, t2;
    t1 = time(NULL);
    for (DWORD item = 1; item <= total; item += 250)
    {
        RecDataCommand cmd(index, item);
        if (!SendPacket(cmd)) {
            fs.close();
            return false;
        }

        int actual    = ReadPacket(buffer, BUFFER_SIZE);
#ifdef _DEBUG
        dumpBin("Input", buffer, actual, false);
#endif //_DEBUG
        RecDataPacket packet;
        if (packet.Load(buffer, actual)) {
        int count = item + packet.GetCount() - 1;
        t2 = time(NULL);
            if (total < 100)
                packet.Show(item);
            else
                printf("%6.1fs: %6.2f%% (%6d/%d)\n", difftime(t2, t1), 100.0 * count / total, count, total);
            packet.ExportCSV(fs, item);
        }
    }

    fs.close();
    printf("Completed record #%d.\n", index);
    return true;
}

