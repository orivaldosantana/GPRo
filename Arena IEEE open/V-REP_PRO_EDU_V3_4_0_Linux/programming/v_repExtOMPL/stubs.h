#ifndef STUBS_H__INCLUDED
#define STUBS_H__INCLUDED

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
simInt simGetStackInt32TableE(simInt stackHandle, simInt *array, simInt count);
simInt simGetStackFloatTableE(simInt stackHandle, simFloat *array, simInt count);
simInt simGetStackDoubleTableE(simInt stackHandle, simDouble *array, simInt count);
simVoid simUnfoldStackTableE(simInt stackHandle);


void read__bool(int stack, bool *value);
void read__int(int stack, int *value);
void read__float(int stack, float *value);
void read__std__string(int stack, std::string *value);
void write__bool(bool value, int stack);
void write__int(int value, int stack);
void write__float(float value, int stack);
void write__std__string(std::string value, int stack);

bool registerScriptStuff();

enum Algorithm
{
    sim_ompl_algorithm_BiTRRT = 30001,
    sim_ompl_algorithm_BITstar,
    sim_ompl_algorithm_BKPIECE1,
    sim_ompl_algorithm_CForest,
    sim_ompl_algorithm_EST,
    sim_ompl_algorithm_FMT,
    sim_ompl_algorithm_KPIECE1,
    sim_ompl_algorithm_LazyPRM,
    sim_ompl_algorithm_LazyPRMstar,
    sim_ompl_algorithm_LazyRRT,
    sim_ompl_algorithm_LBKPIECE1,
    sim_ompl_algorithm_LBTRRT,
    sim_ompl_algorithm_PDST,
    sim_ompl_algorithm_PRM,
    sim_ompl_algorithm_PRMstar,
    sim_ompl_algorithm_pRRT,
    sim_ompl_algorithm_pSBL,
    sim_ompl_algorithm_RRT,
    sim_ompl_algorithm_RRTConnect,
    sim_ompl_algorithm_RRTstar,
    sim_ompl_algorithm_SBL,
    sim_ompl_algorithm_SPARS,
    sim_ompl_algorithm_SPARStwo,
    sim_ompl_algorithm_STRIDE,
    sim_ompl_algorithm_TRRT,
};

const char* algorithm_string(Algorithm x);

enum StateSpaceType
{
    sim_ompl_statespacetype_position2d = 50001,
    sim_ompl_statespacetype_pose2d,
    sim_ompl_statespacetype_position3d,
    sim_ompl_statespacetype_pose3d,
    sim_ompl_statespacetype_joint_position,
};

const char* statespacetype_string(StateSpaceType x);

struct createStateSpace_in
{
    std::string name;
    int type;
    int objectHandle;
    std::vector<float> boundsLow;
    std::vector<float> boundsHigh;
    int useForProjection;
    float weight;
    int refObjectHandle;
    createStateSpace_in();
};

struct createStateSpace_out
{
    int stateSpaceHandle;
    createStateSpace_out();
};

void createStateSpace(SScriptCallBack *p, createStateSpace_in *in, createStateSpace_out *out);
int createStateSpace(SScriptCallBack *p, std::string name, int type, int objectHandle, std::vector<float> boundsLow, std::vector<float> boundsHigh, int useForProjection, float weight = 1.0, int refObjectHandle = -1);
void createStateSpace(SScriptCallBack *p, createStateSpace_out *out, std::string name, int type, int objectHandle, std::vector<float> boundsLow, std::vector<float> boundsHigh, int useForProjection, float weight = 1.0, int refObjectHandle = -1);
void createStateSpace_callback(SScriptCallBack *p);

struct destroyStateSpace_in
{
    int stateSpaceHandle;
    destroyStateSpace_in();
};

struct destroyStateSpace_out
{
    int result;
    destroyStateSpace_out();
};

