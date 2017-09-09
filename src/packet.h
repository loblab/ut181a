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

#include "reader.h"
#include "writer.h"
#include <iostream>

namespace UT181A {

struct DateTime
{
    DWORD year : 6;
    DWORD mon    : 4;
    DWORD day    : 5;
    DWORD hour : 5;
    DWORD min    : 6;
    DWORD sec    : 6;

    void Show();
    LPCSTR DateText();
    LPCSTR TimeText();
};

struct PrecisionValue
{
    float val;
    BYTE pre;

    bool Load(Reader& r);
    LPCSTR Text(char* buffer) const;
};

class Packet
{
public:
    static const BYTE START_BYTE1 = 0xAB;
    static const BYTE START_BYTE2 = 0xCD;
    static WORD CheckSum(const void* data, int dataLen);

protected:
    WORD m_len;

public:
    Packet() : m_len(0) {}
    Packet(WORD len) : m_len(len) {}
    int GetSize() const { return m_len + sizeof(START_BYTE1) + sizeof(START_BYTE2) + sizeof(m_len); }
    bool Load(const void* data, int size);
    bool Dump(void* data, int size);
    
protected:
    virtual bool DumpBody(Writer& w) { return false; }
    virtual bool LoadBody(Reader& r) { return false; }
};

class Command : public Packet
{
public:
    static const BYTE CMD_MONITOR = 0x05;
    static const BYTE CMD_REC_COUNT = 0x0E;
    static const BYTE CMD_REC_INFO = 0x0C;
    static const BYTE CMD_REC_DATA = 0x0D;

protected:
    BYTE m_cmd;

public:
    Command(BYTE cmd, WORD len=3) : Packet(len), m_cmd(cmd) {}
    virtual bool DumpBody(Writer& w) 
    { 
        CHKW(w.Write(m_cmd));
        return true;
    }
};

class MonitorCommand : public Command
{
protected:
    BYTE m_start;

public:
    MonitorCommand(BYTE start) : Command(CMD_MONITOR, 4), m_start(start) {}
    virtual bool DumpBody(Writer& w) 
    { 
        CHKW(w.Write(m_cmd));
        CHKW(w.Write(m_start));
        return true;
    }
};

class RecInfoCommand : public Command
{
protected:
    WORD m_index;

public:
    RecInfoCommand(WORD index) : Command(CMD_REC_INFO, 5), m_index(index) {}
    virtual bool DumpBody(Writer& w) 
    { 
        CHKW(w.Write(m_cmd));
        CHKW(w.Write(m_index));
        return true;
    }
};

class RecDataCommand : public Command
{
protected:
    WORD m_indexRecord;
    DWORD m_indexData;

public:
    RecDataCommand(WORD indexRecord, DWORD indexData) : Command(CMD_REC_DATA, 9), 
        m_indexRecord(indexRecord), m_indexData(indexData) {}
    virtual bool DumpBody(Writer& w) 
    { 
        CHKW(w.Write(m_cmd));
        CHKW(w.Write(m_indexRecord));
        CHKW(w.Write(m_indexData));
        return true;
    }
};

struct TimeValue
{
public:
    float val;
    BYTE range;
    DWORD time;

public:
    bool Load(Reader& r);
    void Show();
};

class MonitorPacket : public Packet
{
public:
    virtual bool LoadBody(Reader& r);
    void Show();

protected:
    BYTE m_head[5];
    BYTE m_r1, m_r2;
    float m_val; 
    float m_val2;
    TimeValue m_max, m_min, m_avg;
    char m_unit[8];
};

class RecordCountPacket : public Packet
{
public:
    virtual bool LoadBody(Reader& r);

public:
    WORD m_count;
};

class RecInfoPacket : public Packet
{
public:
    virtual bool LoadBody(Reader& r);
    void Show();

public:
    BYTE m_u1, m_u2;
    char m_name[10];
    char m_unit[8];
    WORD m_interval;
    DWORD m_duration;
    DWORD m_samples;
    PrecisionValue m_max, m_min, m_avg;
    DateTime m_time;
};

class RecDataPacket : public Packet
{
public:
    virtual bool LoadBody(Reader& r);
    void Show(DWORD offset=1);
    bool ExportCSV(std::ostream& os, DWORD offset=1);
    WORD GetCount() const { return m_count; }

protected:
    static const WORD MAX_ITEM_COUNT = 250;
    DateTime m_dt[MAX_ITEM_COUNT];
    PrecisionValue m_val[MAX_ITEM_COUNT];
    WORD m_count;
};

} //namespace UT181A

