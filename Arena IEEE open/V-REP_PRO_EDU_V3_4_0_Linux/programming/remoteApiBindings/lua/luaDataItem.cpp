// This file is part of the REMOTE API
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The REMOTE API is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// The REMOTE API is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THE REMOTE API IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
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

#include "luaDataItem.h"

CLuaDataItem::CLuaDataItem()
{
    _nilTableSize=0;
    _isTable=false;
    _type=-1; // nil
}

CLuaDataItem::CLuaDataItem(bool v)
{
    _nilTableSize=0;
    _isTable=false;
    _type=0;
    boolData.push_back(v);
}

CLuaDataItem::CLuaDataItem(int v)
{
    _nilTableSize=0;
    _isTable=false;
    _type=1;
    intData.push_back(v);
}

CLuaDataItem::CLuaDataItem(float v)
{
    _nilTableSize=0;
    _isTable=false;
    _type=2;
    floatData.push_back(v);
}

CLuaDataItem::CLuaDataItem(double v)
{
    _nilTableSize=0;
    _isTable=false;
    _type=5;
    doubleData.push_back(v);
}

CLuaDataItem::CLuaDataItem(const std::string& v)
{
    _nilTableSize=0;
    _isTable=false;
    _type=3;
    stringData.push_back(v);
}

CLuaDataItem::CLuaDataItem(const char* bufferPtr,unsigned int bufferLength)
{
    _nilTableSize=0;
    _isTable=false;
    _type=4;
    std::string v(bufferPtr,bufferLength);
    stringData.push_back(v);
}

CLuaDataItem::CLuaDataItem(const std::vector<bool>& v)
{
    _nilTableSize=0;
    _isTable=true;
    _type=0;
    boolData.assign(v.begin(),v.end());
}

CLuaDataItem::CLuaDataItem(const std::vector<int>& v)
{
    _nilTableSize=0;
    _isTable=true;
    _type=1;
    intData.assign(v.begin(),v.end());
}

CLuaDataItem::CLuaDataItem(const std::vector<float>& v)
{
    _nilTableSize=0;
    _isTable=true;
    _type=2;
    floatData.assign(v.begin(),v.end());
}

CLuaDataItem::CLuaDataItem(const std::vector<double>& v)
{
    _nilTableSize=0;
    _isTable=true;
    _type=5;
    doubleData.assign(v.begin(),v.end());
}

CLuaDataItem::CLuaDataItem(const std::vector<std::string>& v)
{
    _nilTableSize=0;
    _isTable=true;
    _type=3;
    stringData.assign(v.begin(),v.end());
}

CLuaDataItem::~CLuaDataItem()
{
}

bool CLuaDataItem::isTable()
{
    return(_isTable);
}

int CLuaDataItem::getType()
{
    return(_type);
}

void CLuaDataItem::setNilTable(int size)
{
    if (_type==-1)
    {
        _isTable=true;
        _nilTableSize=size;
    }
}

int CLuaDataItem::getNilTableSize()
{
    return(_nilTableSize);
}
