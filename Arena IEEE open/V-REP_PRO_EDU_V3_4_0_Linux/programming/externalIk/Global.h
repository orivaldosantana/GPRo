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


enum { // VERY CAREFUL, FOLLOWING VALUES ARE SERIALIZED!!
        SIM_IDSTART_3DOBJECT            =0,
        SIM_IDSTART_COLLECTION_OLD      =100000,
        SIM_IDEND_3DOBJECT              =1999999,
        SIM_IDSTART_COLLECTION          =2000000,
        SIM_IDEND_COLLECTION            =SIM_IDSTART_COLLECTION+10000,
        SIM_IDSTART_COLLISION           =SIM_IDEND_COLLECTION+1,
        SIM_IDEND_COLLISION             =SIM_IDSTART_COLLISION+10000,
        SIM_IDSTART_DISTANCE            =SIM_IDEND_COLLISION+1,
        SIM_IDEND_DISTANCE              =SIM_IDSTART_DISTANCE+10000,
        SIM_IDSTART_IKGROUP             =SIM_IDEND_DISTANCE+1,
        SIM_IDEND_IKGROUP               =SIM_IDSTART_IKGROUP+10000,
        SIM_IDSTART_GCSOBJECT           =SIM_IDEND_IKGROUP+1,
        SIM_IDEND_GCSOBJECT             =SIM_IDSTART_GCSOBJECT+10000,
        SIM_IDSTART_LUASCRIPT           =SIM_IDEND_GCSOBJECT+1,
        SIM_IDEND_LUASCRIPT             =SIM_IDSTART_LUASCRIPT+10000,
        SIM_IDSTART_2DELEMENT           =SIM_IDEND_LUASCRIPT+1,
        SIM_IDEND_2DELEMENT             =SIM_IDSTART_2DELEMENT+10000,
        SIM_IDSTART_PATHPLANNINGTASK    =SIM_IDEND_2DELEMENT+1,
        SIM_IDEND_PATHPLANNINGTASK      =SIM_IDSTART_PATHPLANNINGTASK+10000,
        SIM_IDSTART_GEOMETRIC_OLD       =SIM_IDEND_PATHPLANNINGTASK+1,
        SIM_IDEND_GEOMETRIC_OLD         =SIM_IDSTART_GEOMETRIC_OLD+100000,
        SIM_IDSTART_TEXTURE             =SIM_IDEND_GEOMETRIC_OLD+1,
        SIM_IDEND_TEXTURE               =SIM_IDSTART_TEXTURE+100000,
        SIM_IDSTART_MOTIONPLANNINGTASK  =SIM_IDEND_TEXTURE+1,
        SIM_IDEND_MOTIONPLANNINGTASK    =SIM_IDSTART_MOTIONPLANNINGTASK+10000,
};
