#include "stubs.h"

#include <iostream>

simInt simRegisterScriptCallbackFunctionE(const simChar *funcNameAtPluginName, const simChar *callTips, simVoid (*callBack)(struct SScriptCallBack *cb))
{
    simInt ret = simRegisterScriptCallbackFunction(funcNameAtPluginName, callTips, callBack);
    if(ret == 0)
    {
        std::cout << "Plugin 'OMPL': warning: replaced function '" << funcNameAtPluginName << "'" << std::endl;
    }
    if(ret == -1)
        throw exception("simRegisterScriptCallbackFunction: error");
    return ret;
}

simInt simRegisterScriptVariableE(const simChar *varName, const simChar *varValue, simInt stackID)
{
    simInt ret = simRegisterScriptVariable(varName, varValue, stackID);
    if(ret == 0)
    {
        std::cout << "Plugin 'OMPL': warning: replaced variable '" << varName << "'" << std::endl;
    }
    if(ret == -1)
        throw exception("simRegisterScriptVariable: error");
    return ret;
}

simVoid simCallScriptFunctionExE(simInt scriptHandleOrType,const simChar* functionNameAtScriptName,simInt stackId)
{
    if(simCallScriptFunctionEx(scriptHandleOrType, functionNameAtScriptName, stackId) == -1)
        throw exception("simCallScriptFunctionEx: error");
}

simInt simCreateStackE()
{
    simInt ret = simCreateStack();
    if(ret == -1)
        throw exception("simCreateStack: error");
    return ret;
}

simVoid simReleaseStackE(simInt stackHandle)
{
    if(simReleaseStack(stackHandle) != 1)
        throw exception("simReleaseStack: error");
}

simInt simCopyStackE(simInt stackHandle)
{
    simInt ret = simCopyStack(stackHandle);
    if(ret == -1)
        throw exception("simCopyStack: error");
    return ret;
}

simVoid simPushNullOntoStackE(simInt stackHandle)
{
    if(simPushNullOntoStack(stackHandle) == -1)
        throw exception("simPushNullOntoStack: error");
}

simVoid simPushBoolOntoStackE(simInt stackHandle, simBool value)
{
    if(simPushBoolOntoStack(stackHandle, value) == -1)
        throw exception("simPushBoolOntoStack: error");
}

simVoid simPushInt32OntoStackE(simInt stackHandle, simInt value)
{
    if(simPushInt32OntoStack(stackHandle, value) == -1)
        throw exception("simPushInt32OntoStack: error");
}

simVoid simPushFloatOntoStackE(simInt stackHandle, simFloat value)
{
    if(simPushFloatOntoStack(stackHandle, value) == -1)
        throw exception("simPushFloatOntoStack: error");
}

simVoid simPushDoubleOntoStackE(simInt stackHandle, simDouble value)
{
    if(simPushDoubleOntoStack(stackHandle, value) == -1)
        throw exception("simPushDoubleOntoStack: error");
}

simVoid simPushStringOntoStackE(simInt stackHandle, const simChar *value, simInt stringSize)
{
    if(simPushStringOntoStack(stackHandle, value, stringSize) == -1)
        throw exception("simPushStringOntoStack: error");
}

