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

#include "cp211x.h"
#include "packet.h"

namespace UT181A {

class Device
{
public:
    Device(LPCSTR serial=NULL);
    bool Open();
    void Close();

    bool SendPacket(Packet& packet);
    int ReadPacket(BYTE* buffer, int size);

    bool StartMonitor();
    bool StopMonitor();

    bool Monitor(int loop);

    WORD GetRecordCount();
    DWORD ShowRecordInfo(WORD index);
    int ListRecord();
    bool ReceiveRecord(WORD index);

public:
    void Test();

private:
    CP211x m_tx;
    LPCSTR m_serial;
};

} //namespace UT181A
