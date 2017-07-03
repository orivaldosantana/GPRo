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

#pragma once

#include <vector>

class CVisionVelodyneHDL64E  
{
public:
    CVisionVelodyneHDL64E(const int visionSensorHandles[4],float frequency,int options,float pointSize,float _coloringDistances[2],float scalingFactor,int newPointCloudHandle);
    virtual ~CVisionVelodyneHDL64E();

    int getVelodyneHandle();
    bool areVisionSensorsExplicitelyHandled();
    bool doAllObjectsExistAndAreVisionSensors();
    bool handle(float dt,std::vector<float>& pts);

private:
    void _removePointsBetween(float lowAngle,float range);
    void _getColorFromIntensity(float intensity,unsigned char col[3]);

    int _visionSensorHandles[4];
    float _frequency;
    int _velodyneHandle;
    float _displayScalingFactor;
    float _pointSize;
    bool _displayPts;
    bool _emissivePts;
    bool _displayOnlyCurrent;
    bool _cartesianCoords;
    int _ptCloudHandle;
    int _newPtCloudHandle;
    float lastScanAngle;
    float _coloringDistances[2];
    std::vector<float> _displayPtsXyz;
    std::vector<float> _displayPtsA;
    std::vector<unsigned char> _displayPtsCol;

    static int _nextVelodyneHandle;
};