simVoid simPushInt32TableOntoStackE(simInt stackHandle, const simInt *values, simInt valueCnt)
{
    if(simPushInt32TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw exception("simPushInt32TableOntoStack: error");
}

simVoid simPushFloatTableOntoStackE(simInt stackHandle, const simFloat *values, simInt valueCnt)
{
    if(simPushFloatTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw exception("simPushFloatTableOntoStack: error");
}

simVoid simPushDoubleTableOntoStackE(simInt stackHandle, const simDouble *values, simInt valueCnt)
{
    if(simPushDoubleTableOntoStack(stackHandle, values, valueCnt) == -1)
        throw exception("simPushDoubleTableOntoStack: error");
}

simVoid simPushTableOntoStackE(simInt stackHandle)
{
    if(simPushTableOntoStack(stackHandle) == -1)
        throw exception("simPushTableOntoStack: error");
}

simVoid simInsertDataIntoStackTableE(simInt stackHandle)
{
    if(simInsertDataIntoStackTable(stackHandle) == -1)
        throw exception("simInsertDataIntoStackTable: error");
}

simInt simGetStackSizeE(simInt stackHandle)
{
    simInt ret = simGetStackSize(stackHandle);
    if(ret == -1)
        throw exception("simGetStackSize: error");
    return ret;
}    

simInt simPopStackItemE(simInt stackHandle, simInt count)
{
    simInt ret = simPopStackItem(stackHandle, count);
    if(ret == -1)
        throw exception("simPopStackItem: error");
    return ret;
}    

simVoid simMoveStackItemToTopE(simInt stackHandle, simInt cIndex)
{
    if(simMoveStackItemToTop(stackHandle, cIndex) == -1)
        throw exception("simMoveStackItemToTop: error");
}

simInt simIsStackValueNullE(simInt stackHandle)
{
    simInt ret = simIsStackValueNull(stackHandle);
    if(ret == -1)
        throw exception("simIsStackValueNull: error");
    return ret;
}

simInt simGetStackBoolValueE(simInt stackHandle, simBool *boolValue)
{
    simInt ret = simGetStackBoolValue(stackHandle, boolValue);
    if(ret == -1)
        throw exception("simGetStackBoolValue: error");
    return ret;
}

simInt simGetStackInt32ValueE(simInt stackHandle, simInt *numberValue)
{
    simInt ret = simGetStackInt32Value(stackHandle, numberValue);
    if(ret == -1)
        throw exception("simGetStackInt32Value: error");
    return ret;
}

simInt simGetStackFloatValueE(simInt stackHandle, simFloat *numberValue)
{
    simInt ret = simGetStackFloatValue(stackHandle, numberValue);
    if(ret == -1)
        throw exception("simGetStackFloatValue: error");
    return ret;
}

simInt simGetStackDoubleValueE(simInt stackHandle, simDouble *numberValue)
{
    simInt ret = simGetStackDoubleValue(stackHandle, numberValue);
    if(ret == -1)
        throw exception("simGetStackDoubleValue: error");
    return ret;
}

simChar* simGetStackStringValueE(simInt stackHandle, simInt *stringSize)
{
    simChar *ret = simGetStackStringValue(stackHandle, stringSize);
    if(ret == NULL && (stringSize == NULL || *stringSize == -1))
        throw exception("simGetStackStringValue: error");
    return ret;
}

simInt simGetStackTableInfoE(simInt stackHandle, simInt infoType)
{
    simInt ret = simGetStackTableInfo(stackHandle, infoType);
    if(ret == -1)
        throw exception("simGetStackTableInfo: error");
    return ret;
}

simInt simGetStackInt32TableE(simInt stackHandle, simInt *array, simInt count)
{
    simInt ret = simGetStackInt32Table(stackHandle, array, count);
    if(ret == -1)
        throw exception("simGetStackInt32Table: error");
    return ret;
}

simInt simGetStackFloatTableE(simInt stackHandle, simFloat *array, simInt count)
{
    simInt ret = simGetStackFloatTable(stackHandle, array, count);
    if(ret == -1)
        throw exception("simGetStackFloatTable: error");
    return ret;
}

simInt simGetStackDoubleTableE(simInt stackHandle, simDouble *array, simInt count)
{
    simInt ret = simGetStackDoubleTable(stackHandle, array, count);
    if(ret == -1)
        throw exception("simGetStackDoubleTable: error");
    return ret;
}

simVoid simUnfoldStackTableE(simInt stackHandle)
{
    if(simUnfoldStackTable(stackHandle) == -1)
        throw exception("simUnfoldStackTable: error");
}    

void read__bool(int stack, bool *value)
{
    simBool v;
    if(simGetStackBoolValueE(stack, &v) == 1)
    {
        *value = v;
        simPopStackItemE(stack, 1);
    }
    else
    {
        throw exception("expected bool");
    }
}

void read__int(int stack, int *value)
{
    int v;
    if(simGetStackInt32ValueE(stack, &v) == 1)
    {
        *value = v;
        simPopStackItemE(stack, 1);
    }
    else
    {
        throw exception("expected int");
    }
}

void read__float(int stack, float *value)
{
    simFloat v;
    if(simGetStackFloatValueE(stack, &v) == 1)
    {
        *value = v;
        simPopStackItemE(stack, 1);
    }
    else
    {
        throw exception("expected float");
    }
}

void read__std__string(int stack, std::string *value)
{
    simChar *str;
    simInt strSize;
    if((str = simGetStackStringValueE(stack, &strSize)) != NULL && strSize > 0)
    {
        *value = std::string(str, strSize);
        simPopStackItemE(stack, 1);
    }
    else
    {
        throw exception("expected string");
    }
}

void write__bool(bool value, int stack)
{
    simBool v = value;
    simPushBoolOntoStackE(stack, v);
}

void write__int(int value, int stack)
{
    simInt v = value;
    simPushInt32OntoStackE(stack, v);
}

void write__float(float value, int stack)
{
    simFloat v = value;
    simPushFloatOntoStackE(stack, v);
}

void write__std__string(std::string value, int stack)
{
    const simChar *v = value.c_str();
    simPushStringOntoStackE(stack, v, value.length());
}

bool registerScriptStuff()
{
    try
    {
        simRegisterScriptVariableE("sim_ompl_algorithm_BiTRRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_BiTRRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_BITstar", (boost::lexical_cast<std::string>(sim_ompl_algorithm_BITstar)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_BKPIECE1", (boost::lexical_cast<std::string>(sim_ompl_algorithm_BKPIECE1)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_CForest", (boost::lexical_cast<std::string>(sim_ompl_algorithm_CForest)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_EST", (boost::lexical_cast<std::string>(sim_ompl_algorithm_EST)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_FMT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_FMT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_KPIECE1", (boost::lexical_cast<std::string>(sim_ompl_algorithm_KPIECE1)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_LazyPRM", (boost::lexical_cast<std::string>(sim_ompl_algorithm_LazyPRM)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_LazyPRMstar", (boost::lexical_cast<std::string>(sim_ompl_algorithm_LazyPRMstar)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_LazyRRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_LazyRRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_LBKPIECE1", (boost::lexical_cast<std::string>(sim_ompl_algorithm_LBKPIECE1)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_LBTRRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_LBTRRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_PDST", (boost::lexical_cast<std::string>(sim_ompl_algorithm_PDST)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_PRM", (boost::lexical_cast<std::string>(sim_ompl_algorithm_PRM)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_PRMstar", (boost::lexical_cast<std::string>(sim_ompl_algorithm_PRMstar)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_pRRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_pRRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_pSBL", (boost::lexical_cast<std::string>(sim_ompl_algorithm_pSBL)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_RRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_RRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_RRTConnect", (boost::lexical_cast<std::string>(sim_ompl_algorithm_RRTConnect)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_RRTstar", (boost::lexical_cast<std::string>(sim_ompl_algorithm_RRTstar)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_SBL", (boost::lexical_cast<std::string>(sim_ompl_algorithm_SBL)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_SPARS", (boost::lexical_cast<std::string>(sim_ompl_algorithm_SPARS)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_SPARStwo", (boost::lexical_cast<std::string>(sim_ompl_algorithm_SPARStwo)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_STRIDE", (boost::lexical_cast<std::string>(sim_ompl_algorithm_STRIDE)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_algorithm_TRRT", (boost::lexical_cast<std::string>(sim_ompl_algorithm_TRRT)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_statespacetype_position2d", (boost::lexical_cast<std::string>(sim_ompl_statespacetype_position2d)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_statespacetype_pose2d", (boost::lexical_cast<std::string>(sim_ompl_statespacetype_pose2d)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_statespacetype_position3d", (boost::lexical_cast<std::string>(sim_ompl_statespacetype_position3d)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_statespacetype_pose3d", (boost::lexical_cast<std::string>(sim_ompl_statespacetype_pose3d)).c_str(), 0);
        simRegisterScriptVariableE("sim_ompl_statespacetype_joint_position", (boost::lexical_cast<std::string>(sim_ompl_statespacetype_joint_position)).c_str(), 0);
        simRegisterScriptCallbackFunctionE("simExtOMPL_createStateSpace@OMPL", "number stateSpaceHandle=simExtOMPL_createStateSpace(string name,number type,number objectHandle,table boundsLow,table boundsHigh,number useForProjection,number weight=1.0,number refObjectHandle=-1)", createStateSpace_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_destroyStateSpace@OMPL", "number result=simExtOMPL_destroyStateSpace(number stateSpaceHandle)", destroyStateSpace_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_createTask@OMPL", "number taskHandle=simExtOMPL_createTask(string name)", createTask_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_destroyTask@OMPL", "number result=simExtOMPL_destroyTask(number taskHandle)", destroyTask_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_printTaskInfo@OMPL", "number result=simExtOMPL_printTaskInfo(number taskHandle)", printTaskInfo_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setVerboseLevel@OMPL", "number result=simExtOMPL_setVerboseLevel(number taskHandle,number verboseLevel)", setVerboseLevel_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setStateValidityCheckingResolution@OMPL", "number result=simExtOMPL_setStateValidityCheckingResolution(number taskHandle,number resolution)", setStateValidityCheckingResolution_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setStateSpace@OMPL", "number result=simExtOMPL_setStateSpace(number taskHandle,table stateSpaceHandles)", setStateSpace_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setAlgorithm@OMPL", "number result=simExtOMPL_setAlgorithm(number taskHandle,number algorithm)", setAlgorithm_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setCollisionPairs@OMPL", "number result=simExtOMPL_setCollisionPairs(number taskHandle,table collisionPairHandles)", setCollisionPairs_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setStartState@OMPL", "number result=simExtOMPL_setStartState(number taskHandle,table state)", setStartState_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setGoalState@OMPL", "number result=simExtOMPL_setGoalState(number taskHandle,table state)", setGoalState_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_addGoalState@OMPL", "number result=simExtOMPL_addGoalState(number taskHandle,table state)", addGoalState_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setGoal@OMPL", "number result=simExtOMPL_setGoal(number taskHandle,number robotDummy,number goalDummy,number tolerance=0.001f,table metric={1.0, 1.0, 1.0, 0.1},number refDummy=-1)", setGoal_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setup@OMPL", "number result=simExtOMPL_setup(number taskHandle)", setup_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_solve@OMPL", "number result=simExtOMPL_solve(number taskHandle,number maxTime)", solve_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_simplifyPath@OMPL", "number result=simExtOMPL_simplifyPath(number taskHandle,number maxSimplificationTime=-1.0)", simplifyPath_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_interpolatePath@OMPL", "number result=simExtOMPL_interpolatePath(number taskHandle,number stateCnt=0)", interpolatePath_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_getPath@OMPL", "number result,table states=simExtOMPL_getPath(number taskHandle)", getPath_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_compute@OMPL", "number result,table states=simExtOMPL_compute(number taskHandle,number maxTime,number maxSimplificationTime=-1.0,number stateCnt=0)", compute_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_readState@OMPL", "number result,table state=simExtOMPL_readState(number taskHandle)", readState_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_writeState@OMPL", "number result=simExtOMPL_writeState(number taskHandle,table state)", writeState_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_isStateValid@OMPL", "number valid=simExtOMPL_isStateValid(number taskHandle,table state)", isStateValid_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setProjectionEvaluationCallback@OMPL", "number result=simExtOMPL_setProjectionEvaluationCallback(number taskHandle,string callback,number projectionSize)", setProjectionEvaluationCallback_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setStateValidationCallback@OMPL", "number result=simExtOMPL_setStateValidationCallback(number taskHandle,string callback)", setStateValidationCallback_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setGoalCallback@OMPL", "number result=simExtOMPL_setGoalCallback(number taskHandle,string callback)", setGoalCallback_callback);
        simRegisterScriptCallbackFunctionE("simExtOMPL_setValidStateSamplerCallback@OMPL", "number result=simExtOMPL_setValidStateSamplerCallback(number taskHandle,string callback,string callbackNear)", setValidStateSamplerCallback_callback);
    }
    catch(exception& ex)
    {
        std::cout << "Initialization failed (registerScriptStuff): " << ex.what() << std::endl;
        return false;
    }
    return true;
}

const char* algorithm_string(Algorithm x)
{
    switch(x)
    {
        case sim_ompl_algorithm_BiTRRT: return "sim_ompl_algorithm_BiTRRT";
        case sim_ompl_algorithm_BITstar: return "sim_ompl_algorithm_BITstar";
        case sim_ompl_algorithm_BKPIECE1: return "sim_ompl_algorithm_BKPIECE1";
        case sim_ompl_algorithm_CForest: return "sim_ompl_algorithm_CForest";
        case sim_ompl_algorithm_EST: return "sim_ompl_algorithm_EST";
        case sim_ompl_algorithm_FMT: return "sim_ompl_algorithm_FMT";
        case sim_ompl_algorithm_KPIECE1: return "sim_ompl_algorithm_KPIECE1";
        case sim_ompl_algorithm_LazyPRM: return "sim_ompl_algorithm_LazyPRM";
        case sim_ompl_algorithm_LazyPRMstar: return "sim_ompl_algorithm_LazyPRMstar";
        case sim_ompl_algorithm_LazyRRT: return "sim_ompl_algorithm_LazyRRT";
        case sim_ompl_algorithm_LBKPIECE1: return "sim_ompl_algorithm_LBKPIECE1";
        case sim_ompl_algorithm_LBTRRT: return "sim_ompl_algorithm_LBTRRT";
        case sim_ompl_algorithm_PDST: return "sim_ompl_algorithm_PDST";
        case sim_ompl_algorithm_PRM: return "sim_ompl_algorithm_PRM";
        case sim_ompl_algorithm_PRMstar: return "sim_ompl_algorithm_PRMstar";
        case sim_ompl_algorithm_pRRT: return "sim_ompl_algorithm_pRRT";
        case sim_ompl_algorithm_pSBL: return "sim_ompl_algorithm_pSBL";
        case sim_ompl_algorithm_RRT: return "sim_ompl_algorithm_RRT";
        case sim_ompl_algorithm_RRTConnect: return "sim_ompl_algorithm_RRTConnect";
        case sim_ompl_algorithm_RRTstar: return "sim_ompl_algorithm_RRTstar";
        case sim_ompl_algorithm_SBL: return "sim_ompl_algorithm_SBL";
        case sim_ompl_algorithm_SPARS: return "sim_ompl_algorithm_SPARS";
        case sim_ompl_algorithm_SPARStwo: return "sim_ompl_algorithm_SPARStwo";
        case sim_ompl_algorithm_STRIDE: return "sim_ompl_algorithm_STRIDE";
        case sim_ompl_algorithm_TRRT: return "sim_ompl_algorithm_TRRT";
        default: return "???";
    }
}

const char* statespacetype_string(StateSpaceType x)
{
    switch(x)
    {
        case sim_ompl_statespacetype_position2d: return "sim_ompl_statespacetype_position2d";
        case sim_ompl_statespacetype_pose2d: return "sim_ompl_statespacetype_pose2d";
        case sim_ompl_statespacetype_position3d: return "sim_ompl_statespacetype_position3d";
        case sim_ompl_statespacetype_pose3d: return "sim_ompl_statespacetype_pose3d";
        case sim_ompl_statespacetype_joint_position: return "sim_ompl_statespacetype_joint_position";
        default: return "???";
    }
}

createStateSpace_in::createStateSpace_in()
{
    weight = 1.0;
    refObjectHandle = -1;
}

createStateSpace_out::createStateSpace_out()
{
}

void createStateSpace(SScriptCallBack *p, createStateSpace_in *in_args, createStateSpace_out *out_args)
{
    createStateSpace(p, "simExtOMPL_createStateSpace", in_args, out_args);
}

int createStateSpace(SScriptCallBack *p, std::string name, int type, int objectHandle, std::vector<float> boundsLow, std::vector<float> boundsHigh, int useForProjection, float weight, int refObjectHandle)
{
    createStateSpace_in in_args;
    in_args.name = name;
    in_args.type = type;
    in_args.objectHandle = objectHandle;
    in_args.boundsLow = boundsLow;
    in_args.boundsHigh = boundsHigh;
    in_args.useForProjection = useForProjection;
    in_args.weight = weight;
    in_args.refObjectHandle = refObjectHandle;
    createStateSpace_out out_args;
    createStateSpace(p, &in_args, &out_args);
    return out_args.stateSpaceHandle;
}

void createStateSpace(SScriptCallBack *p, createStateSpace_out *out_args, std::string name, int type, int objectHandle, std::vector<float> boundsLow, std::vector<float> boundsHigh, int useForProjection, float weight, int refObjectHandle)
{
    createStateSpace_in in_args;
    in_args.name = name;
    in_args.type = type;
    in_args.objectHandle = objectHandle;
    in_args.boundsLow = boundsLow;
    in_args.boundsHigh = boundsHigh;
    in_args.useForProjection = useForProjection;
    in_args.weight = weight;
    in_args.refObjectHandle = refObjectHandle;
    createStateSpace(p, &in_args, out_args);
}

void createStateSpace_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_createStateSpace";

    createStateSpace_in in_args;
    createStateSpace_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 6)
            throw exception("too few arguments");
        if(numArgs > 8)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (name)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (type) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.type));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (type)";
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (objectHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.objectHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (objectHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (boundsLow) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 4 (boundsLow): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.boundsLow.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 4 (boundsLow)";
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (boundsHigh) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 5 (boundsHigh): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.boundsHigh.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 5 (boundsHigh)";
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (useForProjection) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.useForProjection));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 6 (useForProjection)";
                throw exception(msg);
            }
        }


        if(numArgs >= 7)
        {
            try
            {
                // read input argument 7 (weight) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.weight));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 7 (weight)";
                throw exception(msg);
            }
        }


        if(numArgs >= 8)
        {
            try
            {
                // read input argument 8 (refObjectHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.refObjectHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 8 (refObjectHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        createStateSpace(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (stateSpaceHandle) of type int
            write__int(out_args.stateSpaceHandle, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (stateSpaceHandle)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

destroyStateSpace_in::destroyStateSpace_in()
{
}

destroyStateSpace_out::destroyStateSpace_out()
{
    result = 0;
}

void destroyStateSpace(SScriptCallBack *p, destroyStateSpace_in *in_args, destroyStateSpace_out *out_args)
{
    destroyStateSpace(p, "simExtOMPL_destroyStateSpace", in_args, out_args);
}

int destroyStateSpace(SScriptCallBack *p, int stateSpaceHandle)
{
    destroyStateSpace_in in_args;
    in_args.stateSpaceHandle = stateSpaceHandle;
    destroyStateSpace_out out_args;
    destroyStateSpace(p, &in_args, &out_args);
    return out_args.result;
}

void destroyStateSpace(SScriptCallBack *p, destroyStateSpace_out *out_args, int stateSpaceHandle)
{
    destroyStateSpace_in in_args;
    in_args.stateSpaceHandle = stateSpaceHandle;
    destroyStateSpace(p, &in_args, out_args);
}

void destroyStateSpace_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_destroyStateSpace";

    destroyStateSpace_in in_args;
    destroyStateSpace_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (stateSpaceHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.stateSpaceHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (stateSpaceHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        destroyStateSpace(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

createTask_in::createTask_in()
{
}

createTask_out::createTask_out()
{
}

void createTask(SScriptCallBack *p, createTask_in *in_args, createTask_out *out_args)
{
    createTask(p, "simExtOMPL_createTask", in_args, out_args);
}

int createTask(SScriptCallBack *p, std::string name)
{
    createTask_in in_args;
    in_args.name = name;
    createTask_out out_args;
    createTask(p, &in_args, &out_args);
    return out_args.taskHandle;
}

void createTask(SScriptCallBack *p, createTask_out *out_args, std::string name)
{
    createTask_in in_args;
    in_args.name = name;
    createTask(p, &in_args, out_args);
}

void createTask_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_createTask";

    createTask_in in_args;
    createTask_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (name)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        createTask(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (taskHandle) of type int
            write__int(out_args.taskHandle, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (taskHandle)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

destroyTask_in::destroyTask_in()
{
}

destroyTask_out::destroyTask_out()
{
    result = 0;
}

void destroyTask(SScriptCallBack *p, destroyTask_in *in_args, destroyTask_out *out_args)
{
    destroyTask(p, "simExtOMPL_destroyTask", in_args, out_args);
}

int destroyTask(SScriptCallBack *p, int taskHandle)
{
    destroyTask_in in_args;
    in_args.taskHandle = taskHandle;
    destroyTask_out out_args;
    destroyTask(p, &in_args, &out_args);
    return out_args.result;
}

void destroyTask(SScriptCallBack *p, destroyTask_out *out_args, int taskHandle)
{
    destroyTask_in in_args;
    in_args.taskHandle = taskHandle;
    destroyTask(p, &in_args, out_args);
}

void destroyTask_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_destroyTask";

    destroyTask_in in_args;
    destroyTask_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        destroyTask(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

printTaskInfo_in::printTaskInfo_in()
{
}

printTaskInfo_out::printTaskInfo_out()
{
    result = 0;
}

void printTaskInfo(SScriptCallBack *p, printTaskInfo_in *in_args, printTaskInfo_out *out_args)
{
    printTaskInfo(p, "simExtOMPL_printTaskInfo", in_args, out_args);
}

int printTaskInfo(SScriptCallBack *p, int taskHandle)
{
    printTaskInfo_in in_args;
    in_args.taskHandle = taskHandle;
    printTaskInfo_out out_args;
    printTaskInfo(p, &in_args, &out_args);
    return out_args.result;
}

void printTaskInfo(SScriptCallBack *p, printTaskInfo_out *out_args, int taskHandle)
{
    printTaskInfo_in in_args;
    in_args.taskHandle = taskHandle;
    printTaskInfo(p, &in_args, out_args);
}

void printTaskInfo_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_printTaskInfo";

    printTaskInfo_in in_args;
    printTaskInfo_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        printTaskInfo(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setVerboseLevel_in::setVerboseLevel_in()
{
}

setVerboseLevel_out::setVerboseLevel_out()
{
    result = 0;
}

void setVerboseLevel(SScriptCallBack *p, setVerboseLevel_in *in_args, setVerboseLevel_out *out_args)
{
    setVerboseLevel(p, "simExtOMPL_setVerboseLevel", in_args, out_args);
}

int setVerboseLevel(SScriptCallBack *p, int taskHandle, int verboseLevel)
{
    setVerboseLevel_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.verboseLevel = verboseLevel;
    setVerboseLevel_out out_args;
    setVerboseLevel(p, &in_args, &out_args);
    return out_args.result;
}

void setVerboseLevel(SScriptCallBack *p, setVerboseLevel_out *out_args, int taskHandle, int verboseLevel)
{
    setVerboseLevel_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.verboseLevel = verboseLevel;
    setVerboseLevel(p, &in_args, out_args);
}

void setVerboseLevel_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setVerboseLevel";

    setVerboseLevel_in in_args;
    setVerboseLevel_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (verboseLevel) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.verboseLevel));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (verboseLevel)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setVerboseLevel(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setStateValidityCheckingResolution_in::setStateValidityCheckingResolution_in()
{
}

setStateValidityCheckingResolution_out::setStateValidityCheckingResolution_out()
{
    result = 0;
}

void setStateValidityCheckingResolution(SScriptCallBack *p, setStateValidityCheckingResolution_in *in_args, setStateValidityCheckingResolution_out *out_args)
{
    setStateValidityCheckingResolution(p, "simExtOMPL_setStateValidityCheckingResolution", in_args, out_args);
}

int setStateValidityCheckingResolution(SScriptCallBack *p, int taskHandle, float resolution)
{
    setStateValidityCheckingResolution_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.resolution = resolution;
    setStateValidityCheckingResolution_out out_args;
    setStateValidityCheckingResolution(p, &in_args, &out_args);
    return out_args.result;
}

void setStateValidityCheckingResolution(SScriptCallBack *p, setStateValidityCheckingResolution_out *out_args, int taskHandle, float resolution)
{
    setStateValidityCheckingResolution_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.resolution = resolution;
    setStateValidityCheckingResolution(p, &in_args, out_args);
}

void setStateValidityCheckingResolution_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setStateValidityCheckingResolution";

    setStateValidityCheckingResolution_in in_args;
    setStateValidityCheckingResolution_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (resolution) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.resolution));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (resolution)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setStateValidityCheckingResolution(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setStateSpace_in::setStateSpace_in()
{
}

setStateSpace_out::setStateSpace_out()
{
    result = 0;
}

void setStateSpace(SScriptCallBack *p, setStateSpace_in *in_args, setStateSpace_out *out_args)
{
    setStateSpace(p, "simExtOMPL_setStateSpace", in_args, out_args);
}

int setStateSpace(SScriptCallBack *p, int taskHandle, std::vector<int> stateSpaceHandles)
{
    setStateSpace_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.stateSpaceHandles = stateSpaceHandles;
    setStateSpace_out out_args;
    setStateSpace(p, &in_args, &out_args);
    return out_args.result;
}

void setStateSpace(SScriptCallBack *p, setStateSpace_out *out_args, int taskHandle, std::vector<int> stateSpaceHandles)
{
    setStateSpace_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.stateSpaceHandles = stateSpaceHandles;
    setStateSpace(p, &in_args, out_args);
}

void setStateSpace_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setStateSpace";

    setStateSpace_in in_args;
    setStateSpace_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (stateSpaceHandles) of type array of std::vector<int>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (stateSpaceHandles): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    int v;
                    read__int(p->stackID, &v);
                    in_args.stateSpaceHandles.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (stateSpaceHandles)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setStateSpace(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setAlgorithm_in::setAlgorithm_in()
{
}

setAlgorithm_out::setAlgorithm_out()
{
    result = 0;
}

void setAlgorithm(SScriptCallBack *p, setAlgorithm_in *in_args, setAlgorithm_out *out_args)
{
    setAlgorithm(p, "simExtOMPL_setAlgorithm", in_args, out_args);
}

int setAlgorithm(SScriptCallBack *p, int taskHandle, int algorithm)
{
    setAlgorithm_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.algorithm = algorithm;
    setAlgorithm_out out_args;
    setAlgorithm(p, &in_args, &out_args);
    return out_args.result;
}

void setAlgorithm(SScriptCallBack *p, setAlgorithm_out *out_args, int taskHandle, int algorithm)
{
    setAlgorithm_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.algorithm = algorithm;
    setAlgorithm(p, &in_args, out_args);
}

void setAlgorithm_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setAlgorithm";

    setAlgorithm_in in_args;
    setAlgorithm_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (algorithm) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.algorithm));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (algorithm)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setAlgorithm(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setCollisionPairs_in::setCollisionPairs_in()
{
}

setCollisionPairs_out::setCollisionPairs_out()
{
    result = 0;
}

void setCollisionPairs(SScriptCallBack *p, setCollisionPairs_in *in_args, setCollisionPairs_out *out_args)
{
    setCollisionPairs(p, "simExtOMPL_setCollisionPairs", in_args, out_args);
}

int setCollisionPairs(SScriptCallBack *p, int taskHandle, std::vector<int> collisionPairHandles)
{
    setCollisionPairs_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.collisionPairHandles = collisionPairHandles;
    setCollisionPairs_out out_args;
    setCollisionPairs(p, &in_args, &out_args);
    return out_args.result;
}

void setCollisionPairs(SScriptCallBack *p, setCollisionPairs_out *out_args, int taskHandle, std::vector<int> collisionPairHandles)
{
    setCollisionPairs_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.collisionPairHandles = collisionPairHandles;
    setCollisionPairs(p, &in_args, out_args);
}

void setCollisionPairs_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setCollisionPairs";

    setCollisionPairs_in in_args;
    setCollisionPairs_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (collisionPairHandles) of type array of std::vector<int>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (collisionPairHandles): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    int v;
                    read__int(p->stackID, &v);
                    in_args.collisionPairHandles.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (collisionPairHandles)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setCollisionPairs(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setStartState_in::setStartState_in()
{
}

setStartState_out::setStartState_out()
{
    result = 0;
}

void setStartState(SScriptCallBack *p, setStartState_in *in_args, setStartState_out *out_args)
{
    setStartState(p, "simExtOMPL_setStartState", in_args, out_args);
}

int setStartState(SScriptCallBack *p, int taskHandle, std::vector<float> state)
{
    setStartState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    setStartState_out out_args;
    setStartState(p, &in_args, &out_args);
    return out_args.result;
}

void setStartState(SScriptCallBack *p, setStartState_out *out_args, int taskHandle, std::vector<float> state)
{
    setStartState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    setStartState(p, &in_args, out_args);
}

void setStartState_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setStartState";

    setStartState_in in_args;
    setStartState_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (state) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (state): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.state.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (state)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setStartState(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setGoalState_in::setGoalState_in()
{
}

setGoalState_out::setGoalState_out()
{
    result = 0;
}

void setGoalState(SScriptCallBack *p, setGoalState_in *in_args, setGoalState_out *out_args)
{
    setGoalState(p, "simExtOMPL_setGoalState", in_args, out_args);
}

int setGoalState(SScriptCallBack *p, int taskHandle, std::vector<float> state)
{
    setGoalState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    setGoalState_out out_args;
    setGoalState(p, &in_args, &out_args);
    return out_args.result;
}

void setGoalState(SScriptCallBack *p, setGoalState_out *out_args, int taskHandle, std::vector<float> state)
{
    setGoalState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    setGoalState(p, &in_args, out_args);
}

void setGoalState_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setGoalState";

    setGoalState_in in_args;
    setGoalState_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (state) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (state): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.state.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (state)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setGoalState(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

addGoalState_in::addGoalState_in()
{
}

addGoalState_out::addGoalState_out()
{
    result = 0;
}

void addGoalState(SScriptCallBack *p, addGoalState_in *in_args, addGoalState_out *out_args)
{
    addGoalState(p, "simExtOMPL_addGoalState", in_args, out_args);
}

int addGoalState(SScriptCallBack *p, int taskHandle, std::vector<float> state)
{
    addGoalState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    addGoalState_out out_args;
    addGoalState(p, &in_args, &out_args);
    return out_args.result;
}

void addGoalState(SScriptCallBack *p, addGoalState_out *out_args, int taskHandle, std::vector<float> state)
{
    addGoalState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    addGoalState(p, &in_args, out_args);
}

void addGoalState_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_addGoalState";

    addGoalState_in in_args;
    addGoalState_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (state) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (state): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.state.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (state)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        addGoalState(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setGoal_in::setGoal_in()
{
    tolerance = 0.001f;
    metric = boost::assign::list_of(1.0)(1.0)(1.0)(0.1);
    refDummy = -1;
}

setGoal_out::setGoal_out()
{
    result = 0;
}

void setGoal(SScriptCallBack *p, setGoal_in *in_args, setGoal_out *out_args)
{
    setGoal(p, "simExtOMPL_setGoal", in_args, out_args);
}

int setGoal(SScriptCallBack *p, int taskHandle, int robotDummy, int goalDummy, float tolerance, std::vector<float> metric, int refDummy)
{
    setGoal_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.robotDummy = robotDummy;
    in_args.goalDummy = goalDummy;
    in_args.tolerance = tolerance;
    in_args.metric = metric;
    in_args.refDummy = refDummy;
    setGoal_out out_args;
    setGoal(p, &in_args, &out_args);
    return out_args.result;
}

void setGoal(SScriptCallBack *p, setGoal_out *out_args, int taskHandle, int robotDummy, int goalDummy, float tolerance, std::vector<float> metric, int refDummy)
{
    setGoal_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.robotDummy = robotDummy;
    in_args.goalDummy = goalDummy;
    in_args.tolerance = tolerance;
    in_args.metric = metric;
    in_args.refDummy = refDummy;
    setGoal(p, &in_args, out_args);
}

void setGoal_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setGoal";

    setGoal_in in_args;
    setGoal_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 6)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (robotDummy) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.robotDummy));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (robotDummy)";
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (goalDummy) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.goalDummy));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (goalDummy)";
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (tolerance) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.tolerance));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 4 (tolerance)";
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (metric) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 5 (metric): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.metric.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 5 (metric)";
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (refDummy) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.refDummy));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 6 (refDummy)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setGoal(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setup_in::setup_in()
{
}

setup_out::setup_out()
{
    result = 0;
}

void setup(SScriptCallBack *p, setup_in *in_args, setup_out *out_args)
{
    setup(p, "simExtOMPL_setup", in_args, out_args);
}

int setup(SScriptCallBack *p, int taskHandle)
{
    setup_in in_args;
    in_args.taskHandle = taskHandle;
    setup_out out_args;
    setup(p, &in_args, &out_args);
    return out_args.result;
}

void setup(SScriptCallBack *p, setup_out *out_args, int taskHandle)
{
    setup_in in_args;
    in_args.taskHandle = taskHandle;
    setup(p, &in_args, out_args);
}

void setup_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setup";

    setup_in in_args;
    setup_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setup(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

solve_in::solve_in()
{
}

solve_out::solve_out()
{
    result = 0;
}

void solve(SScriptCallBack *p, solve_in *in_args, solve_out *out_args)
{
    solve(p, "simExtOMPL_solve", in_args, out_args);
}

int solve(SScriptCallBack *p, int taskHandle, float maxTime)
{
    solve_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.maxTime = maxTime;
    solve_out out_args;
    solve(p, &in_args, &out_args);
    return out_args.result;
}

void solve(SScriptCallBack *p, solve_out *out_args, int taskHandle, float maxTime)
{
    solve_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.maxTime = maxTime;
    solve(p, &in_args, out_args);
}

void solve_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_solve";

    solve_in in_args;
    solve_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (maxTime) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.maxTime));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (maxTime)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        solve(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

simplifyPath_in::simplifyPath_in()
{
    maxSimplificationTime = -1.0;
}

simplifyPath_out::simplifyPath_out()
{
    result = 0;
}

void simplifyPath(SScriptCallBack *p, simplifyPath_in *in_args, simplifyPath_out *out_args)
{
    simplifyPath(p, "simExtOMPL_simplifyPath", in_args, out_args);
}

int simplifyPath(SScriptCallBack *p, int taskHandle, float maxSimplificationTime)
{
    simplifyPath_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.maxSimplificationTime = maxSimplificationTime;
    simplifyPath_out out_args;
    simplifyPath(p, &in_args, &out_args);
    return out_args.result;
}

void simplifyPath(SScriptCallBack *p, simplifyPath_out *out_args, int taskHandle, float maxSimplificationTime)
{
    simplifyPath_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.maxSimplificationTime = maxSimplificationTime;
    simplifyPath(p, &in_args, out_args);
}

void simplifyPath_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_simplifyPath";

    simplifyPath_in in_args;
    simplifyPath_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (maxSimplificationTime) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.maxSimplificationTime));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (maxSimplificationTime)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        simplifyPath(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

interpolatePath_in::interpolatePath_in()
{
    stateCnt = 0;
}

interpolatePath_out::interpolatePath_out()
{
    result = 0;
}

void interpolatePath(SScriptCallBack *p, interpolatePath_in *in_args, interpolatePath_out *out_args)
{
    interpolatePath(p, "simExtOMPL_interpolatePath", in_args, out_args);
}

int interpolatePath(SScriptCallBack *p, int taskHandle, int stateCnt)
{
    interpolatePath_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.stateCnt = stateCnt;
    interpolatePath_out out_args;
    interpolatePath(p, &in_args, &out_args);
    return out_args.result;
}

void interpolatePath(SScriptCallBack *p, interpolatePath_out *out_args, int taskHandle, int stateCnt)
{
    interpolatePath_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.stateCnt = stateCnt;
    interpolatePath(p, &in_args, out_args);
}

void interpolatePath_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_interpolatePath";

    interpolatePath_in in_args;
    interpolatePath_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (stateCnt) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.stateCnt));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (stateCnt)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        interpolatePath(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

getPath_in::getPath_in()
{
}

getPath_out::getPath_out()
{
    result = 0;
}

void getPath(SScriptCallBack *p, getPath_in *in_args, getPath_out *out_args)
{
    getPath(p, "simExtOMPL_getPath", in_args, out_args);
}

void getPath(SScriptCallBack *p, getPath_out *out_args, int taskHandle)
{
    getPath_in in_args;
    in_args.taskHandle = taskHandle;
    getPath(p, &in_args, out_args);
}

void getPath_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_getPath";

    getPath_in in_args;
    getPath_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getPath(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (states) of type array of std::vector<float>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.states.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__float(out_args.states[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 2 (states)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

compute_in::compute_in()
{
    maxSimplificationTime = -1.0;
    stateCnt = 0;
}

compute_out::compute_out()
{
    result = 0;
}

void compute(SScriptCallBack *p, compute_in *in_args, compute_out *out_args)
{
    compute(p, "simExtOMPL_compute", in_args, out_args);
}

void compute(SScriptCallBack *p, compute_out *out_args, int taskHandle, float maxTime, float maxSimplificationTime, int stateCnt)
{
    compute_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.maxTime = maxTime;
    in_args.maxSimplificationTime = maxSimplificationTime;
    in_args.stateCnt = stateCnt;
    compute(p, &in_args, out_args);
}

void compute_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_compute";

    compute_in in_args;
    compute_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (maxTime) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.maxTime));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (maxTime)";
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (maxSimplificationTime) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.maxSimplificationTime));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (maxSimplificationTime)";
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (stateCnt) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.stateCnt));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 4 (stateCnt)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        compute(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (states) of type array of std::vector<float>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.states.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__float(out_args.states[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 2 (states)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

readState_in::readState_in()
{
}

readState_out::readState_out()
{
    result = 0;
}

void readState(SScriptCallBack *p, readState_in *in_args, readState_out *out_args)
{
    readState(p, "simExtOMPL_readState", in_args, out_args);
}

void readState(SScriptCallBack *p, readState_out *out_args, int taskHandle)
{
    readState_in in_args;
    in_args.taskHandle = taskHandle;
    readState(p, &in_args, out_args);
}

void readState_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_readState";

    readState_in in_args;
    readState_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 1)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        readState(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (state) of type array of std::vector<float>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.state.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__float(out_args.state[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 2 (state)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

writeState_in::writeState_in()
{
}

writeState_out::writeState_out()
{
    result = 0;
}

void writeState(SScriptCallBack *p, writeState_in *in_args, writeState_out *out_args)
{
    writeState(p, "simExtOMPL_writeState", in_args, out_args);
}

int writeState(SScriptCallBack *p, int taskHandle, std::vector<float> state)
{
    writeState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    writeState_out out_args;
    writeState(p, &in_args, &out_args);
    return out_args.result;
}

void writeState(SScriptCallBack *p, writeState_out *out_args, int taskHandle, std::vector<float> state)
{
    writeState_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    writeState(p, &in_args, out_args);
}

void writeState_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_writeState";

    writeState_in in_args;
    writeState_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (state) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (state): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.state.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (state)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        writeState(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

isStateValid_in::isStateValid_in()
{
}

isStateValid_out::isStateValid_out()
{
}

void isStateValid(SScriptCallBack *p, isStateValid_in *in_args, isStateValid_out *out_args)
{
    isStateValid(p, "simExtOMPL_isStateValid", in_args, out_args);
}

int isStateValid(SScriptCallBack *p, int taskHandle, std::vector<float> state)
{
    isStateValid_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    isStateValid_out out_args;
    isStateValid(p, &in_args, &out_args);
    return out_args.valid;
}

void isStateValid(SScriptCallBack *p, isStateValid_out *out_args, int taskHandle, std::vector<float> state)
{
    isStateValid_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.state = state;
    isStateValid(p, &in_args, out_args);
}

void isStateValid_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_isStateValid";

    isStateValid_in in_args;
    isStateValid_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (state) of type array of std::vector<float>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 2 (state): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    float v;
                    read__float(p->stackID, &v);
                    in_args.state.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (state)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        isStateValid(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (valid) of type int
            write__int(out_args.valid, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (valid)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setProjectionEvaluationCallback_in::setProjectionEvaluationCallback_in()
{
}

setProjectionEvaluationCallback_out::setProjectionEvaluationCallback_out()
{
    result = 0;
}

void setProjectionEvaluationCallback(SScriptCallBack *p, setProjectionEvaluationCallback_in *in_args, setProjectionEvaluationCallback_out *out_args)
{
    setProjectionEvaluationCallback(p, "simExtOMPL_setProjectionEvaluationCallback", in_args, out_args);
}

int setProjectionEvaluationCallback(SScriptCallBack *p, int taskHandle, std::string callback, int projectionSize)
{
    setProjectionEvaluationCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    in_args.projectionSize = projectionSize;
    setProjectionEvaluationCallback_out out_args;
    setProjectionEvaluationCallback(p, &in_args, &out_args);
    return out_args.result;
}

void setProjectionEvaluationCallback(SScriptCallBack *p, setProjectionEvaluationCallback_out *out_args, int taskHandle, std::string callback, int projectionSize)
{
    setProjectionEvaluationCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    in_args.projectionSize = projectionSize;
    setProjectionEvaluationCallback(p, &in_args, out_args);
}

void setProjectionEvaluationCallback_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setProjectionEvaluationCallback";

    setProjectionEvaluationCallback_in in_args;
    setProjectionEvaluationCallback_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 3)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (callback) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.callback));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (callback)";
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (projectionSize) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.projectionSize));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (projectionSize)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setProjectionEvaluationCallback(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setStateValidationCallback_in::setStateValidationCallback_in()
{
}

setStateValidationCallback_out::setStateValidationCallback_out()
{
    result = 0;
}

void setStateValidationCallback(SScriptCallBack *p, setStateValidationCallback_in *in_args, setStateValidationCallback_out *out_args)
{
    setStateValidationCallback(p, "simExtOMPL_setStateValidationCallback", in_args, out_args);
}

int setStateValidationCallback(SScriptCallBack *p, int taskHandle, std::string callback)
{
    setStateValidationCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    setStateValidationCallback_out out_args;
    setStateValidationCallback(p, &in_args, &out_args);
    return out_args.result;
}

void setStateValidationCallback(SScriptCallBack *p, setStateValidationCallback_out *out_args, int taskHandle, std::string callback)
{
    setStateValidationCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    setStateValidationCallback(p, &in_args, out_args);
}

void setStateValidationCallback_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setStateValidationCallback";

    setStateValidationCallback_in in_args;
    setStateValidationCallback_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (callback) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.callback));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (callback)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setStateValidationCallback(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setGoalCallback_in::setGoalCallback_in()
{
}

setGoalCallback_out::setGoalCallback_out()
{
    result = 0;
}

void setGoalCallback(SScriptCallBack *p, setGoalCallback_in *in_args, setGoalCallback_out *out_args)
{
    setGoalCallback(p, "simExtOMPL_setGoalCallback", in_args, out_args);
}

int setGoalCallback(SScriptCallBack *p, int taskHandle, std::string callback)
{
    setGoalCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    setGoalCallback_out out_args;
    setGoalCallback(p, &in_args, &out_args);
    return out_args.result;
}

void setGoalCallback(SScriptCallBack *p, setGoalCallback_out *out_args, int taskHandle, std::string callback)
{
    setGoalCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    setGoalCallback(p, &in_args, out_args);
}

void setGoalCallback_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setGoalCallback";

    setGoalCallback_in in_args;
    setGoalCallback_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 2)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (callback) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.callback));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (callback)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setGoalCallback(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

setValidStateSamplerCallback_in::setValidStateSamplerCallback_in()
{
}

setValidStateSamplerCallback_out::setValidStateSamplerCallback_out()
{
    result = 0;
}

void setValidStateSamplerCallback(SScriptCallBack *p, setValidStateSamplerCallback_in *in_args, setValidStateSamplerCallback_out *out_args)
{
    setValidStateSamplerCallback(p, "simExtOMPL_setValidStateSamplerCallback", in_args, out_args);
}

int setValidStateSamplerCallback(SScriptCallBack *p, int taskHandle, std::string callback, std::string callbackNear)
{
    setValidStateSamplerCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    in_args.callbackNear = callbackNear;
    setValidStateSamplerCallback_out out_args;
    setValidStateSamplerCallback(p, &in_args, &out_args);
    return out_args.result;
}

void setValidStateSamplerCallback(SScriptCallBack *p, setValidStateSamplerCallback_out *out_args, int taskHandle, std::string callback, std::string callbackNear)
{
    setValidStateSamplerCallback_in in_args;
    in_args.taskHandle = taskHandle;
    in_args.callback = callback;
    in_args.callbackNear = callbackNear;
    setValidStateSamplerCallback(p, &in_args, out_args);
}

void setValidStateSamplerCallback_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtOMPL_setValidStateSamplerCallback";

    setValidStateSamplerCallback_in in_args;
    setValidStateSamplerCallback_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 3)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (taskHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.taskHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (taskHandle)";
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (callback) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.callback));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (callback)";
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (callbackNear) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.callbackNear));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (callbackNear)";
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setValidStateSamplerCallback(p, cmd, &in_args, &out_args);
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);

        // write output arguments to stack

        try
        {
            // write output argument 1 (result) of type int
            write__int(out_args.result, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing output argument 1 (result)";
            throw exception(msg);
        }
    }
    catch(exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

goalCallback_in::goalCallback_in()
{
}

goalCallback_out::goalCallback_out()
{
}

bool goalCallback(simInt scriptId, const char *func, goalCallback_in *in_args, goalCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();
        
        // write input arguments to stack
        
        try
        {
            // write input argument 1 (state) of type array of std::vector<float>
            simPushTableOntoStackE(stackID);
            for(int i = 0; i < in_args->state.size(); i++)
            {
                write__int(i + 1, stackID);
                write__float(in_args->state[i], stackID);
                simInsertDataIntoStackTableE(stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (state)";
            throw exception(msg);
        }

        
        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack
        
        try
        {
            // read output argument 1 (satisfied) of type bool
            read__bool(stackID, &(out_args->satisfied));
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 1 (satisfied)";
            throw exception(msg);
        }
        try
        {
            // read output argument 2 (distance) of type float
            read__float(stackID, &(out_args->distance));
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 2 (distance)";
            throw exception(msg);
        }

        simReleaseStackE(stackID);
        stackID = -1;
    }
    catch(std::exception& ex)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, ex.what());
        return false;
    }
    
    return true;
}

projectionEvaluationCallback_in::projectionEvaluationCallback_in()
{
}

projectionEvaluationCallback_out::projectionEvaluationCallback_out()
{
}

bool projectionEvaluationCallback(simInt scriptId, const char *func, projectionEvaluationCallback_in *in_args, projectionEvaluationCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();
        
        // write input arguments to stack
        
        try
        {
            // write input argument 1 (state) of type array of std::vector<float>
            simPushTableOntoStackE(stackID);
            for(int i = 0; i < in_args->state.size(); i++)
            {
                write__int(i + 1, stackID);
                write__float(in_args->state[i], stackID);
                simInsertDataIntoStackTableE(stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (state)";
            throw exception(msg);
        }

        
        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack
        
        try
        {
            // read output argument 1 (projection) of type array of std::vector<float>
            simMoveStackItemToTopE(stackID, 0);
            int i = simGetStackTableInfoE(stackID, 0);
            if(i < 0)
                throw exception("expected array");
            int oldsz = simGetStackSizeE(stackID);
            simUnfoldStackTableE(stackID);
            int sz = (simGetStackSizeE(stackID) - oldsz + 1) / 2;
            for(int i = 0; i < sz; i++)
            {
                simMoveStackItemToTopE(stackID, oldsz - 1);
                int j;
                read__int(stackID, &j);
                simMoveStackItemToTop(stackID, oldsz - 1);
                float v;
                read__float(stackID, &v);
                out_args->projection.push_back(v);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 1 (projection)";
            throw exception(msg);
        }

        simReleaseStackE(stackID);
        stackID = -1;
    }
    catch(std::exception& ex)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, ex.what());
        return false;
    }
    
    return true;
}

stateValidationCallback_in::stateValidationCallback_in()
{
}

stateValidationCallback_out::stateValidationCallback_out()
{
}

bool stateValidationCallback(simInt scriptId, const char *func, stateValidationCallback_in *in_args, stateValidationCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();
        
        // write input arguments to stack
        
        try
        {
            // write input argument 1 (state) of type array of std::vector<float>
            simPushTableOntoStackE(stackID);
            for(int i = 0; i < in_args->state.size(); i++)
            {
                write__int(i + 1, stackID);
                write__float(in_args->state[i], stackID);
                simInsertDataIntoStackTableE(stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (state)";
            throw exception(msg);
        }

        
        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack
        
        try
        {
            // read output argument 1 (valid) of type bool
            read__bool(stackID, &(out_args->valid));
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 1 (valid)";
            throw exception(msg);
        }

        simReleaseStackE(stackID);
        stackID = -1;
    }
    catch(std::exception& ex)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, ex.what());
        return false;
    }
    
    return true;
}

validStateSamplerCallback_in::validStateSamplerCallback_in()
{
}

validStateSamplerCallback_out::validStateSamplerCallback_out()
{
}

bool validStateSamplerCallback(simInt scriptId, const char *func, validStateSamplerCallback_in *in_args, validStateSamplerCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();
        
        // write input arguments to stack
        

        
        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack
        
        try
        {
            // read output argument 1 (sampledState) of type array of std::vector<float>
            simMoveStackItemToTopE(stackID, 0);
            int i = simGetStackTableInfoE(stackID, 0);
            if(i < 0)
                throw exception("expected array");
            int oldsz = simGetStackSizeE(stackID);
            simUnfoldStackTableE(stackID);
            int sz = (simGetStackSizeE(stackID) - oldsz + 1) / 2;
            for(int i = 0; i < sz; i++)
            {
                simMoveStackItemToTopE(stackID, oldsz - 1);
                int j;
                read__int(stackID, &j);
                simMoveStackItemToTop(stackID, oldsz - 1);
                float v;
                read__float(stackID, &v);
                out_args->sampledState.push_back(v);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 1 (sampledState)";
            throw exception(msg);
        }

        simReleaseStackE(stackID);
        stackID = -1;
    }
    catch(std::exception& ex)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, ex.what());
        return false;
    }
    
    return true;
}

validStateSamplerCallbackNear_in::validStateSamplerCallbackNear_in()
{
}

validStateSamplerCallbackNear_out::validStateSamplerCallbackNear_out()
{
}

bool validStateSamplerCallbackNear(simInt scriptId, const char *func, validStateSamplerCallbackNear_in *in_args, validStateSamplerCallbackNear_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();
        
        // write input arguments to stack
        
        try
        {
            // write input argument 1 (state) of type array of std::vector<float>
            simPushTableOntoStackE(stackID);
            for(int i = 0; i < in_args->state.size(); i++)
            {
                write__int(i + 1, stackID);
                write__float(in_args->state[i], stackID);
                simInsertDataIntoStackTableE(stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (state)";
            throw exception(msg);
        }
        try
        {
            // write input argument 2 (distance) of type float
            write__float(in_args->distance, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (distance)";
            throw exception(msg);
        }

        
        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack
        
        try
        {
            // read output argument 1 (sampledState) of type array of std::vector<float>
            simMoveStackItemToTopE(stackID, 0);
            int i = simGetStackTableInfoE(stackID, 0);
            if(i < 0)
                throw exception("expected array");
            int oldsz = simGetStackSizeE(stackID);
            simUnfoldStackTableE(stackID);
            int sz = (simGetStackSizeE(stackID) - oldsz + 1) / 2;
            for(int i = 0; i < sz; i++)
            {
                simMoveStackItemToTopE(stackID, oldsz - 1);
                int j;
                read__int(stackID, &j);
                simMoveStackItemToTop(stackID, oldsz - 1);
                float v;
                read__float(stackID, &v);
                out_args->sampledState.push_back(v);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when reading output argument 1 (sampledState)";
            throw exception(msg);
        }

        simReleaseStackE(stackID);
        stackID = -1;
    }
    catch(std::exception& ex)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, ex.what());
        return false;
    }
    
    return true;
}

