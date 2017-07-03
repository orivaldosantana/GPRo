#include "v_repExtReflexxesTypeII.h"
#include "v_repLib.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <vector>

// Following required by the Reflexxes Motion Library:
#include <ReflexxesAPI.h>
#include <RMLPositionFlags.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>

#ifdef _WIN32
	#include <shlwapi.h>
	#pragma comment(lib, "Shlwapi.lib")
#endif /* _WIN32 */

#define PLUGIN_VERSION 1

struct SObj
{
	ReflexxesAPI* RML;
	RMLPositionInputParameters* PIP;
	RMLPositionOutputParameters* POP;
	RMLPositionFlags* PFlags;

	RMLVelocityInputParameters* VIP;
	RMLVelocityOutputParameters* VOP;
	RMLVelocityFlags* VFlags;

	bool destroyAtSimulationEnd;
	int objectHandle;
	int dofs;
	double smallestTimeStep;
};

int nextObjectHandle=0;

std::vector<SObj> allObjects;


LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind

//FILE* file=NULL;


VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{
	// 1. Figure out this plugin's directory:
	char curDirAndFile[1024];
#ifdef _WIN32
	GetModuleFileName(NULL,curDirAndFile,1023);
	PathRemoveFileSpec(curDirAndFile);
#elif defined (__linux) || defined (__APPLE__)
	getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif
	std::string currentDirAndPath(curDirAndFile);
	// 2. Append the V-REP library's name:
	std::string temp(currentDirAndPath);
#ifdef _WIN32
	temp+="\\v_rep.dll";
#elif defined (__linux)
	temp+="/libv_rep.so";
#elif defined (__APPLE__)
	temp+="/libv_rep.dylib";
#endif /* __linux || __APPLE__ */
	// 3. Load the V-REP library:
	vrepLib=loadVrepLibrary(temp.c_str());
	if (vrepLib==NULL)
	{
		std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'ReflexxesTypeII' plugin.\n";
		return(0); // Means error, V-REP will unload this plugin
	}
	if (getVrepProcAddresses(vrepLib)==0)
	{
		std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'ReflexxesTypeII' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}

	// Check the version of V-REP:
	int vrepVer;
	simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
	if (vrepVer<20606) // if V-REP version is smaller than 2.06.06
	{
		std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'ReflexxesTypeIV' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}

	// The constants:
	simRegisterCustomLuaVariable("simrml_phase_sync_if_possible",(boost::lexical_cast<std::string>(int(simrml_phase_sync_if_possible))).c_str());
	simRegisterCustomLuaVariable("simrml_only_time_sync",(boost::lexical_cast<std::string>(int(simrml_only_time_sync))).c_str());
	simRegisterCustomLuaVariable("simrml_only_phase_sync",(boost::lexical_cast<std::string>(int(simrml_only_phase_sync))).c_str());
	simRegisterCustomLuaVariable("simrml_no_sync",(boost::lexical_cast<std::string>(int(simrml_no_sync))).c_str());
	simRegisterCustomLuaVariable("simrml_disable_extremum_motion_states_calc",(boost::lexical_cast<std::string>(int(simrml_disable_extremum_motion_states_calc))).c_str());
	simRegisterCustomLuaVariable("simrml_keep_target_vel",(boost::lexical_cast<std::string>(int(simrml_keep_target_vel))).c_str());
	simRegisterCustomLuaVariable("simrml_recompute_trajectory",(boost::lexical_cast<std::string>(int(simrml_recompute_trajectory))).c_str());
	simRegisterCustomLuaVariable("simrml_keep_current_vel_if_fallback_strategy",(boost::lexical_cast<std::string>(int(simrml_keep_current_vel_if_fallback_strategy))).c_str());

//	file=fopen("RMLOutput.txt","wb");

	return(PLUGIN_VERSION); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
}

VREP_DLLEXPORT void v_repEnd()
{
//	fclose(file);

	unloadVrepLibrary(vrepLib); // release the library
}

VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{
	// Keep following 5 lines at the beginning and unchanged:
	int errorModeSaved;
	simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
	simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);
	void* retVal=NULL;

	if ((message==sim_message_eventcallback_rmlposition))//&&(auxiliaryData[0]!=0)) // if auxiliaryData[0] isn't 0, then we wanna use the type 4 lib!
	{ // the sim_message_eventcallback_rmlposition message is passed when the API function simRMLPosition is called from C/C++ or Lua
		ReflexxesAPI* RML=NULL;
		RMLPositionInputParameters* IP=NULL;
		RMLPositionOutputParameters* OP=NULL;
		RMLPositionFlags Flags;
		
		// All input parameters are coded in the data buffer! (i.e. first 4 bytes=DoFs, next 8 bytes=time step, etc.)
		// The values in the data buffer are in meters, not millimeters!
		char* data=(char*)customData;
		int dofs=((int*)(data+0))[0];
		double timeStep=((double*)(data+4))[0];
		int off=12;

		RML = new ReflexxesAPI(dofs,timeStep);
		IP = new RMLPositionInputParameters(dofs);
		OP = new RMLPositionOutputParameters(dofs);
 
		for (int i=0;i<dofs;i++)
			IP->CurrentPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->CurrentVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->CurrentAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			IP->MaxVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->MaxAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->MaxJerkVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			IP->SelectionVector->VecData[i]=(data[off+i]!=0);
		off+=dofs;

		for (int i=0;i<dofs;i++)
			IP->TargetPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->TargetVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		// Apply the flags:
		int flags=((int*)(data+off))[0];
		if (flags>=0)
		{ // we don't have default values!
			if ((flags&3)==simrml_phase_sync_if_possible)
				Flags.SynchronizationBehavior=RMLFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE; // default
			if ((flags&3)==simrml_only_time_sync)
				Flags.SynchronizationBehavior=RMLFlags::ONLY_TIME_SYNCHRONIZATION;
			if ((flags&3)==simrml_only_phase_sync)
				Flags.SynchronizationBehavior=RMLFlags::ONLY_PHASE_SYNCHRONIZATION;
			if ((flags&3)==simrml_no_sync)
				Flags.SynchronizationBehavior=RMLFlags::NO_SYNCHRONIZATION;

			if ((flags&4)==simrml_keep_target_vel)
				Flags.BehaviorAfterFinalStateOfMotionIsReached=RMLPositionFlags::KEEP_TARGET_VELOCITY; // default
			if ((flags&4)==simrml_recompute_trajectory)
				Flags.BehaviorAfterFinalStateOfMotionIsReached=RMLPositionFlags::RECOMPUTE_TRAJECTORY;

			if (flags&simrml_disable_extremum_motion_states_calc)
				Flags.EnableTheCalculationOfTheExtremumMotionStates=false;
			else
				Flags.EnableTheCalculationOfTheExtremumMotionStates=true; // default

			if (flags&simrml_keep_current_vel_if_fallback_strategy)
				Flags.KeepCurrentVelocityInCaseOfFallbackStrategy=true; 
			else
				Flags.KeepCurrentVelocityInCaseOfFallbackStrategy=false; // default
		}
		off+=4;
		
		// Check for extension data (not used for now):
		unsigned char extensionBytes=data[off];
		off+=1+extensionBytes;

//		if (file!=NULL)
//			IP->Echo(file);
		// Execute the Reflexxe function!
		replyData[0]=RML->RMLPosition(*IP,OP,Flags);
//		printf("ret: %i, sync. time: %f, alphaTime: %f\n",replyData[0],(float)OP->SynchronizationTime,(float)OP->AlphaTime);

		// Next to returning the function return value (just here above), we return a buffer with the new position, velocity and acceleration vector:
		// We also return 8 additional doubles for future extension (1 double is already in use)
		char* retBuff=simCreateBuffer(dofs*8*3+8*8);
		off=0;

		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewPositionVector->VecData[i]/1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewVelocityVector->VecData[i]/1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewAccelerationVector->VecData[i]/1000.0;
		off+=dofs*8;

		((double*)(retBuff+off))[0]=OP->SynchronizationTime;
//		((double*)(retBuff+off))[1]=OP->AlphaTime;
		off+=8*8;

		retVal=retBuff;
		
		delete RML;
		delete IP;
		delete OP;
	}

	if ((message==sim_message_eventcallback_rmlvelocity))//&&(auxiliaryData[0]!=0)) // if auxiliaryData[0] isn't 0, then we wanna use the type 4 lib!
	{ // the sim_message_eventcallback_rmlvelocity message is passed when the API function simRMLVelocity is called from C/C++ or Lua
		ReflexxesAPI* RML=NULL;
		RMLVelocityInputParameters* IP=NULL;
		RMLVelocityOutputParameters* OP=NULL;
		RMLVelocityFlags Flags;
		
		// All input parameters are coded in the data buffer! (i.e. first 4 bytes=DoFs, next 8 bytes=time step, etc.)
		// The values in the data buffer are in meters, not millimeters!
		char* data=(char*)customData;
		int dofs=((int*)(data+0))[0];
		double timeStep=((double*)(data+4))[0];
		int off=12;

		RML = new ReflexxesAPI(dofs,timeStep);
		IP = new RMLVelocityInputParameters(dofs);
		OP = new RMLVelocityOutputParameters(dofs);
 
		for (int i=0;i<dofs;i++)
			IP->CurrentPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->CurrentVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->CurrentAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			IP->MaxAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			IP->MaxJerkVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			IP->SelectionVector->VecData[i]=(data[off+i]!=0);
		off+=dofs;

		for (int i=0;i<dofs;i++)
			IP->TargetVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		// Apply the flags:
		int flags=((int*)(data+off))[0];
		if (flags>=0)
		{ // we don't have default values!
			if ((flags&3)==simrml_phase_sync_if_possible)
				Flags.SynchronizationBehavior=RMLFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE; 
			if ((flags&3)==simrml_only_time_sync)
				Flags.SynchronizationBehavior=RMLFlags::ONLY_TIME_SYNCHRONIZATION;
			if ((flags&3)==simrml_only_phase_sync)
				Flags.SynchronizationBehavior=RMLFlags::ONLY_PHASE_SYNCHRONIZATION;
			if ((flags&3)==simrml_no_sync)
				Flags.SynchronizationBehavior=RMLFlags::NO_SYNCHRONIZATION; // default

			if (flags&simrml_disable_extremum_motion_states_calc)
				Flags.EnableTheCalculationOfTheExtremumMotionStates=false;
			else
				Flags.EnableTheCalculationOfTheExtremumMotionStates=true; // default
		}
		off+=4;

		// Check for extension data (not used for now):
		unsigned char extensionBytes=data[off];
		off+=1+extensionBytes;

		// Execute the Reflexxe function!
		replyData[0]=RML->RMLVelocity(*IP,OP,Flags);

		// Next to returning the function return value (just here above), we return a buffer with the new position, velocity and acceleration vector:
		// We also return 8 additional doubles for future extension (1 double is already in use)
		char* retBuff=simCreateBuffer(dofs*8*3+8*8);
		off=0;

		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewPositionVector->VecData[i]/1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewVelocityVector->VecData[i]/1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			((double*)(retBuff+off))[i]=OP->NewAccelerationVector->VecData[i]/1000.0;
		off+=dofs*8;

		((double*)(retBuff+off))[0]=OP->SynchronizationTime;
//		((double*)(retBuff+off))[1]=OP->AlphaTime;
		off+=8*8;

		retVal=retBuff;

		delete RML;
		delete IP;
		delete OP;
	}

	if (message==sim_message_eventcallback_rmlpos)
	{ // the sim_message_eventcallback_rmlpos message is passed when the API function simRMLPos is called from C/C++ or Lua

		SObj obj;
		obj.RML=NULL;
		obj.PIP=NULL;
		obj.POP=NULL;
		obj.PFlags=NULL;
		obj.VIP=NULL;
		obj.VOP=NULL;
		obj.VFlags=NULL;
		obj.destroyAtSimulationEnd=(auxiliaryData[1]!=0);
		obj.objectHandle=nextObjectHandle;
		
		// All input parameters are coded in the data buffer! (i.e. first 4 bytes=DoFs, next 8 bytes=time step, etc.)
		// The values in the data buffer are in meters, not millimeters!
		char* data=(char*)customData;
		int dofs=((int*)(data+0))[0];
		obj.dofs=dofs;
		double timeStep=((double*)(data+4))[0];
		obj.smallestTimeStep=timeStep;
		int off=12;

		obj.RML = new ReflexxesAPI(dofs,timeStep);
		obj.PIP = new RMLPositionInputParameters(dofs);
		obj.POP = new RMLPositionOutputParameters(dofs);
		obj.PFlags = new RMLPositionFlags();
 
		for (int i=0;i<dofs;i++)
			obj.PIP->CurrentPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.PIP->CurrentVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.PIP->CurrentAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			obj.PIP->MaxVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.PIP->MaxAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.PIP->MaxJerkVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			obj.PIP->SelectionVector->VecData[i]=(data[off+i]!=0);
		off+=dofs;

		for (int i=0;i<dofs;i++)
			obj.PIP->TargetPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.PIP->TargetVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		// Apply the flags:
		int flags=((int*)(data+off))[0];
		if (flags>=0)
		{ // we don't have default values!
			if ((flags&3)==simrml_phase_sync_if_possible)
				obj.PFlags->SynchronizationBehavior=RMLFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE; // default
			if ((flags&3)==simrml_only_time_sync)
				obj.PFlags->SynchronizationBehavior=RMLFlags::ONLY_TIME_SYNCHRONIZATION;
			if ((flags&3)==simrml_only_phase_sync)
				obj.PFlags->SynchronizationBehavior=RMLFlags::ONLY_PHASE_SYNCHRONIZATION;
			if ((flags&3)==simrml_no_sync)
				obj.PFlags->SynchronizationBehavior=RMLFlags::NO_SYNCHRONIZATION;

			if ((flags&4)==simrml_keep_target_vel)
				obj.PFlags->BehaviorAfterFinalStateOfMotionIsReached=RMLPositionFlags::KEEP_TARGET_VELOCITY; // default
			if ((flags&4)==simrml_recompute_trajectory)
				obj.PFlags->BehaviorAfterFinalStateOfMotionIsReached=RMLPositionFlags::RECOMPUTE_TRAJECTORY;

			if (flags&simrml_disable_extremum_motion_states_calc)
				obj.PFlags->EnableTheCalculationOfTheExtremumMotionStates=false;
			else
				obj.PFlags->EnableTheCalculationOfTheExtremumMotionStates=true; // default

			if (flags&simrml_keep_current_vel_if_fallback_strategy)
				obj.PFlags->KeepCurrentVelocityInCaseOfFallbackStrategy=true; 
			else
				obj.PFlags->KeepCurrentVelocityInCaseOfFallbackStrategy=false; // default
		}
		off+=4;
		
		// Check for extension data (not used for now):
		unsigned char extensionBytes=data[off];
		off+=1+extensionBytes;

		allObjects.push_back(obj);
		replyData[0]=0;
		replyData[1]=nextObjectHandle++;
	}

	if (message==sim_message_eventcallback_rmlvel)
	{ // the sim_message_eventcallback_rmlvel message is passed when the API function simRMLVel is called from C/C++ or Lua

		SObj obj;
		obj.RML=NULL;
		obj.PIP=NULL;
		obj.POP=NULL;
		obj.PFlags=NULL;
		obj.VIP=NULL;
		obj.VOP=NULL;
		obj.VFlags=NULL;
		obj.destroyAtSimulationEnd=(auxiliaryData[1]!=0);
		obj.objectHandle=nextObjectHandle;
		
		// All input parameters are coded in the data buffer! (i.e. first 4 bytes=DoFs, next 8 bytes=time step, etc.)
		// The values in the data buffer are in meters, not millimeters!
		char* data=(char*)customData;
		int dofs=((int*)(data+0))[0];
		obj.dofs=dofs;
		double timeStep=((double*)(data+4))[0];
		obj.smallestTimeStep=timeStep;
		int off=12;

		obj.RML = new ReflexxesAPI(dofs,timeStep);
		obj.VIP = new RMLVelocityInputParameters(dofs);
		obj.VOP = new RMLVelocityOutputParameters(dofs);
 		obj.VFlags = new RMLVelocityFlags();

		for (int i=0;i<dofs;i++)
			obj.VIP->CurrentPositionVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.VIP->CurrentVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.VIP->CurrentAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			obj.VIP->MaxAccelerationVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;
		for (int i=0;i<dofs;i++)
			obj.VIP->MaxJerkVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		for (int i=0;i<dofs;i++)
			obj.VIP->SelectionVector->VecData[i]=(data[off+i]!=0);
		off+=dofs;

		for (int i=0;i<dofs;i++)
			obj.VIP->TargetVelocityVector->VecData[i]=((double*)(data+off))[i]*1000.0;
		off+=dofs*8;

		// Apply the flags:
		int flags=((int*)(data+off))[0];
		if (flags>=0)
		{ // we don't have default values!
			if ((flags&3)==simrml_phase_sync_if_possible)
				obj.VFlags->SynchronizationBehavior=RMLFlags::PHASE_SYNCHRONIZATION_IF_POSSIBLE; 
			if ((flags&3)==simrml_only_time_sync)
				obj.VFlags->SynchronizationBehavior=RMLFlags::ONLY_TIME_SYNCHRONIZATION;
			if ((flags&3)==simrml_only_phase_sync)
				obj.VFlags->SynchronizationBehavior=RMLFlags::ONLY_PHASE_SYNCHRONIZATION;
			if ((flags&3)==simrml_no_sync)
				obj.VFlags->SynchronizationBehavior=RMLFlags::NO_SYNCHRONIZATION; // default

			if (flags&simrml_disable_extremum_motion_states_calc)
				obj.VFlags->EnableTheCalculationOfTheExtremumMotionStates=false;
			else
				obj.VFlags->EnableTheCalculationOfTheExtremumMotionStates=true; // default
		}
		off+=4;

		// Check for extension data (not used for now):
		unsigned char extensionBytes=data[off];
		off+=1+extensionBytes;

		allObjects.push_back(obj);
		replyData[0]=0;
		replyData[1]=nextObjectHandle++;
	}

	if (message==sim_message_eventcallback_rmlstep)
	{ // the sim_message_eventcallback_rmlstep message is passed when the API function simRMLStep is called from C/C++ or Lua
		int index=-1;
		bool rmlPos=true;
		for (int i=0;i<int(allObjects.size());i++)
		{
			if (allObjects[i].objectHandle==auxiliaryData[1])
			{
				rmlPos=(allObjects[i].PIP!=NULL);
				index=i;
				break;
			}
		}
		if (index!=-1)
		{
			int dofs=allObjects[index].dofs;
			double timeStep=double(auxiliaryData[2])/100000.0;
			int cnt=int((timeStep/allObjects[index].smallestTimeStep)+0.5);
			if (rmlPos)
			{
				for (int i=0;i<cnt;i++)
				{
					replyData[0]=allObjects[index].RML->RMLPosition(*allObjects[index].PIP,allObjects[index].POP,*allObjects[index].PFlags);

					for (int j=0;j<dofs;j++)
						allObjects[index].PIP->CurrentPositionVector->VecData[j]=allObjects[index].POP->NewPositionVector->VecData[j];
					for (int j=0;j<dofs;j++)
						allObjects[index].PIP->CurrentVelocityVector->VecData[j]=allObjects[index].POP->NewVelocityVector->VecData[j];
					for (int j=0;j<dofs;j++)
						allObjects[index].PIP->CurrentAccelerationVector->VecData[j]=allObjects[index].POP->NewAccelerationVector->VecData[j];

					if (replyData[0]!=0)
						break;
				}

				// Next to returning the function return value (just here above), we return a buffer with the new position, velocity and acceleration vector:
				// We also return 8 additional doubles for future extension (1 double is already in use)
				char* retBuff=simCreateBuffer(dofs*8*3+8*8);
				int off=0;

				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].POP->NewPositionVector->VecData[i]/1000.0;
				off+=dofs*8;
				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].POP->NewVelocityVector->VecData[i]/1000.0;
				off+=dofs*8;
				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].POP->NewAccelerationVector->VecData[i]/1000.0;
				off+=dofs*8;

				((double*)(retBuff+off))[0]=allObjects[index].POP->SynchronizationTime;
				off+=8*8;

				retVal=retBuff;
			}
			else
			{ // RML velocity
				for (int i=0;i<cnt;i++)
				{
					replyData[0]=allObjects[index].RML->RMLVelocity(*allObjects[index].VIP,allObjects[index].VOP,*allObjects[index].VFlags);

					for (int j=0;j<dofs;j++)
						allObjects[index].VIP->CurrentPositionVector->VecData[j]=allObjects[index].VOP->NewPositionVector->VecData[j];
					for (int j=0;j<dofs;j++)
						allObjects[index].VIP->CurrentVelocityVector->VecData[j]=allObjects[index].VOP->NewVelocityVector->VecData[j];
					for (int j=0;j<dofs;j++)
						allObjects[index].VIP->CurrentAccelerationVector->VecData[j]=allObjects[index].VOP->NewAccelerationVector->VecData[j];

					if (replyData[0]!=0)
						break;
				}

				// Next to returning the function return value (just here above), we return a buffer with the new position, velocity and acceleration vector:
				// We also return 8 additional doubles for future extension (1 double is already in use)
				char* retBuff=simCreateBuffer(dofs*8*3+8*8);
				int off=0;

				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].VOP->NewPositionVector->VecData[i]/1000.0;
				off+=dofs*8;
				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].VOP->NewVelocityVector->VecData[i]/1000.0;
				off+=dofs*8;
				for (int i=0;i<dofs;i++)
					((double*)(retBuff+off))[i]=allObjects[index].VOP->NewAccelerationVector->VecData[i]/1000.0;
				off+=dofs*8;

				((double*)(retBuff+off))[0]=allObjects[index].VOP->SynchronizationTime;
				off+=8*8;

				retVal=retBuff;
			}
			replyData[1]=dofs;
		}
	}

	if (message==sim_message_eventcallback_rmlremove)
	{ // the sim_message_eventcallback_rmlremove message is passed when the API function simRMLRemove is called from C/C++ or Lua
		replyData[1]=0;
		for (int i=0;i<int(allObjects.size());i++)
		{
			if (allObjects[i].objectHandle==auxiliaryData[1])
			{
				if (allObjects[i].PIP!=NULL)
				{
					delete allObjects[i].RML;
					delete allObjects[i].PIP;
					delete allObjects[i].POP;
					delete allObjects[i].PFlags;
				}
				else
				{
					delete allObjects[i].RML;
					delete allObjects[i].VIP;
					delete allObjects[i].VOP;
					delete allObjects[i].VFlags;
				}
				allObjects.erase(allObjects.begin()+i);
				replyData[1]=1;
				break;
			}
		}
		replyData[0]=0;
	}

	if (message==sim_message_eventcallback_rmlinfo)
	{ // the sim_message_eventcallback_rmlinfo is used in V-REP internally
		if (auxiliaryData[0]==0)
		{ // means: give me the Dofs of this object
			replyData[1]=-1;
			for (int i=0;i<int(allObjects.size());i++)
			{
				if (allObjects[i].objectHandle==auxiliaryData[1])
				{
					replyData[1]=allObjects[i].dofs;
					break;
				}
			}
		}
		replyData[0]=0;
	}


	if (message==sim_message_eventcallback_simulationabouttostart)
	{ // Simulation is about to start

	}

	if (message==sim_message_eventcallback_simulationended)
	{ // Simulation just ended
		for (int i=0;i<int(allObjects.size());i++)
		{
			if (allObjects[i].destroyAtSimulationEnd)
			{
				if (allObjects[i].PIP!=NULL)
				{
					delete allObjects[i].RML;
					delete allObjects[i].PIP;
					delete allObjects[i].POP;
					delete allObjects[i].PFlags;
				}
				else
				{
					delete allObjects[i].RML;
					delete allObjects[i].VIP;
					delete allObjects[i].VOP;
					delete allObjects[i].VFlags;
				}
				allObjects.erase(allObjects.begin()+i);
				i--; // reprocess this position
			}
		}
	}


	// Keep following unchanged:
	simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
	return(retVal);
}

