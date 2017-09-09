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

#include "reader.h"
#include <string.h>

Reader::Reader(const void* buffer, int size)
{
    m_p1 = (const BYTE*) buffer;
    m_p2 = m_p1 + size;
    m_p = m_p1;
}

bool Reader::Skip(int size)
{
    if (m_p + size > m_p2)
        return false;
    m_p += size;
    return true;
}

bool Reader::Read(char& val)
{
    if (m_p >= m_p2)
        return false;
    val = *(const char*)m_p++;
    return true;
}

bool Reader::Read(BYTE& val)
{
    if (m_p >= m_p2)
        return false;
    val = *m_p++;
    return true;
}

bool Reader::Read(short& val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *p++ = *m_p++;
    *p++ = *m_p++;
    return true;
}

bool Reader::Read(WORD& val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *p++ = *m_p++;
    *p++ = *m_p++;
    return true;
}

bool Reader::Read(int& val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    return true;
}

bool Reader::Read(DWORD& val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    return true;
}

bool Reader::Read(float& val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    *p++ = *m_p++;
    return true;
}


bool Reader::Read(char* str, int size)
{
    int len = strlen((const char*)m_p);
    if (m_p + len >= m_p2)
        return false;
    if (len >= size)
        return false;
    memcpy(str, m_p, size);
    m_p += size;
    return true;
}

bool Reader::Read(BYTE* data, int size)
{
    if (m_p + size > m_p2)
        return false;
    memcpy(data, m_p, size);
    m_p += size;
    return true;
}

#include <iostream>
#include "debug.h"
using namespace std;

void Reader::SelfTest()
{
    const char* data = "\xAB\xCD\x21\x00\x02\x08\x01\x11\x31\x01\x8A\xAA\x20\x40\x40\x56\x44\x43\x00\x00\x00\x00\x00\xF8\x79\x22\x40\x56\x44\x43\x00\x00\x00\x00\x00\xD0\x05";
    dumpBin("Data", data, 37, true);
    Reader r(data, 37);
    short len;
    BYTE r1, r2;
    float val1, val2;
    char unit1[8], unit2[8];
    r.Skip(2);
    r.Read(len);
    r.Skip(5);
    r.Read(r1);
    r.Read(val1);
    r.Read(r2);
    r.Read(unit1, 8);
    r.Read(val2);
    r.Read(unit2, 8);
    cout << "len: " << len << endl;
    cout << "r1: " << (int)r1 << endl;
    cout << "val1: " << val1 << unit1 << endl;
    cout << "r2: " << (int)r2 << endl;
    cout << "val2: " << val2 << unit2 << endl;
}
