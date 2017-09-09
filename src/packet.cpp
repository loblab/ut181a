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
#include "packet.h"

using namespace std;
using namespace UT181A;

#define LEN_MAXMINAVG 48
#define LEN_2CH 34

LPCSTR DateTime::DateText()
{
    static char buf[16];
    sprintf(buf, "%02d/%02d/%04d", mon, day, year + 2000);
    return buf;
}

LPCSTR DateTime::TimeText()
{
    static char buf[16];
    sprintf(buf, "%02d:%02d:%02d", hour, min, sec);
    return buf;
}

void DateTime::Show()
{
    printf("%s %s\n", DateText(), TimeText());
}

bool PrecisionValue::Load(Reader& r)
{
    CHKR(r.Read(val))
    CHKR(r.Read(pre))
    return true;
}

LPCSTR PrecisionValue::Text(char* buffer) const
{
    char fmt[8];
    int digit = pre >> 4;
    //++digit;    // test, try to read 1 more digit
    sprintf(fmt, "%%6.%df", digit);
    sprintf(buffer, fmt, val);
    return buffer;
}

WORD Packet::CheckSum(const void* data, int dataLen)
{
    const BYTE* p1 = (const BYTE*) data;
    const BYTE* p2 = p1 + dataLen;
    WORD sum = 0;
    while (p1 < p2)
        sum += *p1++;
    return sum;
}

bool Packet::Load(const void* data, int size)
{
    Reader r(data, size);
    CHKR(r.Skip(2))
    CHKR(r.Read(m_len))
    CHKR(LoadBody(r))
    return true;
}

bool Packet::Dump(void* data, int size)
{
    Writer w(data, size);
    CHKW(w.Write(START_BYTE1))
    CHKW(w.Write(START_BYTE2))
    CHKW(w.Write(m_len))
    CHKW(DumpBody(w))
    WORD sum = CheckSum((const BYTE*)data + sizeof(START_BYTE1) + sizeof(START_BYTE2), m_len);
    CHKW(w.Write(sum))
    return true;
}

void time_duration(unsigned int time, int& hh, int& mm, int& ss)
{
    hh = time / 3600;
    mm = (time - 3600 * hh) / 60;
    ss = time % 60;
}

bool TimeValue::Load(Reader& r)
{
    CHKR(r.Read(val))
    CHKR(r.Read(range))
    CHKR(r.Read(time))
    return true;
}

void TimeValue::Show()
{
    int hh, mm, ss;
    time_duration(time, hh, mm, ss);
    printf(" - %f/%d (%02d:%02d:%02d)", val, range, hh, mm, ss);
}

void MonitorPacket::Show()
{
    printf("%d - %02X %02X %02X %02X %02X - %d/%d - %f %s", 
        m_len, 
        m_head[0], m_head[1], m_head[2], m_head[3], m_head[4],
        m_r1, m_r2, m_val, m_unit);
    if (m_len < LEN_MAXMINAVG)
    {
        printf(" - %f", m_val2);
    }
    else
    {
        m_max.Show();
        m_avg.Show();
        m_min.Show();
    }
    printf("\n");
}

bool MonitorPacket::LoadBody(Reader& r)
{
    CHKR(r.Read(m_head, 5))
    CHKR(r.Read(m_r1))
    CHKR(r.Read(m_val))
    CHKR(r.Read(m_r2))
    if (m_len < LEN_MAXMINAVG)
    {
        CHKR(r.Read(m_unit, 8))
        CHKR(r.Read(m_val2))
        BYTE tmp;
        CHKR(r.Read(tmp))
    }
    else
    {
        CHKR(m_max.Load(r))
        CHKR(m_avg.Load(r))
        CHKR(m_min.Load(r))
    }
    CHKR(r.Read(m_unit, 8))
    return true;
}

bool RecordCountPacket::LoadBody(Reader& r)
{
    CHKR(r.Skip(2))
    CHKR(r.Read(m_count))
    return true;
}

void RecInfoPacket::Show()
{
    int hh, mm, ss;
    time_duration(m_duration, hh, mm, ss);
    char buf1[8], buf2[8], buf3[8];
    printf(
#ifdef _DEBUG
        "(%02X %02X) "
#endif //_DEBUG
        "%s %s - %9s - %3ds x %6d = %02d:%02d:%02d - [ %s ~ %s ] %s (%s)", 
#ifdef _DEBUG
        m_u1, m_u2,
#endif //_DEBUG
        m_time.DateText(), m_time.TimeText(),
        m_name,
        m_interval, m_samples, hh, mm, ss,
        m_min.Text(buf1), m_max.Text(buf2), m_avg.Text(buf3),
        m_unit);
    printf("\n");
}

bool RecInfoPacket::LoadBody(Reader& r)
{
    CHKR(r.Read(m_u1))
    CHKR(r.Read(m_name, 10))
    CHKR(r.Read(m_u2))
    CHKR(r.Read(m_unit, 8))
    CHKR(r.Read(m_interval))
    CHKR(r.Read(m_duration))
    CHKR(r.Read(m_samples))
    CHKR(m_max.Load(r))
    CHKR(m_avg.Load(r))
    CHKR(m_min.Load(r))
    CHKR(r.Read((BYTE*)&m_time, 4))
    return true;
}

void RecDataPacket::Show(DWORD offset)
{
        char buf[8];
        for (WORD index = 0; index < m_count; index++)
        {
            printf("%4d - %s %s - %s\n",
                offset + index, 
                m_dt[index].DateText(), m_dt[index].TimeText(), 
                m_val[index].Text(buf));
        }
}

bool RecDataPacket::ExportCSV(ostream& os, DWORD offset)
{
    char buf[8];
    for (WORD index = 0; index < m_count; index++)
    {
#ifdef _DEBUG
        printf("%4d - %s %s - %s\n",
            offset + index, 
            m_dt[index].DateText(), m_dt[index].TimeText(), 
            m_val[index].Text(buf));
#endif
        os << offset + index << ", " 
            << m_dt[index].DateText() << ", "
            << m_dt[index].TimeText() << ", "
            << m_val[index].Text(buf) << endl;
    }
    return true;
}

bool RecDataPacket::LoadBody(Reader& r)
{
    r.Skip(2);
    WORD remain = m_len;
    WORD index = 0;
    while (remain >= 9)
    {
        if (index >= MAX_ITEM_COUNT)
            return false;
        CHKR(m_val[index].Load(r))
        CHKR(r.Read((BYTE*)(m_dt + index), 4))
        ++index;
        remain -= 9;
    }
    m_count = index;
    return true;
}

