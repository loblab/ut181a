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
#include <stdlib.h>
#include <string.h>
#include "debug.h"

int g_debug;

void dumpBin(const char* title, const void* pData, int len, bool outText)
{
    if (0 == len)
        return;

    const unsigned char* pb = (const unsigned char*)pData;
    const char* pc = (const char*)pData;
    char* buf = new char[len * 4 + 64];
    char* pOut = buf;
    sprintf(pOut, "%3d - ", len);
    pOut += strlen(pOut);
    if (outText)
    {
        for (int i = 0; i<len; i++)
        {
            char ch = *pc;
            if (ch < 32 || ch > 126)
                ch = '.';
            sprintf(pOut, "%c", ch);
            pOut += 1;
            pc++;
        }
        sprintf(pOut, " - ");
        pOut += 3;
    }
    for (int i = 0; i<len; i++)
    {
        sprintf(pOut, "%02X ", *pb);
        pOut += 3;
        pb++;
    }
    *(--pOut) = '\0'; //remove the last space
    printf("%s: %s\n", title, buf);
    delete[] buf;
}


