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

#include <string.h>
#include <stdio.h>
#include "writer.h"
#include "debug.h"

Writer::Writer(void* buffer, int size)
{
    m_p1 = (BYTE*) buffer;
    m_p2 = m_p1 + size;
    m_p = m_p1;
}

bool Writer::Skip(int size)
{
    if (m_p + size > m_p2)
        return false;
    m_p += size;
    return true;
}

bool Writer::Write(char val)
{
    if (m_p >= m_p2)
        return false;
    *(char*)m_p++ = val;
    return true;
}

bool Writer::Write(BYTE val)
{
    if (m_p >= m_p2)
        return false;
    *m_p++ = val;
    return true;
}

bool Writer::Write(short val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *m_p++ = *p++;
    *m_p++ = *p++;
    return true;
}

bool Writer::Write(WORD val)
{
    //printf("%p - %p\n", m_p + sizeof(val), m_p2);
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *m_p++ = *p++;
    *m_p++ = *p++;
    return true;
}

bool Writer::Write(int val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    return true;
}

bool Writer::Write(DWORD val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    return true;
}

bool Writer::Write(float val)
{
    if (m_p + sizeof(val) > m_p2)
        return false;
    BYTE* p = (BYTE*)&val;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    *m_p++ = *p++;
    return true;
}


bool Writer::Write(const char* str, int size)
{
    int len = strlen(str);
    if (m_p + len >= m_p2)
        return false;
    if (m_p + size > m_p2)
        return false;
    strcpy((char*)m_p, str);
    m_p += size;
    return true;
}

bool Writer::Write(const BYTE* data, int size)
{
    if (m_p + size > m_p2)
        return false;
    memcpy(m_p, data, size);
    m_p += size;
    return true;
}

bool Writer::SelfTest()
{
    const char* HEAD_REC_INFO = "\xAB\xCD\x05\x00\x0C\x01\x00\x12\x00";
    const int bufsize = 9;
    BYTE cmdbuf[bufsize];
    Writer wr(cmdbuf, bufsize);
    CHKW(wr.Write((const BYTE*) HEAD_REC_INFO, 5))
    WORD index = 1;
    CHKW(wr.Write(index))
    index += 0x11;
    CHKW(wr.Write(index))
    dumpBin("Command", cmdbuf, bufsize, true);
    return true;
}
