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

#include "inputOutputBlock.h"

#define OUTPUT_BLOCK_SIZE 1024

CInputOutputBlock::CInputOutputBlock(int opcode,const char* rcsHandle)
{ // Input block
    pushBackInteger(0); // will be completed when retrieving the data pointer
    pushBackInteger(OUTPUT_BLOCK_SIZE);
    pushBackInteger(opcode);
    if (rcsHandle!=NULL)
        pushBackBuffer(rcsHandle,8);
    _readOffset=0;
}

CInputOutputBlock::CInputOutputBlock(unsigned char* data,int dataSize)
{ // Output block
    _data.assign(data,data+dataSize);
    _readOffset=0;
}

CInputOutputBlock::~CInputOutputBlock()
{
}

void* CInputOutputBlock::getDataPointer(int* blockSize)
{
    if (_tempData.size()!=0)
    { // we have an input block
        _data.clear();
        _data.assign(_tempData.begin(),_tempData.end());
        int off=int(_tempData.size());
        _data.insert(_data.end(),_tempStringData.begin(),_tempStringData.end());
        for (int i=0;i<int(_tempStringOffsetOffsets.size());i++)
        {
            int o=_tempStringOffsetOffsets[i];
            int fullOffset=((int*)(&_data[o]))[0]+off;
            const char* c=(char*)(&fullOffset);
            _data[o+0]=c[0];
            _data[o+1]=c[1];
            _data[o+2]=c[2];
            _data[o+3]=c[3];
        }

        int inputDataLength=(int)_data.size();
        const char* c=(char*)(&inputDataLength);
        _data[0]=c[0];
        _data[1]=c[1];
        _data[2]=c[2];
        _data[3]=c[3];

        _tempData.clear();
        _tempStringOffsetOffsets.clear();
        _tempStringData.clear();
    }
    if (blockSize!=NULL)
        blockSize[0]=int(_data.size());
/*
    printf("DataSize: %i\n",int(_data.size()));
    int dataLeft=int(_data.size());
    int off=0;
    while (true)
    {
        for (int i=0;i<10;i++)
        {
            printf("%03d, ",_data[off]);
            dataLeft--;
            off++;
            if (dataLeft==0)
                break;
        }
        printf("\n");
        if (dataLeft==0)
            break;
    }
*/
    return(&_data[0]);
}

void CInputOutputBlock::pushBackReal(double doubleData)
{
    const char* c=(char*)(&doubleData);
    _tempData.push_back(c[0]);
    _tempData.push_back(c[1]);
    _tempData.push_back(c[2]);
    _tempData.push_back(c[3]);
    _tempData.push_back(c[4]);
    _tempData.push_back(c[5]);
    _tempData.push_back(c[6]);
    _tempData.push_back(c[7]);
}

void CInputOutputBlock::pushBackInteger(int intData)
{
    const char* c=(char*)(&intData);
    _tempData.push_back(c[0]);
    _tempData.push_back(c[1]);
    _tempData.push_back(c[2]);
    _tempData.push_back(c[3]);
}

void CInputOutputBlock::pushBackBuffer(const char* buffer,int bufferSize)
{
    for (int i=0;i<bufferSize;i++)
        _tempData.push_back(buffer[i]);
}

void CInputOutputBlock::pushBackString(const std::string& string)
{
    _tempStringOffsetOffsets.push_back(int(_tempData.size()));
    pushBackInteger(int(_tempStringData.size())); 
    for (unsigned int i=0;i<string.size();i++)
        _tempStringData.push_back(string[i]);
    _tempStringData.push_back(0);
}

void CInputOutputBlock::pushBackBitstring(const std::string& bitstring4)
{
    pushBackBuffer(&bitstring4[0],BITSTRING_SIZE);
}

void CInputOutputBlock::pushBackBitstring2(const std::string& bitstring8)
{
    pushBackBuffer(&bitstring8[0], BITSTRING2_SIZE);
}

void CInputOutputBlock::pushBackCartPos(const std::string& cartPos388)
{
    pushBackBuffer(&cartPos388[0], CARTPOS_SIZE);
}

void CInputOutputBlock::pushBackFrame(const std::string& frame384)
{
    pushBackBuffer(&frame384[0], FRAME_SIZE);
}

void CInputOutputBlock::pushBackJointPos(const std::string& jointPos1032)
{
    pushBackBuffer(&jointPos1032[0], JOINTPOS_SIZE);
}

unsigned char CInputOutputBlock::readByte()
{
    unsigned char retVal=_data[_readOffset];
    _readOffset++;
    return(retVal);
}

int CInputOutputBlock::readInteger()
{
    int retVal=((int*)(&_data[_readOffset]))[0];
    _readOffset+=4;
    return(retVal);
}

double CInputOutputBlock::readReal()
{
    double retVal=((double*)(&_data[_readOffset]))[0];
    _readOffset+=8;
    return(retVal);
}

std::string CInputOutputBlock::readString()
{
    int offset=((int*)(&_data[_readOffset]))[0];
    _readOffset+=4;
    std::string retVal((char*)(&(_data[0])+offset));
    return(retVal);
}

std::string CInputOutputBlock::readBuffer(int bufferSize)
{
    std::string retVal;
    for (unsigned int i=0;i<(unsigned int)bufferSize;i++)
        retVal+=_data[_readOffset++];
    return(retVal);
}

std::string CInputOutputBlock::readBitstring()
{
    std::string retVal((char*)&_data[_readOffset],BITSTRING_SIZE);
    _readOffset+=BITSTRING_SIZE;
    return(retVal);
}

std::string CInputOutputBlock::readBitstring2()
{
    std::string retVal((char*)&_data[_readOffset], BITSTRING2_SIZE);
    _readOffset+=BITSTRING2_SIZE;
    return(retVal);
}

std::string CInputOutputBlock::readCartPos()
{
    std::string retVal((char*)&_data[_readOffset],CARTPOS_SIZE);
    _readOffset+=CARTPOS_SIZE;
    return(retVal);
}

std::string CInputOutputBlock::readFrame()
{
    std::string retVal((char*)&_data[_readOffset], FRAME_SIZE);
    _readOffset+=FRAME_SIZE;
    return(retVal);
}

std::string CInputOutputBlock::readJointPos()
{
    std::string retVal((char*)&_data[_readOffset], JOINTPOS_SIZE);
    _readOffset+=JOINTPOS_SIZE;
    return(retVal);
}
