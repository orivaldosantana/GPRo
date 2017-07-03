#include "stubs.h"

#include <iostream>

simInt simRegisterScriptCallbackFunctionE(const simChar *funcNameAtPluginName, const simChar *callTips, simVoid (*callBack)(struct SScriptCallBack *cb))
{
    simInt ret = simRegisterScriptCallbackFunction(funcNameAtPluginName, callTips, callBack);
    if(ret == 0)
    {
        std::cout << "Plugin 'SDF': warning: replaced function '" << funcNameAtPluginName << "'" << std::endl;
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
        std::cout << "Plugin 'SDF': warning: replaced variable '" << varName << "'" << std::endl;
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

void write__std__string(std::string value, int stack)
{
    const simChar *v = value.c_str();
    simPushStringOntoStackE(stack, v, value.length());
}

bool registerScriptStuff()
{
    try
    {
        simRegisterScriptCallbackFunctionE("simExtSDF_import@SDF", "simExtSDF_import(string fileName,bool ignoreMissingValues=false,bool hideCollisionLinks=true,bool hideJoints=true,bool convexDecompose=true,bool showConvexDecompositionDlg=false,bool createVisualIfNone=true,bool centerModel=true,bool prepareModel=true,bool noSelfCollision=true,bool positionCtrl=true)", import_callback);
        simRegisterScriptCallbackFunctionE("simExtSDF_dump@SDF", "simExtSDF_dump(string fileName)", dump_callback);
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

import_in::import_in()
{
    ignoreMissingValues = false;
    hideCollisionLinks = true;
    hideJoints = true;
    convexDecompose = true;
    showConvexDecompositionDlg = false;
    createVisualIfNone = true;
    centerModel = true;
    prepareModel = true;
    noSelfCollision = true;
    positionCtrl = true;
}

import_out::import_out()
{
}

void import(SScriptCallBack *p, import_in *in_args, import_out *out_args)
{
    import(p, "simExtSDF_import", in_args, out_args);
}

void import(SScriptCallBack *p, std::string fileName, bool ignoreMissingValues, bool hideCollisionLinks, bool hideJoints, bool convexDecompose, bool showConvexDecompositionDlg, bool createVisualIfNone, bool centerModel, bool prepareModel, bool noSelfCollision, bool positionCtrl)
{
    import_in in_args;
    in_args.fileName = fileName;
    in_args.ignoreMissingValues = ignoreMissingValues;
    in_args.hideCollisionLinks = hideCollisionLinks;
    in_args.hideJoints = hideJoints;
    in_args.convexDecompose = convexDecompose;
    in_args.showConvexDecompositionDlg = showConvexDecompositionDlg;
    in_args.createVisualIfNone = createVisualIfNone;
    in_args.centerModel = centerModel;
    in_args.prepareModel = prepareModel;
    in_args.noSelfCollision = noSelfCollision;
    in_args.positionCtrl = positionCtrl;
    import_out out_args;
    import(p, &in_args, &out_args);
}

void import(SScriptCallBack *p, import_out *out_args, std::string fileName, bool ignoreMissingValues, bool hideCollisionLinks, bool hideJoints, bool convexDecompose, bool showConvexDecompositionDlg, bool createVisualIfNone, bool centerModel, bool prepareModel, bool noSelfCollision, bool positionCtrl)
{
    import_in in_args;
    in_args.fileName = fileName;
    in_args.ignoreMissingValues = ignoreMissingValues;
    in_args.hideCollisionLinks = hideCollisionLinks;
    in_args.hideJoints = hideJoints;
    in_args.convexDecompose = convexDecompose;
    in_args.showConvexDecompositionDlg = showConvexDecompositionDlg;
    in_args.createVisualIfNone = createVisualIfNone;
    in_args.centerModel = centerModel;
    in_args.prepareModel = prepareModel;
    in_args.noSelfCollision = noSelfCollision;
    in_args.positionCtrl = positionCtrl;
    import(p, &in_args, out_args);
}

void import_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtSDF_import";

    import_in in_args;
    import_out out_args;

    try
    {
        // check argument count
        
        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 1)
            throw exception("too few arguments");
        if(numArgs > 11)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (fileName) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.fileName));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (fileName)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 1 (fileName))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 1 (fileName)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (ignoreMissingValues) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.ignoreMissingValues));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 2 (ignoreMissingValues)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 2 (ignoreMissingValues))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 2 (ignoreMissingValues)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (hideCollisionLinks) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.hideCollisionLinks));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 3 (hideCollisionLinks)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 3 (hideCollisionLinks))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 3 (hideCollisionLinks)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (hideJoints) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.hideJoints));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 4 (hideJoints)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 4 (hideJoints))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 4 (hideJoints)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (convexDecompose) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.convexDecompose));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 5 (convexDecompose)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 5 (convexDecompose))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 5 (convexDecompose)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (showConvexDecompositionDlg) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.showConvexDecompositionDlg));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 6 (showConvexDecompositionDlg)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 6 (showConvexDecompositionDlg))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 6 (showConvexDecompositionDlg)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 7)
        {
            try
            {
                // read input argument 7 (createVisualIfNone) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.createVisualIfNone));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 7 (createVisualIfNone)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 7 (createVisualIfNone))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 7 (createVisualIfNone)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 8)
        {
            try
            {
                // read input argument 8 (centerModel) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.centerModel));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 8 (centerModel)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 8 (centerModel))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 8 (centerModel)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 9)
        {
            try
            {
                // read input argument 9 (prepareModel) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.prepareModel));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 9 (prepareModel)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 9 (prepareModel))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 9 (prepareModel)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 10)
        {
            try
            {
                // read input argument 10 (noSelfCollision) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.noSelfCollision));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 10 (noSelfCollision)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 10 (noSelfCollision))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 10 (noSelfCollision)";
                throw exception(ss.str());
            }
        }


        if(numArgs >= 11)
        {
            try
            {
                // read input argument 11 (positionCtrl) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.positionCtrl));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 11 (positionCtrl)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 11 (positionCtrl))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 11 (positionCtrl)";
                throw exception(ss.str());
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        import(p, cmd, &in_args, &out_args);
    }
    catch(std::exception& e)
    {
        simSetLastError(cmd, e.what());
    }
    catch(std::string& s)
    {
        simSetLastError(cmd, s.c_str());
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(cmd, ss.str().c_str());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);


        // write output arguments to stack

    }
    catch(std::exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(std::string& s)
    {
        simSetLastError(cmd, s.c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(cmd, ss.str().c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

dump_in::dump_in()
{
}

dump_out::dump_out()
{
}

void dump(SScriptCallBack *p, dump_in *in_args, dump_out *out_args)
{
    dump(p, "simExtSDF_dump", in_args, out_args);
}

void dump(SScriptCallBack *p, std::string fileName)
{
    dump_in in_args;
    in_args.fileName = fileName;
    dump_out out_args;
    dump(p, &in_args, &out_args);
}

void dump(SScriptCallBack *p, dump_out *out_args, std::string fileName)
{
    dump_in in_args;
    in_args.fileName = fileName;
    dump(p, &in_args, out_args);
}

void dump_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtSDF_dump";

    dump_in in_args;
    dump_out out_args;

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
                // read input argument 1 (fileName) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.fileName));
            }
            catch(std::exception& ex)
            {
                std::string msg = ex.what();
                msg += " when reading input argument 1 (fileName)";
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = s;
                msg += " (when reading input argument 1 (fileName))";
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n << " when reading input argument 1 (fileName)";
                throw exception(ss.str());
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        dump(p, cmd, &in_args, &out_args);
    }
    catch(std::exception& e)
    {
        simSetLastError(cmd, e.what());
    }
    catch(std::string& s)
    {
        simSetLastError(cmd, s.c_str());
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(cmd, ss.str().c_str());
    }

    try
    {
        // clear stack
        simPopStackItemE(p->stackID, 0);


        // write output arguments to stack

    }
    catch(std::exception& e)
    {
        simSetLastError(cmd, e.what());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(std::string& s)
    {
        simSetLastError(cmd, s.c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
    catch(int& n)
    {
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(cmd, ss.str().c_str());
        // clear stack
        simPopStackItem(p->stackID, 0); // don't throw
    }
}