void destroyStateSpace(SScriptCallBack *p, destroyStateSpace_in *in, destroyStateSpace_out *out);
int destroyStateSpace(SScriptCallBack *p, int stateSpaceHandle);
void destroyStateSpace(SScriptCallBack *p, destroyStateSpace_out *out, int stateSpaceHandle);
void destroyStateSpace_callback(SScriptCallBack *p);

struct createTask_in
{
    std::string name;
    createTask_in();
};

struct createTask_out
{
    int taskHandle;
    createTask_out();
};

void createTask(SScriptCallBack *p, createTask_in *in, createTask_out *out);
int createTask(SScriptCallBack *p, std::string name);
void createTask(SScriptCallBack *p, createTask_out *out, std::string name);
void createTask_callback(SScriptCallBack *p);

struct destroyTask_in
{
    int taskHandle;
    destroyTask_in();
};

struct destroyTask_out
{
    int result;
    destroyTask_out();
};

void destroyTask(SScriptCallBack *p, destroyTask_in *in, destroyTask_out *out);
int destroyTask(SScriptCallBack *p, int taskHandle);
void destroyTask(SScriptCallBack *p, destroyTask_out *out, int taskHandle);
void destroyTask_callback(SScriptCallBack *p);

struct printTaskInfo_in
{
    int taskHandle;
    printTaskInfo_in();
};

struct printTaskInfo_out
{
    int result;
    printTaskInfo_out();
};

void printTaskInfo(SScriptCallBack *p, printTaskInfo_in *in, printTaskInfo_out *out);
int printTaskInfo(SScriptCallBack *p, int taskHandle);
void printTaskInfo(SScriptCallBack *p, printTaskInfo_out *out, int taskHandle);
void printTaskInfo_callback(SScriptCallBack *p);

struct setVerboseLevel_in
{
    int taskHandle;
    int verboseLevel;
    setVerboseLevel_in();
};

struct setVerboseLevel_out
{
    int result;
    setVerboseLevel_out();
};

void setVerboseLevel(SScriptCallBack *p, setVerboseLevel_in *in, setVerboseLevel_out *out);
int setVerboseLevel(SScriptCallBack *p, int taskHandle, int verboseLevel);
void setVerboseLevel(SScriptCallBack *p, setVerboseLevel_out *out, int taskHandle, int verboseLevel);
void setVerboseLevel_callback(SScriptCallBack *p);

struct setStateValidityCheckingResolution_in
{
    int taskHandle;
    float resolution;
    setStateValidityCheckingResolution_in();
};

struct setStateValidityCheckingResolution_out
{
    int result;
    setStateValidityCheckingResolution_out();
};

void setStateValidityCheckingResolution(SScriptCallBack *p, setStateValidityCheckingResolution_in *in, setStateValidityCheckingResolution_out *out);
int setStateValidityCheckingResolution(SScriptCallBack *p, int taskHandle, float resolution);
void setStateValidityCheckingResolution(SScriptCallBack *p, setStateValidityCheckingResolution_out *out, int taskHandle, float resolution);
void setStateValidityCheckingResolution_callback(SScriptCallBack *p);

struct setStateSpace_in
{
    int taskHandle;
    std::vector<int> stateSpaceHandles;
    setStateSpace_in();
};

struct setStateSpace_out
{
    int result;
    setStateSpace_out();
};

void setStateSpace(SScriptCallBack *p, setStateSpace_in *in, setStateSpace_out *out);
int setStateSpace(SScriptCallBack *p, int taskHandle, std::vector<int> stateSpaceHandles);
void setStateSpace(SScriptCallBack *p, setStateSpace_out *out, int taskHandle, std::vector<int> stateSpaceHandles);
void setStateSpace_callback(SScriptCallBack *p);

struct setAlgorithm_in
{
    int taskHandle;
    int algorithm;
    setAlgorithm_in();
};

struct setAlgorithm_out
{
    int result;
    setAlgorithm_out();
};

