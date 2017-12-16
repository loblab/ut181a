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
#include <sys/time.h>
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
    int dataSize = packet.GetSize();
    int wrSize = m_tx.Write(buffer, dataSize);
    if (wrSize < 0)
        return false;
    if (wrSize == dataSize)
    {
        if (g_debug >= 2)
            dumpBin("Output", buffer, dataSize, false);
        return true;
    }
    dumpBin("Failed to output", buffer, dataSize, false);
    return false;
}

Device::Device(LPCSTR serial)
: m_serial(serial)
{
}

bool Device::Open()
{
    if (g_debug >= 3)
        m_tx.ListSerial();
    if (! m_tx.Open(m_serial))
        return false;
    if (g_debug >= 3)
        m_tx.ShowDeviceInfo();
    if (g_debug >= 4)
        m_tx.ShowUartConfig();
    if (!m_tx.SetUartConfig(9600, 3, HID_UART_NO_PARITY, HID_UART_SHORT_STOP_BIT, HID_UART_NO_FLOW_CONTROL))
    {
        Close();
        return false;
    }
    if (g_debug >= 3)
        m_tx.ShowUartConfig();
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
    m_tx.SetTimeouts(128, 1000);
    
    MonitorCommand cmd(1);
    if (!SendPacket(cmd))
        return false;

    //ignore remaining data in the buffer
    const DWORD respSize = 10;
    BYTE buffer[respSize];
    if (m_tx.Read(buffer, respSize) < 0)
        return false;

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
        int ret = m_tx.Read(p, 1);
        if (ret < 0) // timeout
            return 0;
        if (ret == 0)
            continue;
        if (*p != Packet::START_BYTE1)
            continue;
        p++;
        if (m_tx.Read(p, 1) <= 0)
            continue;
        if (*p != Packet::START_BYTE2)
            continue;
        p++;
        if (m_tx.Read(p, 2) <= 0)
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
    while (remain > 0)
    {
        int actual = m_tx.Read(p, remain);
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

bool Device::Monitor(int& quit_flag)
{
    struct timeval t1, t2;
    time_t now;

    if (!StartMonitor())
        return false;

    BYTE buffer[BUFFER_SIZE];
    MonitorPacket packet;
    gettimeofday(&t1, NULL);
    printf("Date, Time, Elapsed, Value, Unit\n");
    while (!quit_flag)
    {
        int len = ReadPacket(buffer, BUFFER_SIZE);
        if (len < 1)
            continue;
        gettimeofday(&t2, NULL);
        now = time(NULL);
        if (g_debug > 0)
            dumpBin("Input", buffer, len, false);
        if (packet.Load(buffer, len))
        {
            if (g_debug > 0)
                packet.ShowRaw();
            struct tm* tinfo = localtime(&now);
            char tbuf[32];
            strftime(tbuf, sizeof(tbuf), "%Y/%m/%d, %H:%M:%S", tinfo);
            double diff = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1e6;
            printf("%s, %.3f, ", tbuf, diff);
            packet.Show();
        }
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
    int actual = ReadPacket(buffer, respSize);
    if (g_debug >= 2)
        dumpBin("Input", buffer, actual, false);

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
    if (g_debug >= 2)
        dumpBin("Input", buffer, actual, true);
    RecInfoPacket packet;
    bool b = packet.Load(buffer, actual);
    if (g_debug == 0)
    {
        printf("%02d - ", index);
        if (b)
            packet.Show();
    }
    return b ? packet.m_samples : 0;
}

int Device::ListRecord(int& quit_flag)
{
    WORD total = GetRecordCount();
    printf("Total: %d record%c\n", total, (total > 1 ? 's' : ' '));

    for (int i = 1; i <= total; i++)
    {
        if (quit_flag)
            break;
        if (!ShowRecordInfo(i))
            return -1;
    }
    return total;
}

bool Device::ReceiveRecord(WORD index, int& quit_flag)
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
        if (quit_flag)
            break;
        RecDataCommand cmd(index, item);
        if (!SendPacket(cmd))
        {
            fs.close();
            return false;
        }

        int actual    = ReadPacket(buffer, BUFFER_SIZE);
        if (g_debug >= 2)
            dumpBin("Input", buffer, actual, false);
        RecDataPacket packet;
        if (packet.Load(buffer, actual))
        {
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
    printf("%s record #%d.\n", (quit_flag ? "Aborted" : "Completed"), index);
    return true;
}

