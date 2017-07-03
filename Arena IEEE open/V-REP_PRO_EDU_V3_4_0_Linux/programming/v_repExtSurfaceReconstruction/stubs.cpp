#include "stubs.h"

#include <iostream>

simInt simRegisterScriptCallbackFunctionE(const simChar *funcNameAtPluginName, const simChar *callTips, simVoid (*callBack)(struct SScriptCallBack *cb))
{
    simInt ret = simRegisterScriptCallbackFunction(funcNameAtPluginName, callTips, callBack);
    if(ret == 0)
    {
        std::cout << "Plugin 'SurfaceReconstruction': warning: replaced function '" << funcNameAtPluginName << "'" << std::endl;
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
        std::cout << "Plugin 'SurfaceReconstruction': warning: replaced variable '" << varName << "'" << std::endl;
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

simVoid simPushUInt8TableOntoStackE(simInt stackHandle, const simUChar *values, simInt valueCnt)
{
    if(simPushUInt8TableOntoStack(stackHandle, values, valueCnt) == -1)
        throw exception("simPushUInt8TableOntoStack: error");
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
    // if stringSize is NULL, we cannot distinguish error (-1) from type error (0)
    if(ret == NULL && stringSize && *stringSize == -1)
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

simInt simGetStackUInt8TableE(simInt stackHandle, simUChar *array, simInt count)
{
    simInt ret = simGetStackUInt8Table(stackHandle, array, count);
    if(ret == -1)
        throw exception("simGetStackUInt8Table: error");
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

simInt simGetInt32ParameterE(simInt parameter)
{
    simInt ret;
    if(simGetInt32Parameter(parameter, &ret) == -1)
        throw exception("simGetInt32Parameter: error");
    return ret;
}

simChar* simCreateBufferE(simInt size)
{
    simChar *ret = simCreateBuffer(size);
    if(ret == NULL)
        throw exception("simCreateBuffer: error");
    return ret;
}

simVoid simReleaseBufferE(simChar *buffer)
{
    if(simReleaseBuffer(buffer) == -1)
        throw exception("simReleaseBuffer: error");
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

void read__double(int stack, double *value)
{
    simDouble v;
    if(simGetStackDoubleValueE(stack, &v) == 1)
    {
        *value = v;
        simPopStackItemE(stack, 1);
    }
    else
    {
        throw exception("expected double");
    }
}

void read__std__string(int stack, std::string *value)
{
    simChar *str;
    simInt strSize;
    if((str = simGetStackStringValueE(stack, &strSize)) != NULL && strSize >= 0)
    {
        *value = std::string(str, strSize);
        simPopStackItemE(stack, 1);
        simReleaseBufferE(str);
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

void write__double(double value, int stack)
{
    simDouble v = value;
    simPushDoubleOntoStackE(stack, v);
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
        simRegisterScriptVariableE("sim_surfacereconstruction_algorithm_scalespace", (boost::lexical_cast<std::string>(sim_surfacereconstruction_algorithm_scalespace)).c_str(), 0);
        simRegisterScriptVariableE("sim_surfacereconstruction_algorithm_poisson", (boost::lexical_cast<std::string>(sim_surfacereconstruction_algorithm_poisson)).c_str(), 0);
        simRegisterScriptVariableE("sim_surfacereconstruction_algorithm_advancingfront", (boost::lexical_cast<std::string>(sim_surfacereconstruction_algorithm_advancingfront)).c_str(), 0);
        simRegisterScriptCallbackFunctionE("simExtSurfaceReconstruction_reconstruct@SurfaceReconstruction", "number shapeHandle,number neighborhoodSquaredRadius=simExtSurfaceReconstruction_reconstruct(number pointCloudHandle,number algorithm=sim_surfacereconstruction_algorithm_scalespace)", reconstruct_callback);
    }
    catch(std::exception& ex)
    {
        std::cout << "Initialization failed (registerScriptStuff): " << ex.what() << std::endl;
        return false;
    }
    catch(std::string& s)
    {
        std::cout << "Initialization failed (registerScriptStuff): " << s << std::endl;
        return false;
    }
    catch(int& n)
    {
        std::cout << "Initialization failed (registerScriptStuff): error #" << n << std::endl;
        return false;
    }
    return true;
}

const char* algorithm_string(Algorithm x)
{
    switch(x)
    {
        case sim_surfacereconstruction_algorithm_scalespace: return "sim_surfacereconstruction_algorithm_scalespace";
        case sim_surfacereconstruction_algorithm_poisson: return "sim_surfacereconstruction_algorithm_poisson";
        case sim_surfacereconstruction_algorithm_advancingfront: return "sim_surfacereconstruction_algorithm_advancingfront";
        default: return "???";
    }
}

reconstruct_in::reconstruct_in()
{
    algorithm = sim_surfacereconstruction_algorithm_scalespace;
}

reconstruct_out::reconstruct_out()
{
}

void reconstruct(SScriptCallBack *p, reconstruct_in *in_args, reconstruct_out *out_args)
{
    reconstruct(p, "simExtSurfaceReconstruction_reconstruct", in_args, out_args);
}

void reconstruct(SScriptCallBack *p, reconstruct_out *out_args, int pointCloudHandle, int algorithm)
{
    reconstruct_in in_args;
    in_args.pointCloudHandle = pointCloudHandle;
    in_args.algorithm = algorithm;
    reconstruct(p, &in_args, out_args);
}

void reconstruct_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtSurfaceReconstruction_reconstruct";

    reconstruct_in in_args;
    reconstruct_out out_args;

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
                // read input argument 1 (pointCloudHandle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.pointCloudHandle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (pointCloudHandle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (pointCloudHandle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (pointCloudHandle): ";
                msg += ss.str();
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
                std::string msg = "read in arg 2 (algorithm): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (algorithm): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (algorithm): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        reconstruct(p, cmd, &in_args, &out_args);
    }
    catch(std::exception& e)
    {
#ifdef DEBUG
        std::cerr << cmd << ": " << e.what() << std::endl;
#endif
        simSetLastError(cmd, e.what());
    }
    catch(std::string& s)
    {
#ifdef DEBUG
        std::cerr << cmd << ": " << s << std::endl;
#endif
        simSetLastError(cmd, s.c_str());
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
#ifdef DEBUG
        std::cerr << cmd << ": " << ss.str() << std::endl;
#endif
        simSetLastError(cmd, ss.str().c_str());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);


        // write output arguments to stack

        try
        {
            // write output argument 1 (meshHandle) of type int
            write__int(out_args.meshHandle, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (meshHandle): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (meshHandle)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (meshHandle)): ";
            msg += ss.str();
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (neighborhoodSquaredRadius) of type float
            write__float(out_args.neighborhoodSquaredRadius, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 2 (neighborhoodSquaredRadius): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 2 (neighborhoodSquaredRadius)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 2 (neighborhoodSquaredRadius)): ";
            msg += ss.str();
            throw exception(msg);
        }
    }
    catch(std::exception& e)
    {
#ifdef DEBUG
        std::cerr << cmd << ": " << e.what() << std::endl;
#endif
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(std::string& s)
    {
#ifdef DEBUG
        std::cerr << cmd << ": " << s << std::endl;
#endif
        simSetLastError(cmd, s.c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
#ifdef DEBUG
        std::cerr << cmd << ": " << ss.str() << std::endl;
#endif
        simSetLastError(cmd, ss.str().c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