void setAlgorithm(SScriptCallBack *p, setAlgorithm_in *in, setAlgorithm_out *out);
int setAlgorithm(SScriptCallBack *p, int taskHandle, int algorithm);
void setAlgorithm(SScriptCallBack *p, setAlgorithm_out *out, int taskHandle, int algorithm);
void setAlgorithm_callback(SScriptCallBack *p);

struct setCollisionPairs_in
{
    int taskHandle;
    std::vector<int> collisionPairHandles;
    setCollisionPairs_in();
};

struct setCollisionPairs_out
{
    int result;
    setCollisionPairs_out();
};

void setCollisionPairs(SScriptCallBack *p, setCollisionPairs_in *in, setCollisionPairs_out *out);
int setCollisionPairs(SScriptCallBack *p, int taskHandle, std::vector<int> collisionPairHandles);
void setCollisionPairs(SScriptCallBack *p, setCollisionPairs_out *out, int taskHandle, std::vector<int> collisionPairHandles);
void setCollisionPairs_callback(SScriptCallBack *p);

struct setStartState_in
{
    int taskHandle;
    std::vector<float> state;
    setStartState_in();
};

struct setStartState_out
{
    int result;
    setStartState_out();
};

void setStartState(SScriptCallBack *p, setStartState_in *in, setStartState_out *out);
int setStartState(SScriptCallBack *p, int taskHandle, std::vector<float> state);
void setStartState(SScriptCallBack *p, setStartState_out *out, int taskHandle, std::vector<float> state);
void setStartState_callback(SScriptCallBack *p);

struct setGoalState_in
{
    int taskHandle;
    std::vector<float> state;
    setGoalState_in();
};

struct setGoalState_out
{
    int result;
    setGoalState_out();
};

void setGoalState(SScriptCallBack *p, setGoalState_in *in, setGoalState_out *out);
int setGoalState(SScriptCallBack *p, int taskHandle, std::vector<float> state);
void setGoalState(SScriptCallBack *p, setGoalState_out *out, int taskHandle, std::vector<float> state);
void setGoalState_callback(SScriptCallBack *p);

struct addGoalState_in
{
    int taskHandle;
    std::vector<float> state;
    addGoalState_in();
};

struct addGoalState_out
{
    int result;
    addGoalState_out();
};

void addGoalState(SScriptCallBack *p, addGoalState_in *in, addGoalState_out *out);
int addGoalState(SScriptCallBack *p, int taskHandle, std::vector<float> state);
void addGoalState(SScriptCallBack *p, addGoalState_out *out, int taskHandle, std::vector<float> state);
void addGoalState_callback(SScriptCallBack *p);

struct setGoal_in
{
    int taskHandle;
    int robotDummy;
    int goalDummy;
    float tolerance;
    std::vector<float> metric;
    int refDummy;
    setGoal_in();
};

struct setGoal_out
{
    int result;
    setGoal_out();
};

void setGoal(SScriptCallBack *p, setGoal_in *in, setGoal_out *out);
int setGoal(SScriptCallBack *p, int taskHandle, int robotDummy, int goalDummy, float tolerance = 0.001f, std::vector<float> metric = boost::assign::list_of(1.0)(1.0)(1.0)(0.1), int refDummy = -1);
void setGoal(SScriptCallBack *p, setGoal_out *out, int taskHandle, int robotDummy, int goalDummy, float tolerance = 0.001f, std::vector<float> metric = boost::assign::list_of(1.0)(1.0)(1.0)(0.1), int refDummy = -1);
void setGoal_callback(SScriptCallBack *p);

struct setup_in
{
    int taskHandle;
    setup_in();
};

struct setup_out
{
    int result;
    setup_out();
};

void setup(SScriptCallBack *p, setup_in *in, setup_out *out);
int setup(SScriptCallBack *p, int taskHandle);
void setup(SScriptCallBack *p, setup_out *out, int taskHandle);
void setup_callback(SScriptCallBack *p);

struct solve_in
{
    int taskHandle;
    float maxTime;
    solve_in();
};

