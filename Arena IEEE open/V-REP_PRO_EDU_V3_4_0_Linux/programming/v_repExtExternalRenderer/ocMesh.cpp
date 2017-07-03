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

#include "ocMesh.h"
#include <qopengl.h>

#define MESH_INIT_USED_COUNT 10

COcMesh::COcMesh(int id,float* vert,int vertL,int* ind,int indL,float* norm,int normL,float* tex,int texL,unsigned char* ed)
{
    _id=id;
    _vertices.assign(vert,vert+vertL);
    _indices.assign(ind,ind+indL);
    _normals.assign(norm,norm+normL);
    if (tex!=NULL)
        _textureCoords.assign(tex,tex+texL);
    _usedCount=MESH_INIT_USED_COUNT;

    for (int i=0;i<indL/3;i++)
    { // for each triangle...
        int indOr[3]={3*_indices[3*i+0],3*_indices[3*i+1],3*_indices[3*i+2]};
        const float* vertices0=&_vertices[indOr[0]];
        const float* vertices1=&_vertices[indOr[1]];
        const float* vertices2=&_vertices[indOr[2]];
        if ( ( (ed[(3*i+0)>>3]&(1<<((3*i+0)&7)))!=0) ) // -1 means the edge was disabled
        {
            _edges.push_back(vertices0[0]);
            _edges.push_back(vertices0[1]);
            _edges.push_back(vertices0[2]);
            _edges.push_back(vertices1[0]);
            _edges.push_back(vertices1[1]);
            _edges.push_back(vertices1[2]);
        }
        if ( ( (ed[(3*i+1)>>3]&(1<<((3*i+1)&7)))!=0) ) // -1 means the edge was disabled
        {
            _edges.push_back(vertices1[0]);
            _edges.push_back(vertices1[1]);
            _edges.push_back(vertices1[2]);
            _edges.push_back(vertices2[0]);
            _edges.push_back(vertices2[1]);
            _edges.push_back(vertices2[2]);
        }
        if ( ( (ed[(3*i+2)>>3]&(1<<((3*i+2)&7)))!=0) ) // -1 means the edge was disabled
        {
            _edges.push_back(vertices2[0]);
            _edges.push_back(vertices2[1]);
            _edges.push_back(vertices2[2]);
            _edges.push_back(vertices0[0]);
            _edges.push_back(vertices0[1]);
            _edges.push_back(vertices0[2]);
        }
    }

}

COcMesh::~COcMesh()
{
}

void COcMesh::decrementUsedCount()
{
    _usedCount--;
}

int COcMesh::getUsedCount()
{
    return(_usedCount);
}

int COcMesh::getId()
{
    return(_id);
}

void COcMesh::render(const C7Vector& tr,const float* colors,bool textured,float shadingAngle,bool translucid,float opacityFactor,bool backfaceCulling,bool repeatU,bool repeatV,bool interpolateColors,int applyMode,COcTexture* texture,bool visibleEdges)
{
    _usedCount=MESH_INIT_USED_COUNT;
    glPushMatrix();
    glPushAttrib(GL_POLYGON_BIT);

    glTranslatef(tr.X(0),tr.X(1),tr.X(2));
    C4Vector axis=tr.Q.getAngleAndAxisNoChecking();
    glRotatef(axis(0)*radToDeg,axis(1),axis(2),axis(3));


    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,48);
    float ambientDiffuse[4]={colors[0],colors[1],colors[2],opacityFactor};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,ambientDiffuse);
    float specular[4]={colors[6],colors[7],colors[8],1.0f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    float emission[4]={colors[9],colors[10],colors[11],1.0f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emission);

    if (translucid)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }

    if (backfaceCulling)
        glEnable(GL_CULL_FACE);

    glPolygonOffset(0.5f,0.0f);
    glEnable(GL_POLYGON_OFFSET_FILL);

    if (textured&&(_textureCoords.size()!=0)&&(texture!=0))
    {
        texture->startTexture(repeatU,repeatV,interpolateColors,applyMode);

        glBegin(GL_TRIANGLES);
        for (int i=0;i<int(_indices.size());i++)
        {
            glNormal3fv(&_normals[0]+3*i);
            glTexCoord2fv(&_textureCoords[0]+2*i);
            glVertex3fv(&_vertices[0]+3*_indices[i]);
        }
        glEnd();

        texture->endTexture();
    }
    else
    {
        glBegin(GL_TRIANGLES);
        for (int i=0;i<int(_indices.size());i++)
        {
            glNormal3fv(&_normals[0]+3*i);
            glVertex3fv(&_vertices[0]+3*_indices[i]);
        }
        glEnd();
    }

    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    // Now the edges:
    if (visibleEdges)
    {
        ambientDiffuse[0]=0.0f;
        ambientDiffuse[1]=0.0f;
        ambientDiffuse[2]=0.0f;
        ambientDiffuse[3]=1.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,ambientDiffuse);
        specular[0]=0.0f;
        specular[1]=0.0f;
        specular[2]=0.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
        emission[0]=0.0f;
        emission[1]=0.0f;
        emission[2]=0.0f;
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,emission);

        glBegin(GL_LINES);
        for (int i=0;i<int(_edges.size())/3;i++)
            glVertex3fv(&_edges[3*i]);
        glEnd();
    }

    glPopAttrib();
    glPopMatrix();
}

