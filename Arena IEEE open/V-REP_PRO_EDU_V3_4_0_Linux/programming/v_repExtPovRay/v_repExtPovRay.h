// This file is part of the POV RAY PLUGIN for V-REP
// 
// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The POV RAY PLUGIN was written by Andreu Vidal on behalf of Coppelia Robotics GmbH.
// 
// The POV RAY PLUGIN is licensed under the same terms as the POV Ray
// library: GNU Affero General Public License. You should have received
// a copy of the GNU Affero General Public License along with the
// POV RAY PLUGIN.  If not, see <http://www.gnu.org/licenses/agpl-3.0.en.html>.
// 

//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

#ifndef V_REPEXTRAYTRACER_H
#define V_REPEXTRAYTRACER_H

#ifdef _WIN32
    #define VREP_DLLEXPORT extern "C" __declspec(dllexport)
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #define VREP_DLLEXPORT extern "C"
#endif /* __linux || __APPLE__ */

// The 3 required entry points of the plugin:
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt);
VREP_DLLEXPORT void v_repEnd();
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData);

VREP_DLLEXPORT void v_repPovRay(int message,void* data);

#endif // V_REPEXTRAYTRACER_H