struct solve_out
{
    int result;
    solve_out();
};

void solve(SScriptCallBack *p, solve_in *in, solve_out *out);
int solve(SScriptCallBack *p, int taskHandle, float maxTime);
void solve(SScriptCallBack *p, solve_out *out, int taskHandle, float maxTime);
void solve_callback(SScriptCallBack *p);

struct simplifyPath_in
{
    int taskHandle;
    float maxSimplificationTime;
    simplifyPath_in();
};

struct simplifyPath_out
{
    int result;
    simplifyPath_out();
};

void simplifyPath(SScriptCallBack *p, simplifyPath_in *in, simplifyPath_out *out);
int simplifyPath(SScriptCallBack *p, int taskHandle, float maxSimplificationTime = -1.0);
void simplifyPath(SScriptCallBack *p, simplifyPath_out *out, int taskHandle, float maxSimplificationTime = -1.0);
void simplifyPath_callback(SScriptCallBack *p);

struct interpolatePath_in
{
    int taskHandle;
    int stateCnt;
    interpolatePath_in();
};

struct interpolatePath_out
{
    int result;
    interpolatePath_out();
};

void interpolatePath(SScriptCallBack *p, interpolatePath_in *in, interpolatePath_out *out);
int interpolatePath(SScriptCallBack *p, int taskHandle, int stateCnt = 0);
void interpolatePath(SScriptCallBack *p, interpolatePath_out *out, int taskHandle, int stateCnt = 0);
void interpolatePath_callback(SScriptCallBack *p);

struct getPath_in
{
    int taskHandle;
    getPath_in();
};

struct getPath_out
{
    int result;
    std::vector<float> states;
    getPath_out();
};

void getPath(SScriptCallBack *p, getPath_in *in, getPath_out *out);
void getPath(SScriptCallBack *p, getPath_out *out, int taskHandle);
void getPath_callback(SScriptCallBack *p);

struct compute_in
{
    int taskHandle;
    float maxTime;
    float maxSimplificationTime;
    int stateCnt;
    compute_in();
};

struct compute_out
{
    int result;
    std::vector<float> states;
    compute_out();
};

void compute(SScriptCallBack *p, compute_in *in, compute_out *out);
void compute(SScriptCallBack *p, compute_out *out, int taskHandle, float maxTime, float maxSimplificationTime = -1.0, int stateCnt = 0);
void compute_callback(SScriptCallBack *p);

struct readState_in
{
    int taskHandle;
    readState_in();
};

struct readState_out
{
    int result;
    std::vector<float> state;
    readState_out();
};

void readState(SScriptCallBack *p, readState_in *in, readState_out *out);
void readState(SScriptCallBack *p, readState_out *out, int taskHandle);
void readState_callback(SScriptCallBack *p);

struct writeState_in
{
    int taskHandle;
    std::vector<float> state;
    writeState_in();
};

struct writeState_out
{
    int result;
    writeState_out();
};

void writeState(SScriptCallBack *p, writeState_in *in, writeState_out *out);
int writeState(SScriptCallBack *p, int taskHandle, std::vector<float> state);
void writeState(SScriptCallBack *p, writeState_out *out, int taskHandle, std::vector<float> state);
void writeState_callback(SScriptCallBack *p);

struct isStateValid_in
{
    int taskHandle;
    std::vector<float> state;
    isStateValid_in();
};

struct isStateValid_out
{
    int valid;
    isStateValid_out();
};

void isStateValid(SScriptCallBack *p, isStateValid_in *in, isStateValid_out *out);
int isStateValid(SScriptCallBack *p, int taskHandle, std::vector<float> state);
void isStateValid(SScriptCallBack *p, isStateValid_out *out, int taskHandle, std::vector<float> state);
void isStateValid_callback(SScriptCallBack *p);

struct setProjectionEvaluationCallback_in
{
    int taskHandle;
    std::string callback;
    int projectionSize;
    setProjectionEvaluationCallback_in();
};

