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

#include "Types.h"

#define CHKR(c) \
if (!c) \
{ \
    fprintf(stderr, "Failed to read at line %d of %s\n", __LINE__, __FILE__); \
    return false; \
}

class Reader
{
public:
    Reader(const void* buffer, int size);

    bool Skip(int size);
    bool Read(char& val);
    bool Read(BYTE& val);
    bool Read(short& val);
    bool Read(WORD& val);
    bool Read(int& val);
    bool Read(DWORD& val);
    bool Read(float& val);
    bool Read(char* str, int size);
    bool Read(BYTE* data, int size);

protected:
    const BYTE* m_p1;
    const BYTE* m_p2;
    const BYTE* m_p;

public:
    static void SelfTest();
};

