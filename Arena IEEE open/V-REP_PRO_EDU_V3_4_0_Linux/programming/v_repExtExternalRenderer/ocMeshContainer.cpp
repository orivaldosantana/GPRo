// This file is part of the EXTERNAL RENDERER PLUGIN for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The EXTERNAL RENDERER PLUGIN is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// The EXTERNAL RENDERER PLUGIN is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// THE EXTERNAL RENDERER PLUGIN IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
//  
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with the EXTERNAL RENDERER PLUGIN.  If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#include "ocMeshContainer.h"

COcMeshContainer::COcMeshContainer()
{
}

COcMeshContainer::~COcMeshContainer()
{
    for (int i=0;i<int(_meshes.size());i++)
        delete _meshes[i];
}

void COcMeshContainer::decrementAllUsedCount()
{
    for (int i=0;i<int(_meshes.size());i++)
        _meshes[i]->decrementUsedCount();
}

void COcMeshContainer::removeAllUnused()
{
    for (int i=0;i<int(_meshes.size());i++)
    {
        if (_meshes[i]->getUsedCount()<=0)
        {
            delete _meshes[i];
            _meshes.erase(_meshes.begin()+i);
            i--; // reprocess this position
        }
    }
}

COcMesh* COcMeshContainer::getFromId(int id)
{
    for (int i=0;i<int(_meshes.size());i++)
    {
        if (_meshes[i]->getId()==id)
            return(_meshes[i]);
    }
    return(0);
}

void COcMeshContainer::add(COcMesh* mesh)
{
    _meshes.push_back(mesh);
}
