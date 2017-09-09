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

#define CHKW(c) \
if (!c) \
{ \
    printf("Failed to write at line %d of %s\n", __LINE__, __FILE__); \
    return false; \
}

class Writer
{
public:
    Writer(void* buffer, int size);

    bool Skip(int size);
    bool Write(char val);
    bool Write(BYTE val);
    bool Write(short val);
    bool Write(WORD val);
    bool Write(int val);
    bool Write(DWORD val);
    bool Write(float val);
    bool Write(const char* str, int size);
    bool Write(const BYTE* data, int size);

protected:
    BYTE* m_p1;
    BYTE* m_p2;
    BYTE* m_p;

public:
    static bool SelfTest();
};