struct setProjectionEvaluationCallback_out
{
    int result;
    setProjectionEvaluationCallback_out();
};

void setProjectionEvaluationCallback(SScriptCallBack *p, setProjectionEvaluationCallback_in *in, setProjectionEvaluationCallback_out *out);
int setProjectionEvaluationCallback(SScriptCallBack *p, int taskHandle, std::string callback, int projectionSize);
void setProjectionEvaluationCallback(SScriptCallBack *p, setProjectionEvaluationCallback_out *out, int taskHandle, std::string callback, int projectionSize);
void setProjectionEvaluationCallback_callback(SScriptCallBack *p);

struct setStateValidationCallback_in
{
    int taskHandle;
    std::string callback;
    setStateValidationCallback_in();
};

struct setStateValidationCallback_out
{
    int result;
    setStateValidationCallback_out();
};

void setStateValidationCallback(SScriptCallBack *p, setStateValidationCallback_in *in, setStateValidationCallback_out *out);
int setStateValidationCallback(SScriptCallBack *p, int taskHandle, std::string callback);
void setStateValidationCallback(SScriptCallBack *p, setStateValidationCallback_out *out, int taskHandle, std::string callback);
void setStateValidationCallback_callback(SScriptCallBack *p);

struct setGoalCallback_in
{
    int taskHandle;
    std::string callback;
    setGoalCallback_in();
};

struct setGoalCallback_out
{
    int result;
    setGoalCallback_out();
};

void setGoalCallback(SScriptCallBack *p, setGoalCallback_in *in, setGoalCallback_out *out);
int setGoalCallback(SScriptCallBack *p, int taskHandle, std::string callback);
void setGoalCallback(SScriptCallBack *p, setGoalCallback_out *out, int taskHandle, std::string callback);
void setGoalCallback_callback(SScriptCallBack *p);

struct setValidStateSamplerCallback_in
{
    int taskHandle;
    std::string callback;
    std::string callbackNear;
    setValidStateSamplerCallback_in();
};

struct setValidStateSamplerCallback_out
{
    int result;
    setValidStateSamplerCallback_out();
};

void setValidStateSamplerCallback(SScriptCallBack *p, setValidStateSamplerCallback_in *in, setValidStateSamplerCallback_out *out);
int setValidStateSamplerCallback(SScriptCallBack *p, int taskHandle, std::string callback, std::string callbackNear);
void setValidStateSamplerCallback(SScriptCallBack *p, setValidStateSamplerCallback_out *out, int taskHandle, std::string callback, std::string callbackNear);
void setValidStateSamplerCallback_callback(SScriptCallBack *p);

struct goalCallback_in
{
    std::vector<float> state;
    goalCallback_in();
};

struct goalCallback_out
{
    bool satisfied;
    float distance;
    goalCallback_out();
};

bool goalCallback(simInt scriptId, const char *func, goalCallback_in *in_args, goalCallback_out *out_args);

struct projectionEvaluationCallback_in
{
    std::vector<float> state;
    projectionEvaluationCallback_in();
};

struct projectionEvaluationCallback_out
{
    std::vector<float> projection;
    projectionEvaluationCallback_out();
};

bool projectionEvaluationCallback(simInt scriptId, const char *func, projectionEvaluationCallback_in *in_args, projectionEvaluationCallback_out *out_args);

struct stateValidationCallback_in
{
    std::vector<float> state;
    stateValidationCallback_in();
};

struct stateValidationCallback_out
{
    bool valid;
    stateValidationCallback_out();
};

bool stateValidationCallback(simInt scriptId, const char *func, stateValidationCallback_in *in_args, stateValidationCallback_out *out_args);

struct validStateSamplerCallback_in
{
    validStateSamplerCallback_in();
};

struct validStateSamplerCallback_out
{
    std::vector<float> sampledState;
    validStateSamplerCallback_out();
};

bool validStateSamplerCallback(simInt scriptId, const char *func, validStateSamplerCallback_in *in_args, validStateSamplerCallback_out *out_args);

