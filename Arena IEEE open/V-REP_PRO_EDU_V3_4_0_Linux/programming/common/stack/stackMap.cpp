// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "stackMap.h"
#include "stackArray.h"
#include "stackNull.h"
#include "stackNumber.h"
#include "stackBool.h"
#include "stackString.h"

CStackMap::CStackMap()
{
    _objectType=STACK_MAP;
}

CStackMap::~CStackMap()
{
    for (std::map<std::string,CStackObject*>::iterator it=_objectValues.begin();it!=_objectValues.end();it++)
        delete it->second;
}

void CStackMap::_remove(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
    {
        delete it->second;
        _objectValues.erase(it);
    }
}

void CStackMap::appendTopStackItem(const char* key,int stackId)
{ // this also clears the item from the stack
    _remove(key);
    _objectValues[key]=CStackObject::buildItemFromTopStackPosition(stackId);
}

std::map<std::string,CStackObject*>* CStackMap::getKeyValuePairs()
{
    return(&_objectValues);
}

bool CStackMap::isKeyPresent(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    return(it!=_objectValues.end());
}

bool CStackMap::isNull(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
        return(it->second->getObjectType()==STACK_NULL);
    return(false);
}

bool CStackMap::isBool(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
        return(it->second->getObjectType()==STACK_BOOL);
    return(false);
}

bool CStackMap::isNumber(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
        return(it->second->getObjectType()==STACK_NUMBER);
    return(false);
}

bool CStackMap::isString(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
        return(it->second->getObjectType()==STACK_STRING);
    return(false);
}

bool CStackMap::isArray(const char* key,size_t minSize/*=0*/)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
    {
        if (it->second->getObjectType()==STACK_ARRAY)
            return(((CStackArray*)it->second)->getSize()>=minSize);
    }
    return(false);
}

bool CStackMap::isMap(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if (it!=_objectValues.end())
        return(it->second->getObjectType()==STACK_MAP);
    return(false);
}

bool CStackMap::getBool(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_BOOL) )
        return(((CStackBool*)it->second)->getValue());
    return(false);
}

float CStackMap::getFloat(const char* key)
{
    return((float)getDouble(key));
}

double CStackMap::getDouble(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_NUMBER) )
        return(((CStackNumber*)it->second)->getValue());
    return(0.0);
}

int CStackMap::getInt(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_NUMBER) )
        return(int(((CStackNumber*)it->second)->getValue()));
    return(0);
}

std::string CStackMap::getString(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_STRING) )
        return(((CStackString*)it->second)->getValue());
    return("");
}

CStackArray* CStackMap::getArray(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_ARRAY) )
        return((CStackArray*)it->second);
    return(NULL);
}

CStackMap* CStackMap::getMap(const char* key)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==STACK_MAP) )
        return((CStackMap*)it->second);
    return(NULL);
}

bool CStackMap::contains(const char* key,int theType,size_t theMinSizeIfArray/*=0*/,bool onlyNumbersInArray/*=false*/)
{
    std::map<std::string,CStackObject*>::iterator it=_objectValues.find(key);
    if ( (it!=_objectValues.end())&&(it->second->getObjectType()==theType) )
    {
        if (theType!=STACK_ARRAY)
            return(true);
        CStackArray* arr=(CStackArray*)it->second;
        if (arr->getSize()>=theMinSizeIfArray)
            return((!onlyNumbersInArray)||arr->isNumberArray());
    }
    return(false);
}

void CStackMap::setNull(const char* key)
{
    _remove(key);
    _objectValues[key]=new CStackNull();
}

void CStackMap::setBool(const char* key,bool d)
{
    _remove(key);
    _objectValues[key]=new CStackBool(d);
}

void CStackMap::setFloat(const char* key,float d)
{
    setDouble(key,(double)d);
}

void CStackMap::setDouble(const char* key,double d)
{
    _remove(key);
    _objectValues[key]=new CStackNumber(d);
}

void CStackMap::setInt(const char* key,int d)
{
    _remove(key);
    _objectValues[key]=new CStackNumber(double(d));
}

void CStackMap::setString(const char* key,const std::string& d)
{
    _remove(key);
    _objectValues[key]=new CStackString(d.c_str(),int(d.length()));
}

void CStackMap::setArray(const char* key,CStackArray* arr)
{
    _remove(key);
    _objectValues[key]=arr;
}

void CStackMap::setMap(const char* key,CStackMap* map)
{
    _remove(key);
    _objectValues[key]=map;
}

CStackObject* CStackMap::copyYourself()
{
    CStackMap* retVal=new CStackMap();
    for (std::map<std::string,CStackObject*>::iterator it=_objectValues.begin();it!=_objectValues.end();it++)
        retVal->_objectValues[it->first]=it->second->copyYourself();
    return(retVal);
}

