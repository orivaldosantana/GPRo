// This file is part of the RRS-1 interface for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// This interface is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// This interface is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THIS INTERFACE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with the REMOTE API.  If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#pragma once

#include <vector>
#include <string>

#define BITSTRING_SIZE 4
#define BITSTRING2_SIZE 8
#define CARTPOS_SIZE 100
#define FRAME_SIZE 96
#define JOINTPOS_SIZE 264

class CInputOutputBlock
{
public:
    CInputOutputBlock(int opcode,const char* rcsHandle); // Input block
    CInputOutputBlock(unsigned char* data,int dataSize); // Output block
    virtual ~CInputOutputBlock();

    // When block is INPUT:
    void pushBackReal(double doubleData);
    void pushBackInteger(int intData);
    void pushBackString(const std::string& string);
    void pushBackBuffer(const char* buffer,int bufferSize);
    void pushBackBitstring(const std::string& bitstring4);
    void pushBackBitstring2(const std::string& bitstring8);
    void pushBackCartPos(const std::string& cartPos388);
    void pushBackFrame(const std::string& frame384);
    void pushBackJointPos(const std::string& jointPos1032);

    void* getDataPointer(int* blockSize);

    // When block is OUTPUT:
    unsigned char readByte();
    int readInteger();
    double readReal();
    std::string readString();
    std::string readBuffer(int bufferSize);
    std::string readBitstring();
    std::string readBitstring2();
    std::string readCartPos();
    std::string readFrame();
    std::string readJointPos();

protected:
    std::vector<unsigned char> _data;

    std::vector<unsigned char> _tempData;
    std::vector<int> _tempStringOffsetOffsets;
    std::vector<unsigned char> _tempStringData;

    int _readOffset;
};