struct validStateSamplerCallbackNear_in
{
    std::vector<float> state;
    float distance;
    validStateSamplerCallbackNear_in();
};

struct validStateSamplerCallbackNear_out
{
    std::vector<float> sampledState;
    validStateSamplerCallbackNear_out();
};

bool validStateSamplerCallbackNear(simInt scriptId, const char *func, validStateSamplerCallbackNear_in *in_args, validStateSamplerCallbackNear_out *out_args);


// following functions must be implemented in the plugin

void createStateSpace(SScriptCallBack *p, const char *cmd, createStateSpace_in *in, createStateSpace_out *out);
void destroyStateSpace(SScriptCallBack *p, const char *cmd, destroyStateSpace_in *in, destroyStateSpace_out *out);
void createTask(SScriptCallBack *p, const char *cmd, createTask_in *in, createTask_out *out);
void destroyTask(SScriptCallBack *p, const char *cmd, destroyTask_in *in, destroyTask_out *out);
void printTaskInfo(SScriptCallBack *p, const char *cmd, printTaskInfo_in *in, printTaskInfo_out *out);
void setVerboseLevel(SScriptCallBack *p, const char *cmd, setVerboseLevel_in *in, setVerboseLevel_out *out);
void setStateValidityCheckingResolution(SScriptCallBack *p, const char *cmd, setStateValidityCheckingResolution_in *in, setStateValidityCheckingResolution_out *out);
void setStateSpace(SScriptCallBack *p, const char *cmd, setStateSpace_in *in, setStateSpace_out *out);
void setAlgorithm(SScriptCallBack *p, const char *cmd, setAlgorithm_in *in, setAlgorithm_out *out);
void setCollisionPairs(SScriptCallBack *p, const char *cmd, setCollisionPairs_in *in, setCollisionPairs_out *out);
void setStartState(SScriptCallBack *p, const char *cmd, setStartState_in *in, setStartState_out *out);
void setGoalState(SScriptCallBack *p, const char *cmd, setGoalState_in *in, setGoalState_out *out);
void addGoalState(SScriptCallBack *p, const char *cmd, addGoalState_in *in, addGoalState_out *out);
void setGoal(SScriptCallBack *p, const char *cmd, setGoal_in *in, setGoal_out *out);
void setup(SScriptCallBack *p, const char *cmd, setup_in *in, setup_out *out);
void solve(SScriptCallBack *p, const char *cmd, solve_in *in, solve_out *out);
void simplifyPath(SScriptCallBack *p, const char *cmd, simplifyPath_in *in, simplifyPath_out *out);
void interpolatePath(SScriptCallBack *p, const char *cmd, interpolatePath_in *in, interpolatePath_out *out);
void getPath(SScriptCallBack *p, const char *cmd, getPath_in *in, getPath_out *out);
void compute(SScriptCallBack *p, const char *cmd, compute_in *in, compute_out *out);
void readState(SScriptCallBack *p, const char *cmd, readState_in *in, readState_out *out);
void writeState(SScriptCallBack *p, const char *cmd, writeState_in *in, writeState_out *out);
void isStateValid(SScriptCallBack *p, const char *cmd, isStateValid_in *in, isStateValid_out *out);
void setProjectionEvaluationCallback(SScriptCallBack *p, const char *cmd, setProjectionEvaluationCallback_in *in, setProjectionEvaluationCallback_out *out);
void setStateValidationCallback(SScriptCallBack *p, const char *cmd, setStateValidationCallback_in *in, setStateValidationCallback_out *out);
void setGoalCallback(SScriptCallBack *p, const char *cmd, setGoalCallback_in *in, setGoalCallback_out *out);
void setValidStateSamplerCallback(SScriptCallBack *p, const char *cmd, setValidStateSamplerCallback_in *in, setValidStateSamplerCallback_out *out);

#endif // STUBS_H__INCLUDED
