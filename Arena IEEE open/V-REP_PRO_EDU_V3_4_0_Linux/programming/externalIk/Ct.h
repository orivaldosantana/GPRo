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

#pragma once

#include "extIk.h"
#include "Registerediks.h"
#include "ObjCont.h"

class Ct  
{
public:
    Ct();
    virtual ~Ct();
    void createSingletonsAndOneInstance();
    void destroySingletonsAndAllInstances();

public:
    void emptyScene(bool notCalledFromUndoFunction);
    int createNewInstance(bool triggerCallbackAndHandleDialogs);
    bool makeInstanceCurrentFromIndex(int instanceIndex,bool triggerCallbackAndHandleDialogs);
    int destroyCurrentInstance();

private:    
    int currentInstanceIndex;

    static std::vector<Ct*> _allCts;
    static std::vector<int> _allCtHandles;
    static int _nextHandle;

public:
    CRegisterediks* ikGroups;
    CObjCont* objCont;

    std::vector<CRegisterediks*> ikGroupList;
    std::vector<CObjCont*> objContList;

    static Ct* ct;
    static int currentHandle;

    static int insertNewInstance(Ct* inst);
    static int removeInstance(int handle);
    static bool switchToInstance(int handle);
};

