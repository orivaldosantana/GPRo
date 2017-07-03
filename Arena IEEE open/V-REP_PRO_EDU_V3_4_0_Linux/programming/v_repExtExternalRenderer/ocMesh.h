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

#ifndef OCMESH_H
#define OCMESH_H

#include <vector>
#include "7Vector.h"
#include "ocTexture.h"

class COcMesh
{
public:
    COcMesh(int id,float* vert,int vertL,int* ind,int indL,float* norm,int normL,float* tex,int texL,unsigned char* ed);
    ~COcMesh();

    void render(const C7Vector& tr,const float* colors,bool textured,float shadingAngle,bool translucid,float opacityFactor,bool backfaceCulling,bool repeatU,bool repeatV,bool interpolateColors,int applyMode,COcTexture* texture,bool visibleEdges);
    void decrementUsedCount();
    int getUsedCount();
    int getId();

protected:
    std::vector<float> _vertices;
    std::vector<int> _indices;
    std::vector<float> _normals;
    std::vector<float> _textureCoords;
    std::vector<float> _edges;
    int _usedCount;
    int _id;
};
#endif // OCMESH_H
