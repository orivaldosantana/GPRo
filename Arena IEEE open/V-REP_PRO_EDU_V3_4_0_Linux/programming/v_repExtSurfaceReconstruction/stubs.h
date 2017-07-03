#ifndef STUBS_H__INCLUDED
#define STUBS_H__INCLUDED

#ifdef QT_COMPIL
#include <QThread>
#if defined(QT_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif
#endif // QT_COMPIL

#include <v_repLib.h>
#include <string>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>

struct exception : public std::exception
{
    std::string s;
    exception(std::string s_) : s(s_) {}
    ~exception() throw() {}
    const char* what() const throw() {return s.c_str();}
};

simInt simRegisterScriptCallbackFunctionE(const simChar *funcNameAtPluginName, const simChar *callTips, simVoid (*callBack)(struct SScriptCallBack *cb));
simInt simRegisterScriptVariableE(const simChar *varName, const simChar *varValue, simInt stackID);
simVoid simCallScriptFunctionExE(simInt scriptHandleOrType,const simChar* functionNameAtScriptName,simInt stackId);
simInt simCreateStackE();
simVoid simReleaseStackE(simInt stackHandle);
simInt simCopyStackE(simInt stackHandle);
simVoid simPushNullOntoStackE(simInt stackHandle);
simVoid simPushBoolOntoStackE(simInt stackHandle, simBool value);
simVoid simPushInt32OntoStackE(simInt stackHandle, simInt value);
simVoid simPushFloatOntoStackE(simInt stackHandle, simFloat value);
simVoid simPushDoubleOntoStackE(simInt stackHandle, simDouble value);
simVoid simPushStringOntoStackE(simInt stackHandle, const simChar *value, simInt stringSize);
simVoid simPushUInt8TableOntoStackE(simInt stackHandle, const simUChar *values, simInt valueCnt);
simVoid simPushInt32TableOntoStackE(simInt stackHandle, const simInt *values, simInt valueCnt);
simVoid simPushFloatTableOntoStackE(simInt stackHandle, const simFloat *values, simInt valueCnt);
simVoid simPushDoubleTableOntoStackE(simInt stackHandle, const simDouble *values, simInt valueCnt);
simVoid simPushTableOntoStackE(simInt stackHandle);
simVoid simInsertDataIntoStackTableE(simInt stackHandle);
simInt simGetStackSizeE(simInt stackHandle);
simInt simPopStackItemE(simInt stackHandle, simInt count);
simVoid simMoveStackItemToTopE(simInt stackHandle, simInt cIndex);
simInt simIsStackValueNullE(simInt stackHandle);
simInt simGetStackBoolValueE(simInt stackHandle, simBool *boolValue);
simInt simGetStackInt32ValueE(simInt stackHandle, simInt *numberValue);
simInt simGetStackFloatValueE(simInt stackHandle, simFloat *numberValue);
simInt simGetStackDoubleValueE(simInt stackHandle, simDouble *numberValue);
simChar* simGetStackStringValueE(simInt stackHandle, simInt *stringSize);
simInt simGetStackTableInfoE(simInt stackHandle, simInt infoType);
simInt simGetStackUInt8TableE(simInt stackHandle, simUChar *array, simInt count);
simInt simGetStackInt32TableE(simInt stackHandle, simInt *array, simInt count);
simInt simGetStackFloatTableE(simInt stackHandle, simFloat *array, simInt count);
simInt simGetStackDoubleTableE(simInt stackHandle, simDouble *array, simInt count);
simVoid simUnfoldStackTableE(simInt stackHandle);
simInt simGetInt32ParameterE(simInt parameter);
simChar* simCreateBufferE(simInt size);
simVoid simReleaseBufferE(simChar *buffer);


void read__bool(int stack, bool *value);
void read__int(int stack, int *value);
void read__float(int stack, float *value);
void read__double(int stack, double *value);
void read__std__string(int stack, std::string *value);
void write__bool(bool value, int stack);
void write__int(int value, int stack);
void write__float(float value, int stack);
void write__double(double value, int stack);
void write__std__string(std::string value, int stack);

bool registerScriptStuff();

enum Algorithm
{
    sim_surfacereconstruction_algorithm_scalespace = 6785,
    sim_surfacereconstruction_algorithm_poisson,
    sim_surfacereconstruction_algorithm_advancingfront,
};

const char* algorithm_string(Algorithm x);

struct reconstruct_in
{
    int pointCloudHandle;
    int algorithm;
    reconstruct_in();
};

struct reconstruct_out
{
    int meshHandle;
    float neighborhoodSquaredRadius;
    reconstruct_out();
};

void reconstruct(SScriptCallBack *p, reconstruct_in *in, reconstruct_out *out);
void reconstruct(SScriptCallBack *p, reconstruct_out *out, int pointCloudHandle, int algorithm = sim_surfacereconstruction_algorithm_scalespace);
void reconstruct_callback(SScriptCallBack *p);


// following functions must be implemented in the plugin

void reconstruct(SScriptCallBack *p, const char *cmd, reconstruct_in *in, reconstruct_out *out);

#endif // STUBS_H__INCLUDED
