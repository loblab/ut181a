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
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <signal.h>
#include <unistd.h>

#include "ut181a.h"
#include "reader.h"
#include "writer.h"
#include "debug.h"

using namespace std;

void SigintHandler(int param)
{
    exit(0);
}


int test1()
{
    float data[] = {
        2.5104,
        4.5985,
        5.0244,
        5.4943,
        5.4944,
        6.002,
        6.003,
    };
    dumpBin("Float", data, sizeof(data), false);
    return 0;
}

int test2()
{
    Reader::SelfTest();
    return 0;
}

int test3()
{
    Writer::SelfTest();
    return 0;
}

int test4()
{
    UT181A::DateTime dt;
    memcpy(&dt, "\x11\x79\x4B\xD3", 4);
    dt.Show();
    memcpy(&dt, "\x51\x05\x00\xD0", 4);
    dt.Show();
    return 0;
}

int main(int argc, char* argv[])
{
    // Register for SIGINT events
    signal(SIGINT, SigintHandler);

    UT181A::Device dmm;
    if (!dmm.Open())
    {
        cerr << "Failed to open UT181A DMM. Please check device connection." << endl;
        return -1;
    }
    
    //dmm.Monitor(10);
    int total = dmm.ListRecord();
    if (total > 0)
        dmm.ReceiveRecord(total);

    dmm.Close();
    return 0;
}

