// This file is part of the EXTERNAL IK for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The EXTERNAL IK is licensed under the terms of EITHER:
//   1. EXTERNAL IK commercial license (contact us for details)
//   2. EXTERNAL IK educational license (see below)
// 
// EXTERNAL IK educational license:
// -------------------------------------------------------------------
// The EXTERNAL IK educational license applies only to EDUCATIONAL
// ENTITIES composed by following people and institutions:
// 
// 1. Hobbyists, students, teachers and professors
// 2. Schools and universities
// 
// EDUCATIONAL ENTITIES do NOT include companies, research institutions,
// non-profit organisations, foundations, etc.
// 
// An EDUCATIONAL ENTITY may use, modify, compile and distribute the
// modified/unmodified EXTERNAL IK under following conditions:
//  
// 1. Distribution should be free of charge.
// 2. Distribution should be to EDUCATIONAL ENTITIES only.
// 3. Usage should be non-commercial.
// 4. Altered source versions must be plainly marked as such and distributed
//    along with any compiled code.
// 5. The origin of the EXTERNAL IK must not be misrepresented. you must
//    not claim that you wrote the original software.
// 
// THE EXTERNAL IK IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "Ct.h"

std::vector<Ct*> Ct::_allCts;
std::vector<int> Ct::_allCtHandles;
Ct* Ct::ct=NULL;
int Ct::currentHandle=0;
int Ct::_nextHandle=1;

int Ct::insertNewInstance(Ct* inst)
{
    _allCts.push_back(inst);
    _allCtHandles.push_back(_nextHandle);
    ct=inst;
    currentHandle=_nextHandle;
    _nextHandle++;
    return(_nextHandle-1);
}

int Ct::removeInstance(int handle)
{
    for (int i=0;i<int(_allCtHandles.size());i++)
    {
        if (_allCtHandles[i]==handle)
        {
            _allCtHandles.erase(_allCtHandles.begin()+i);
            _allCts.erase(_allCts.begin()+i);
            if (currentHandle==handle)
            {
                currentHandle=0;
                ct=NULL;
                if (_allCtHandles.size()>0)
                {
                    currentHandle=_allCtHandles[0];
                    ct=_allCts[0];
                }
                return(currentHandle);
            }
        }
    }
    return(-1); // error, instance doesn't exist
}

bool Ct::switchToInstance(int handle)
{
    for (int i=0;i<int(_allCtHandles.size());i++)
    {
        if (_allCtHandles[i]==handle)
        {
            currentHandle=handle;
            ct=_allCts[i];
            return(true);
        }
    }
    return(false); // error, instance doesn't exist
}

Ct::Ct()
{
    currentInstanceIndex=-1;
    ikGroups=NULL;
    objCont=NULL;
    createSingletonsAndOneInstance(); // This has to be done at the very beginning!
}

Ct::~Ct()
{
    destroySingletonsAndAllInstances(); // This has to be done at the very end
}

int Ct::createNewInstance(bool triggerCallbackAndHandleDialogs)
{
    ikGroupList.push_back(NULL);
    objContList.push_back(NULL);

    currentInstanceIndex=int(objContList.size())-1;


    ikGroups=NULL;
    objCont=NULL;

    ikGroups=new CRegisterediks();
    objCont=new CObjCont();


    ikGroupList[currentInstanceIndex]=ikGroups;
    objContList[currentInstanceIndex]=objCont;


    return(currentInstanceIndex);
}

int Ct::destroyCurrentInstance()
{
    if (currentInstanceIndex==-1)
        return(-1);


    // Important to empty objects first (since ct::objCont->announce....willBeErase
    // might be called for already destroyed objects!)

    ikGroups->removeAllIkGroups();


    objCont->removeAllObjects();




    delete objCont;
    objCont=NULL;


    delete ikGroups;
    ikGroups=NULL;



    objContList.erase(objContList.begin()+currentInstanceIndex);
    ikGroupList.erase(ikGroupList.begin()+currentInstanceIndex);

    if (objContList.size()!=0)
    {
        currentInstanceIndex=-1; // We set this so that next instruction succeeds (otherwise we might already be in instance 0 and the function will not process). Added on 5/3/2012
        makeInstanceCurrentFromIndex(int(objContList.size())-1,true);
    }
    else
        currentInstanceIndex=-1;
    return(currentInstanceIndex);   
}

void Ct::createSingletonsAndOneInstance()
{
    createNewInstance(true);
}

void Ct::destroySingletonsAndAllInstances()
{

    while (objContList.size()!=0)
    {
        makeInstanceCurrentFromIndex(int(objContList.size())-1,true);
        destroyCurrentInstance();
    }

}

void Ct::emptyScene(bool notCalledFromUndoFunction)
{

    // Following is saved:

    ikGroups->removeAllIkGroups();


    objCont->newSceneProcedure();

}

bool Ct::makeInstanceCurrentFromIndex(int instanceIndex,bool triggerCallbackAndHandleDialogs)
{ 
    if ( (instanceIndex<0)||(instanceIndex>=int(objContList.size())))
        return(false);

    if (currentInstanceIndex==instanceIndex)
        return(true);

    currentInstanceIndex=instanceIndex;

    ikGroups=ikGroupList[currentInstanceIndex];
    objCont=objContList[currentInstanceIndex];


    return(true);
}
