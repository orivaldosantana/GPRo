#include "stubs.h"

#include <iostream>

simInt simRegisterScriptCallbackFunctionE(const simChar *funcNameAtPluginName, const simChar *callTips, simVoid (*callBack)(struct SScriptCallBack *cb))
{
    simInt ret = simRegisterScriptCallbackFunction(funcNameAtPluginName, callTips, callBack);
    if(ret == 0)
    {
        std::cout << "Plugin 'CustomUI': warning: replaced function '" << funcNameAtPluginName << "'" << std::endl;
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
        std::cout << "Plugin 'CustomUI': warning: replaced variable '" << varName << "'" << std::endl;
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

void read__curve_options(int stack, curve_options *value)
{
    try
    {
        simInt info = simGetStackTableInfoE(stack, 0);
        if(info == sim_stack_table_empty)
            return;
        if(info != sim_stack_table_map)
            throw exception("expected a table");

        int oldsz = simGetStackSizeE(stack);
        simUnfoldStackTableE(stack);
        int numItems = (simGetStackSizeE(stack) - oldsz + 1) / 2;

        char *str;
        int strSz;

        while(numItems >= 1)
        {
            simMoveStackItemToTopE(stack, oldsz - 1); // move key to top
            if((str = simGetStackStringValueE(stack, &strSz)) != NULL && strSz >= 0)
            {
                simPopStackItemE(stack, 1);

                simMoveStackItemToTopE(stack, oldsz - 1); // move value to top

                if(0) {}
                else if(strcmp(str, "scatter_shape") == 0)
                {
                    try
                    {
                        // read field 'scatter_shape' of type int
                        read__int(stack, &(value->scatter_shape));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'scatter_shape': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "scatter_size") == 0)
                {
                    try
                    {
                        // read field 'scatter_size' of type int
                        read__int(stack, &(value->scatter_size));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'scatter_size': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "line_size") == 0)
                {
                    try
                    {
                        // read field 'line_size' of type int
                        read__int(stack, &(value->line_size));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'line_size': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "line_style") == 0)
                {
                    try
                    {
                        // read field 'line_style' of type int
                        read__int(stack, &(value->line_style));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'line_style': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "add_to_legend") == 0)
                {
                    try
                    {
                        // read field 'add_to_legend' of type bool
                        read__bool(stack, &(value->add_to_legend));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'add_to_legend': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "selectable") == 0)
                {
                    try
                    {
                        // read field 'selectable' of type bool
                        read__bool(stack, &(value->selectable));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'selectable': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else if(strcmp(str, "track") == 0)
                {
                    try
                    {
                        // read field 'track' of type bool
                        read__bool(stack, &(value->track));
                    }
                    catch(exception &ex)
                    {
                        std::string msg = "field 'track': ";
                        msg += ex.what();
                        throw exception(msg);
                    }
                }
                else
                {
                    std::string msg = "unexpected key: ";
                    msg += str;
                    throw exception(msg);
                }
            }
            numItems = (simGetStackSizeE(stack) - oldsz + 1) / 2;
        }
    }
    catch(exception &ex)
    {
        std::string msg = "read__curve_options: ";
        msg += ex.what();
        throw exception(msg);
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

void write__curve_options(int stack, curve_options *value)
{
    try
    {
        simPushTableOntoStackE(stack);

        try
        {
            // write field 'scatter_shape' of type int
            write__int(value->scatter_shape, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'scatter_shape': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'scatter_size' of type int
            write__int(value->scatter_size, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'scatter_size': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'line_size' of type int
            write__int(value->line_size, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'line_size': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'line_style' of type int
            write__int(value->line_style, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'line_style': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'add_to_legend' of type bool
            write__bool(value->add_to_legend, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'add_to_legend': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'selectable' of type bool
            write__bool(value->selectable, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'selectable': ";
            msg += ex.what();
            throw exception(msg);
        }

        try
        {
            // write field 'track' of type bool
            write__bool(value->track, stack);
        }
        catch(exception &ex)
        {
            std::string msg = "field 'track': ";
            msg += ex.what();
            throw exception(msg);
        }

    }
    catch(exception &ex)
    {
        std::string msg = "write__curve_options: ";
        msg += ex.what();
        throw exception(msg);
    }
}

curve_options::curve_options()
{
    scatter_shape = sim_customui_curve_scatter_shape_disc;
    scatter_size = 10;
    line_size = 2;
    line_style = sim_customui_line_style_solid;
    add_to_legend = true;
    selectable = true;
    track = false;
}


bool registerScriptStuff()
{
    try
    {
        simRegisterScriptVariableE("sim_customui_curve_type_time", (boost::lexical_cast<std::string>(sim_customui_curve_type_time)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_type_xy", (boost::lexical_cast<std::string>(sim_customui_curve_type_xy)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_scatter", (boost::lexical_cast<std::string>(sim_customui_curve_style_scatter)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_line", (boost::lexical_cast<std::string>(sim_customui_curve_style_line)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_line_and_scatter", (boost::lexical_cast<std::string>(sim_customui_curve_style_line_and_scatter)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_step_left", (boost::lexical_cast<std::string>(sim_customui_curve_style_step_left)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_step_center", (boost::lexical_cast<std::string>(sim_customui_curve_style_step_center)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_step_right", (boost::lexical_cast<std::string>(sim_customui_curve_style_step_right)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_style_impulse", (boost::lexical_cast<std::string>(sim_customui_curve_style_impulse)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_none", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_none)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_dot", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_dot)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_cross", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_cross)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_plus", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_plus)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_circle", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_circle)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_disc", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_disc)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_square", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_square)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_diamond", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_diamond)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_star", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_star)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_triangle", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_triangle)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_triangle_inverted", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_triangle_inverted)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_cross_square", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_cross_square)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_plus_square", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_plus_square)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_cross_circle", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_cross_circle)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_plus_circle", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_plus_circle)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_curve_scatter_shape_peace", (boost::lexical_cast<std::string>(sim_customui_curve_scatter_shape_peace)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_line_style_solid", (boost::lexical_cast<std::string>(sim_customui_line_style_solid)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_line_style_dotted", (boost::lexical_cast<std::string>(sim_customui_line_style_dotted)).c_str(), 0);
        simRegisterScriptVariableE("sim_customui_line_style_dashed", (boost::lexical_cast<std::string>(sim_customui_line_style_dashed)).c_str(), 0);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_create@CustomUI", "number uiHandle=simExtCustomUI_create(string xml)", create_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_destroy@CustomUI", "simExtCustomUI_destroy(number handle)", destroy_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getSliderValue@CustomUI", "number value=simExtCustomUI_getSliderValue(number handle,number id)", getSliderValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setSliderValue@CustomUI", "simExtCustomUI_setSliderValue(number handle,number id,number value,bool suppressEvents=true)", setSliderValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getEditValue@CustomUI", "string value=simExtCustomUI_getEditValue(number handle,number id)", getEditValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setEditValue@CustomUI", "simExtCustomUI_setEditValue(number handle,number id,string value,bool suppressEvents=true)", setEditValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getSpinboxValue@CustomUI", "number value=simExtCustomUI_getSpinboxValue(number handle,number id)", getSpinboxValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setSpinboxValue@CustomUI", "simExtCustomUI_setSpinboxValue(number handle,number id,number value,bool suppressEvents=true)", setSpinboxValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getCheckboxValue@CustomUI", "number value=simExtCustomUI_getCheckboxValue(number handle,number id)", getCheckboxValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setCheckboxValue@CustomUI", "simExtCustomUI_setCheckboxValue(number handle,number id,number value,bool suppressEvents=true)", setCheckboxValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getRadiobuttonValue@CustomUI", "number value=simExtCustomUI_getRadiobuttonValue(number handle,number id)", getRadiobuttonValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setRadiobuttonValue@CustomUI", "simExtCustomUI_setRadiobuttonValue(number handle,number id,number value,bool suppressEvents=true)", setRadiobuttonValue_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getLabelText@CustomUI", "string text=simExtCustomUI_getLabelText(number handle,number id)", getLabelText_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setLabelText@CustomUI", "simExtCustomUI_setLabelText(number handle,number id,string text,bool suppressEvents=true)", setLabelText_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_insertComboboxItem@CustomUI", "simExtCustomUI_insertComboboxItem(number handle,number id,number index,string text,bool suppressEvents=true)", insertComboboxItem_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_removeComboboxItem@CustomUI", "simExtCustomUI_removeComboboxItem(number handle,number id,number index,bool suppressEvents=true)", removeComboboxItem_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getComboboxItemCount@CustomUI", "number count=simExtCustomUI_getComboboxItemCount(number handle,number id)", getComboboxItemCount_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getComboboxItemText@CustomUI", "string text=simExtCustomUI_getComboboxItemText(number handle,number id,number index)", getComboboxItemText_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getComboboxItems@CustomUI", "table items=simExtCustomUI_getComboboxItems(number handle,number id)", getComboboxItems_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setComboboxItems@CustomUI", "simExtCustomUI_setComboboxItems(number handle,number id,table items,number index,bool suppressEvents=true)", setComboboxItems_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setComboboxSelectedIndex@CustomUI", "simExtCustomUI_setComboboxSelectedIndex(number handle,number id,number index,bool suppressEvents=true)", setComboboxSelectedIndex_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_hide@CustomUI", "simExtCustomUI_hide(number handle)", hide_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_show@CustomUI", "simExtCustomUI_show(number handle)", show_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_isVisible@CustomUI", "bool visibility=simExtCustomUI_isVisible(number handle)", isVisible_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getPosition@CustomUI", "number x,number y=simExtCustomUI_getPosition(number handle)", getPosition_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPosition@CustomUI", "simExtCustomUI_setPosition(number handle,number x,number y,bool suppressEvents=true)", setPosition_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getSize@CustomUI", "number w,number h=simExtCustomUI_getSize(number handle)", getSize_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setSize@CustomUI", "simExtCustomUI_setSize(number handle,number w,number h,bool suppressEvents=true)", setSize_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getTitle@CustomUI", "string title=simExtCustomUI_getTitle(number handle)", getTitle_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setTitle@CustomUI", "simExtCustomUI_setTitle(number handle,string title,bool suppressEvents=true)", setTitle_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setImageData@CustomUI", "simExtCustomUI_setImageData(number handle,number id,string data,number width,number height)", setImageData_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setEnabled@CustomUI", "simExtCustomUI_setEnabled(number handle,number id,bool enabled,bool suppressEvents=true)", setEnabled_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getCurrentTab@CustomUI", "number index=simExtCustomUI_getCurrentTab(number handle,number id)", getCurrentTab_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setCurrentTab@CustomUI", "simExtCustomUI_setCurrentTab(number handle,number id,number index,bool suppressEvents=true)", setCurrentTab_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getWidgetVisibility@CustomUI", "bool visibility=simExtCustomUI_getWidgetVisibility(number handle,number id)", getWidgetVisibility_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setWidgetVisibility@CustomUI", "simExtCustomUI_setWidgetVisibility(number handle,number id,bool visibility)", setWidgetVisibility_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getCurrentEditWidget@CustomUI", "number id=simExtCustomUI_getCurrentEditWidget(number handle)", getCurrentEditWidget_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setCurrentEditWidget@CustomUI", "simExtCustomUI_setCurrentEditWidget(number handle,number id)", setCurrentEditWidget_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_replot@CustomUI", "simExtCustomUI_replot(number handle,number id)", replot_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_addCurve@CustomUI", "simExtCustomUI_addCurve(number handle,number id,number type,string name,table color,number style,curve_options options)", addCurve_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_addCurveTimePoints@CustomUI", "simExtCustomUI_addCurveTimePoints(number handle,number id,string name,table x,table y)", addCurveTimePoints_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_addCurveXYPoints@CustomUI", "simExtCustomUI_addCurveXYPoints(number handle,number id,string name,table t,table x,table y)", addCurveXYPoints_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_clearCurve@CustomUI", "simExtCustomUI_clearCurve(number handle,number id,string name)", clearCurve_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_removeCurve@CustomUI", "simExtCustomUI_removeCurve(number handle,number id,string name)", removeCurve_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotRanges@CustomUI", "simExtCustomUI_setPlotRanges(number handle,number id,number xmin,number xmax,number ymin,number ymax)", setPlotRanges_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotXRange@CustomUI", "simExtCustomUI_setPlotXRange(number handle,number id,number xmin,number xmax)", setPlotXRange_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotYRange@CustomUI", "simExtCustomUI_setPlotYRange(number handle,number id,number ymin,number ymax)", setPlotYRange_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_growPlotRanges@CustomUI", "simExtCustomUI_growPlotRanges(number handle,number id,number xmin,number xmax,number ymin,number ymax)", growPlotRanges_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_growPlotXRange@CustomUI", "simExtCustomUI_growPlotXRange(number handle,number id,number xmin,number xmax)", growPlotXRange_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_growPlotYRange@CustomUI", "simExtCustomUI_growPlotYRange(number handle,number id,number ymin,number ymax)", growPlotYRange_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotLabels@CustomUI", "simExtCustomUI_setPlotLabels(number handle,number id,string x,string y)", setPlotLabels_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotXLabel@CustomUI", "simExtCustomUI_setPlotXLabel(number handle,number id,string label)", setPlotXLabel_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setPlotYLabel@CustomUI", "simExtCustomUI_setPlotYLabel(number handle,number id,string label)", setPlotYLabel_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_rescaleAxes@CustomUI", "simExtCustomUI_rescaleAxes(number handle,number id,string name,bool onlyEnlargeX=false,bool onlyEnlargeY=false)", rescaleAxes_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_rescaleAxesAll@CustomUI", "simExtCustomUI_rescaleAxesAll(number handle,number id,bool onlyEnlargeX=false,bool onlyEnlargeY=false)", rescaleAxesAll_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setMouseOptions@CustomUI", "simExtCustomUI_setMouseOptions(number handle,number id,bool panX,bool panY,bool zoomX,bool zoomY)", setMouseOptions_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_setLegendVisibility@CustomUI", "simExtCustomUI_setLegendVisibility(number handle,number id,bool visible)", setLegendVisibility_callback);
        simRegisterScriptCallbackFunctionE("simExtCustomUI_getCurveData@CustomUI", "table t,table x,table y=simExtCustomUI_getCurveData(number handle,number id,string name)", getCurveData_callback);
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

const char* curve_type_string(curve_type x)
{
    switch(x)
    {
        case sim_customui_curve_type_time: return "sim_customui_curve_type_time";
        case sim_customui_curve_type_xy: return "sim_customui_curve_type_xy";
        default: return "???";
    }
}

const char* curve_style_string(curve_style x)
{
    switch(x)
    {
        case sim_customui_curve_style_scatter: return "sim_customui_curve_style_scatter";
        case sim_customui_curve_style_line: return "sim_customui_curve_style_line";
        case sim_customui_curve_style_line_and_scatter: return "sim_customui_curve_style_line_and_scatter";
        case sim_customui_curve_style_step_left: return "sim_customui_curve_style_step_left";
        case sim_customui_curve_style_step_center: return "sim_customui_curve_style_step_center";
        case sim_customui_curve_style_step_right: return "sim_customui_curve_style_step_right";
        case sim_customui_curve_style_impulse: return "sim_customui_curve_style_impulse";
        default: return "???";
    }
}

const char* curve_scatter_shape_string(curve_scatter_shape x)
{
    switch(x)
    {
        case sim_customui_curve_scatter_shape_none: return "sim_customui_curve_scatter_shape_none";
        case sim_customui_curve_scatter_shape_dot: return "sim_customui_curve_scatter_shape_dot";
        case sim_customui_curve_scatter_shape_cross: return "sim_customui_curve_scatter_shape_cross";
        case sim_customui_curve_scatter_shape_plus: return "sim_customui_curve_scatter_shape_plus";
        case sim_customui_curve_scatter_shape_circle: return "sim_customui_curve_scatter_shape_circle";
        case sim_customui_curve_scatter_shape_disc: return "sim_customui_curve_scatter_shape_disc";
        case sim_customui_curve_scatter_shape_square: return "sim_customui_curve_scatter_shape_square";
        case sim_customui_curve_scatter_shape_diamond: return "sim_customui_curve_scatter_shape_diamond";
        case sim_customui_curve_scatter_shape_star: return "sim_customui_curve_scatter_shape_star";
        case sim_customui_curve_scatter_shape_triangle: return "sim_customui_curve_scatter_shape_triangle";
        case sim_customui_curve_scatter_shape_triangle_inverted: return "sim_customui_curve_scatter_shape_triangle_inverted";
        case sim_customui_curve_scatter_shape_cross_square: return "sim_customui_curve_scatter_shape_cross_square";
        case sim_customui_curve_scatter_shape_plus_square: return "sim_customui_curve_scatter_shape_plus_square";
        case sim_customui_curve_scatter_shape_cross_circle: return "sim_customui_curve_scatter_shape_cross_circle";
        case sim_customui_curve_scatter_shape_plus_circle: return "sim_customui_curve_scatter_shape_plus_circle";
        case sim_customui_curve_scatter_shape_peace: return "sim_customui_curve_scatter_shape_peace";
        default: return "???";
    }
}

const char* line_style_string(line_style x)
{
    switch(x)
    {
        case sim_customui_line_style_solid: return "sim_customui_line_style_solid";
        case sim_customui_line_style_dotted: return "sim_customui_line_style_dotted";
        case sim_customui_line_style_dashed: return "sim_customui_line_style_dashed";
        default: return "???";
    }
}

create_in::create_in()
{
}

create_out::create_out()
{
    uiHandle = -1;
}

void create(SScriptCallBack *p, create_in *in_args, create_out *out_args)
{
    create(p, "simExtCustomUI_create", in_args, out_args);
}

int create(SScriptCallBack *p, std::string xml)
{
    create_in in_args;
    in_args.xml = xml;
    create_out out_args;
    create(p, &in_args, &out_args);
    return out_args.uiHandle;
}

void create(SScriptCallBack *p, create_out *out_args, std::string xml)
{
    create_in in_args;
    in_args.xml = xml;
    create(p, &in_args, out_args);
}

void create_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_create";

    create_in in_args;
    create_out out_args;

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
                // read input argument 1 (xml) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.xml));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (xml): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (xml): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (xml): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        create(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (uiHandle) of type int
            write__int(out_args.uiHandle, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (uiHandle): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (uiHandle)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (uiHandle)): ";
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

destroy_in::destroy_in()
{
}

destroy_out::destroy_out()
{
}

void destroy(SScriptCallBack *p, destroy_in *in_args, destroy_out *out_args)
{
    destroy(p, "simExtCustomUI_destroy", in_args, out_args);
}

void destroy(SScriptCallBack *p, int handle)
{
    destroy_in in_args;
    in_args.handle = handle;
    destroy_out out_args;
    destroy(p, &in_args, &out_args);
}

void destroy(SScriptCallBack *p, destroy_out *out_args, int handle)
{
    destroy_in in_args;
    in_args.handle = handle;
    destroy(p, &in_args, out_args);
}

void destroy_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_destroy";

    destroy_in in_args;
    destroy_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        destroy(p, cmd, &in_args, &out_args);
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

getSliderValue_in::getSliderValue_in()
{
}

getSliderValue_out::getSliderValue_out()
{
}

void getSliderValue(SScriptCallBack *p, getSliderValue_in *in_args, getSliderValue_out *out_args)
{
    getSliderValue(p, "simExtCustomUI_getSliderValue", in_args, out_args);
}

int getSliderValue(SScriptCallBack *p, int handle, int id)
{
    getSliderValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getSliderValue_out out_args;
    getSliderValue(p, &in_args, &out_args);
    return out_args.value;
}

void getSliderValue(SScriptCallBack *p, getSliderValue_out *out_args, int handle, int id)
{
    getSliderValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getSliderValue(p, &in_args, out_args);
}

void getSliderValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getSliderValue";

    getSliderValue_in in_args;
    getSliderValue_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getSliderValue(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (value) of type int
            write__int(out_args.value, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (value): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (value)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (value)): ";
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

setSliderValue_in::setSliderValue_in()
{
    suppressEvents = true;
}

setSliderValue_out::setSliderValue_out()
{
}

void setSliderValue(SScriptCallBack *p, setSliderValue_in *in_args, setSliderValue_out *out_args)
{
    setSliderValue(p, "simExtCustomUI_setSliderValue", in_args, out_args);
}

void setSliderValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents)
{
    setSliderValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setSliderValue_out out_args;
    setSliderValue(p, &in_args, &out_args);
}

void setSliderValue(SScriptCallBack *p, setSliderValue_out *out_args, int handle, int id, int value, bool suppressEvents)
{
    setSliderValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setSliderValue(p, &in_args, out_args);
}

void setSliderValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setSliderValue";

    setSliderValue_in in_args;
    setSliderValue_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (value) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.value));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (value): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setSliderValue(p, cmd, &in_args, &out_args);
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

getEditValue_in::getEditValue_in()
{
}

getEditValue_out::getEditValue_out()
{
}

void getEditValue(SScriptCallBack *p, getEditValue_in *in_args, getEditValue_out *out_args)
{
    getEditValue(p, "simExtCustomUI_getEditValue", in_args, out_args);
}

std::string getEditValue(SScriptCallBack *p, int handle, int id)
{
    getEditValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getEditValue_out out_args;
    getEditValue(p, &in_args, &out_args);
    return out_args.value;
}

void getEditValue(SScriptCallBack *p, getEditValue_out *out_args, int handle, int id)
{
    getEditValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getEditValue(p, &in_args, out_args);
}

void getEditValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getEditValue";

    getEditValue_in in_args;
    getEditValue_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getEditValue(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (value) of type std::string
            write__std__string(out_args.value, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (value): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (value)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (value)): ";
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

setEditValue_in::setEditValue_in()
{
    suppressEvents = true;
}

setEditValue_out::setEditValue_out()
{
}

void setEditValue(SScriptCallBack *p, setEditValue_in *in_args, setEditValue_out *out_args)
{
    setEditValue(p, "simExtCustomUI_setEditValue", in_args, out_args);
}

void setEditValue(SScriptCallBack *p, int handle, int id, std::string value, bool suppressEvents)
{
    setEditValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setEditValue_out out_args;
    setEditValue(p, &in_args, &out_args);
}

void setEditValue(SScriptCallBack *p, setEditValue_out *out_args, int handle, int id, std::string value, bool suppressEvents)
{
    setEditValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setEditValue(p, &in_args, out_args);
}

void setEditValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setEditValue";

    setEditValue_in in_args;
    setEditValue_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (value) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.value));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (value): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setEditValue(p, cmd, &in_args, &out_args);
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

getSpinboxValue_in::getSpinboxValue_in()
{
}

getSpinboxValue_out::getSpinboxValue_out()
{
}

void getSpinboxValue(SScriptCallBack *p, getSpinboxValue_in *in_args, getSpinboxValue_out *out_args)
{
    getSpinboxValue(p, "simExtCustomUI_getSpinboxValue", in_args, out_args);
}

float getSpinboxValue(SScriptCallBack *p, int handle, int id)
{
    getSpinboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getSpinboxValue_out out_args;
    getSpinboxValue(p, &in_args, &out_args);
    return out_args.value;
}

void getSpinboxValue(SScriptCallBack *p, getSpinboxValue_out *out_args, int handle, int id)
{
    getSpinboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getSpinboxValue(p, &in_args, out_args);
}

void getSpinboxValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getSpinboxValue";

    getSpinboxValue_in in_args;
    getSpinboxValue_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getSpinboxValue(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (value) of type float
            write__float(out_args.value, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (value): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (value)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (value)): ";
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

setSpinboxValue_in::setSpinboxValue_in()
{
    suppressEvents = true;
}

setSpinboxValue_out::setSpinboxValue_out()
{
}

void setSpinboxValue(SScriptCallBack *p, setSpinboxValue_in *in_args, setSpinboxValue_out *out_args)
{
    setSpinboxValue(p, "simExtCustomUI_setSpinboxValue", in_args, out_args);
}

void setSpinboxValue(SScriptCallBack *p, int handle, int id, float value, bool suppressEvents)
{
    setSpinboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setSpinboxValue_out out_args;
    setSpinboxValue(p, &in_args, &out_args);
}

void setSpinboxValue(SScriptCallBack *p, setSpinboxValue_out *out_args, int handle, int id, float value, bool suppressEvents)
{
    setSpinboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setSpinboxValue(p, &in_args, out_args);
}

void setSpinboxValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setSpinboxValue";

    setSpinboxValue_in in_args;
    setSpinboxValue_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (value) of type float
                simMoveStackItemToTopE(p->stackID, 0);
                read__float(p->stackID, &(in_args.value));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (value): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setSpinboxValue(p, cmd, &in_args, &out_args);
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

getCheckboxValue_in::getCheckboxValue_in()
{
}

getCheckboxValue_out::getCheckboxValue_out()
{
}

void getCheckboxValue(SScriptCallBack *p, getCheckboxValue_in *in_args, getCheckboxValue_out *out_args)
{
    getCheckboxValue(p, "simExtCustomUI_getCheckboxValue", in_args, out_args);
}

int getCheckboxValue(SScriptCallBack *p, int handle, int id)
{
    getCheckboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getCheckboxValue_out out_args;
    getCheckboxValue(p, &in_args, &out_args);
    return out_args.value;
}

void getCheckboxValue(SScriptCallBack *p, getCheckboxValue_out *out_args, int handle, int id)
{
    getCheckboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getCheckboxValue(p, &in_args, out_args);
}

void getCheckboxValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getCheckboxValue";

    getCheckboxValue_in in_args;
    getCheckboxValue_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getCheckboxValue(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (value) of type int
            write__int(out_args.value, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (value): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (value)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (value)): ";
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

setCheckboxValue_in::setCheckboxValue_in()
{
    suppressEvents = true;
}

setCheckboxValue_out::setCheckboxValue_out()
{
}

void setCheckboxValue(SScriptCallBack *p, setCheckboxValue_in *in_args, setCheckboxValue_out *out_args)
{
    setCheckboxValue(p, "simExtCustomUI_setCheckboxValue", in_args, out_args);
}

void setCheckboxValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents)
{
    setCheckboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setCheckboxValue_out out_args;
    setCheckboxValue(p, &in_args, &out_args);
}

void setCheckboxValue(SScriptCallBack *p, setCheckboxValue_out *out_args, int handle, int id, int value, bool suppressEvents)
{
    setCheckboxValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setCheckboxValue(p, &in_args, out_args);
}

void setCheckboxValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setCheckboxValue";

    setCheckboxValue_in in_args;
    setCheckboxValue_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (value) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.value));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (value): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setCheckboxValue(p, cmd, &in_args, &out_args);
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

getRadiobuttonValue_in::getRadiobuttonValue_in()
{
}

getRadiobuttonValue_out::getRadiobuttonValue_out()
{
}

void getRadiobuttonValue(SScriptCallBack *p, getRadiobuttonValue_in *in_args, getRadiobuttonValue_out *out_args)
{
    getRadiobuttonValue(p, "simExtCustomUI_getRadiobuttonValue", in_args, out_args);
}

int getRadiobuttonValue(SScriptCallBack *p, int handle, int id)
{
    getRadiobuttonValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getRadiobuttonValue_out out_args;
    getRadiobuttonValue(p, &in_args, &out_args);
    return out_args.value;
}

void getRadiobuttonValue(SScriptCallBack *p, getRadiobuttonValue_out *out_args, int handle, int id)
{
    getRadiobuttonValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getRadiobuttonValue(p, &in_args, out_args);
}

void getRadiobuttonValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getRadiobuttonValue";

    getRadiobuttonValue_in in_args;
    getRadiobuttonValue_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getRadiobuttonValue(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (value) of type int
            write__int(out_args.value, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (value): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (value)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (value)): ";
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

setRadiobuttonValue_in::setRadiobuttonValue_in()
{
    suppressEvents = true;
}

setRadiobuttonValue_out::setRadiobuttonValue_out()
{
}

void setRadiobuttonValue(SScriptCallBack *p, setRadiobuttonValue_in *in_args, setRadiobuttonValue_out *out_args)
{
    setRadiobuttonValue(p, "simExtCustomUI_setRadiobuttonValue", in_args, out_args);
}

void setRadiobuttonValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents)
{
    setRadiobuttonValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setRadiobuttonValue_out out_args;
    setRadiobuttonValue(p, &in_args, &out_args);
}

void setRadiobuttonValue(SScriptCallBack *p, setRadiobuttonValue_out *out_args, int handle, int id, int value, bool suppressEvents)
{
    setRadiobuttonValue_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    in_args.suppressEvents = suppressEvents;
    setRadiobuttonValue(p, &in_args, out_args);
}

void setRadiobuttonValue_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setRadiobuttonValue";

    setRadiobuttonValue_in in_args;
    setRadiobuttonValue_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (value) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.value));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (value): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (value): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setRadiobuttonValue(p, cmd, &in_args, &out_args);
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

getLabelText_in::getLabelText_in()
{
}

getLabelText_out::getLabelText_out()
{
}

void getLabelText(SScriptCallBack *p, getLabelText_in *in_args, getLabelText_out *out_args)
{
    getLabelText(p, "simExtCustomUI_getLabelText", in_args, out_args);
}

std::string getLabelText(SScriptCallBack *p, int handle, int id)
{
    getLabelText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getLabelText_out out_args;
    getLabelText(p, &in_args, &out_args);
    return out_args.text;
}

void getLabelText(SScriptCallBack *p, getLabelText_out *out_args, int handle, int id)
{
    getLabelText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getLabelText(p, &in_args, out_args);
}

void getLabelText_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getLabelText";

    getLabelText_in in_args;
    getLabelText_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getLabelText(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (text) of type std::string
            write__std__string(out_args.text, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (text): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (text)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (text)): ";
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

setLabelText_in::setLabelText_in()
{
    suppressEvents = true;
}

setLabelText_out::setLabelText_out()
{
}

void setLabelText(SScriptCallBack *p, setLabelText_in *in_args, setLabelText_out *out_args)
{
    setLabelText(p, "simExtCustomUI_setLabelText", in_args, out_args);
}

void setLabelText(SScriptCallBack *p, int handle, int id, std::string text, bool suppressEvents)
{
    setLabelText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.text = text;
    in_args.suppressEvents = suppressEvents;
    setLabelText_out out_args;
    setLabelText(p, &in_args, &out_args);
}

void setLabelText(SScriptCallBack *p, setLabelText_out *out_args, int handle, int id, std::string text, bool suppressEvents)
{
    setLabelText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.text = text;
    in_args.suppressEvents = suppressEvents;
    setLabelText(p, &in_args, out_args);
}

void setLabelText_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setLabelText";

    setLabelText_in in_args;
    setLabelText_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (text) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.text));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (text): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (text): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (text): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setLabelText(p, cmd, &in_args, &out_args);
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

insertComboboxItem_in::insertComboboxItem_in()
{
    suppressEvents = true;
}

insertComboboxItem_out::insertComboboxItem_out()
{
}

void insertComboboxItem(SScriptCallBack *p, insertComboboxItem_in *in_args, insertComboboxItem_out *out_args)
{
    insertComboboxItem(p, "simExtCustomUI_insertComboboxItem", in_args, out_args);
}

void insertComboboxItem(SScriptCallBack *p, int handle, int id, int index, std::string text, bool suppressEvents)
{
    insertComboboxItem_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.text = text;
    in_args.suppressEvents = suppressEvents;
    insertComboboxItem_out out_args;
    insertComboboxItem(p, &in_args, &out_args);
}

void insertComboboxItem(SScriptCallBack *p, insertComboboxItem_out *out_args, int handle, int id, int index, std::string text, bool suppressEvents)
{
    insertComboboxItem_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.text = text;
    in_args.suppressEvents = suppressEvents;
    insertComboboxItem(p, &in_args, out_args);
}

void insertComboboxItem_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_insertComboboxItem";

    insertComboboxItem_in in_args;
    insertComboboxItem_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 5)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (text) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.text));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (text): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (text): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (text): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        insertComboboxItem(p, cmd, &in_args, &out_args);
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

removeComboboxItem_in::removeComboboxItem_in()
{
    suppressEvents = true;
}

removeComboboxItem_out::removeComboboxItem_out()
{
}

void removeComboboxItem(SScriptCallBack *p, removeComboboxItem_in *in_args, removeComboboxItem_out *out_args)
{
    removeComboboxItem(p, "simExtCustomUI_removeComboboxItem", in_args, out_args);
}

void removeComboboxItem(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents)
{
    removeComboboxItem_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    removeComboboxItem_out out_args;
    removeComboboxItem(p, &in_args, &out_args);
}

void removeComboboxItem(SScriptCallBack *p, removeComboboxItem_out *out_args, int handle, int id, int index, bool suppressEvents)
{
    removeComboboxItem_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    removeComboboxItem(p, &in_args, out_args);
}

void removeComboboxItem_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_removeComboboxItem";

    removeComboboxItem_in in_args;
    removeComboboxItem_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        removeComboboxItem(p, cmd, &in_args, &out_args);
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

getComboboxItemCount_in::getComboboxItemCount_in()
{
}

getComboboxItemCount_out::getComboboxItemCount_out()
{
}

void getComboboxItemCount(SScriptCallBack *p, getComboboxItemCount_in *in_args, getComboboxItemCount_out *out_args)
{
    getComboboxItemCount(p, "simExtCustomUI_getComboboxItemCount", in_args, out_args);
}

int getComboboxItemCount(SScriptCallBack *p, int handle, int id)
{
    getComboboxItemCount_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getComboboxItemCount_out out_args;
    getComboboxItemCount(p, &in_args, &out_args);
    return out_args.count;
}

void getComboboxItemCount(SScriptCallBack *p, getComboboxItemCount_out *out_args, int handle, int id)
{
    getComboboxItemCount_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getComboboxItemCount(p, &in_args, out_args);
}

void getComboboxItemCount_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getComboboxItemCount";

    getComboboxItemCount_in in_args;
    getComboboxItemCount_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getComboboxItemCount(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (count) of type int
            write__int(out_args.count, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (count): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (count)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (count)): ";
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

getComboboxItemText_in::getComboboxItemText_in()
{
}

getComboboxItemText_out::getComboboxItemText_out()
{
}

void getComboboxItemText(SScriptCallBack *p, getComboboxItemText_in *in_args, getComboboxItemText_out *out_args)
{
    getComboboxItemText(p, "simExtCustomUI_getComboboxItemText", in_args, out_args);
}

std::string getComboboxItemText(SScriptCallBack *p, int handle, int id, int index)
{
    getComboboxItemText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    getComboboxItemText_out out_args;
    getComboboxItemText(p, &in_args, &out_args);
    return out_args.text;
}

void getComboboxItemText(SScriptCallBack *p, getComboboxItemText_out *out_args, int handle, int id, int index)
{
    getComboboxItemText_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    getComboboxItemText(p, &in_args, out_args);
}

void getComboboxItemText_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getComboboxItemText";

    getComboboxItemText_in in_args;
    getComboboxItemText_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getComboboxItemText(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (text) of type std::string
            write__std__string(out_args.text, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (text): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (text)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (text)): ";
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

getComboboxItems_in::getComboboxItems_in()
{
}

getComboboxItems_out::getComboboxItems_out()
{
}

void getComboboxItems(SScriptCallBack *p, getComboboxItems_in *in_args, getComboboxItems_out *out_args)
{
    getComboboxItems(p, "simExtCustomUI_getComboboxItems", in_args, out_args);
}

std::vector<std::string> getComboboxItems(SScriptCallBack *p, int handle, int id)
{
    getComboboxItems_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getComboboxItems_out out_args;
    getComboboxItems(p, &in_args, &out_args);
    return out_args.items;
}

void getComboboxItems(SScriptCallBack *p, getComboboxItems_out *out_args, int handle, int id)
{
    getComboboxItems_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getComboboxItems(p, &in_args, out_args);
}

void getComboboxItems_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getComboboxItems";

    getComboboxItems_in in_args;
    getComboboxItems_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getComboboxItems(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (items) of type array of std::vector<std::string>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.items.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__std__string(out_args.items[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (items): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (items)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (items)): ";
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

setComboboxItems_in::setComboboxItems_in()
{
    suppressEvents = true;
}

setComboboxItems_out::setComboboxItems_out()
{
}

void setComboboxItems(SScriptCallBack *p, setComboboxItems_in *in_args, setComboboxItems_out *out_args)
{
    setComboboxItems(p, "simExtCustomUI_setComboboxItems", in_args, out_args);
}

void setComboboxItems(SScriptCallBack *p, int handle, int id, std::vector<std::string> items, int index, bool suppressEvents)
{
    setComboboxItems_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.items = items;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setComboboxItems_out out_args;
    setComboboxItems(p, &in_args, &out_args);
}

void setComboboxItems(SScriptCallBack *p, setComboboxItems_out *out_args, int handle, int id, std::vector<std::string> items, int index, bool suppressEvents)
{
    setComboboxItems_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.items = items;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setComboboxItems(p, &in_args, out_args);
}

void setComboboxItems_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setComboboxItems";

    setComboboxItems_in in_args;
    setComboboxItems_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 5)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (items) of type array of std::vector<std::string>
                simMoveStackItemToTopE(p->stackID, 0);
                int i = simGetStackTableInfoE(p->stackID, 0);
                if(i < 0)
                    throw exception("error reading input argument 3 (items): expected array");
                int oldsz = simGetStackSizeE(p->stackID);
                simUnfoldStackTableE(p->stackID);
                int sz = (simGetStackSizeE(p->stackID) - oldsz + 1) / 2;
                for(int i = 0; i < sz; i++)
                {
                    simMoveStackItemToTopE(p->stackID, oldsz - 1);
                    int j;
                    read__int(p->stackID, &j);
                    simMoveStackItemToTop(p->stackID, oldsz - 1);
                    std::string v;
                    read__std__string(p->stackID, &v);
                    in_args.items.push_back(v);
                }
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (items): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (items): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (items): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setComboboxItems(p, cmd, &in_args, &out_args);
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

setComboboxSelectedIndex_in::setComboboxSelectedIndex_in()
{
    suppressEvents = true;
}

setComboboxSelectedIndex_out::setComboboxSelectedIndex_out()
{
}

void setComboboxSelectedIndex(SScriptCallBack *p, setComboboxSelectedIndex_in *in_args, setComboboxSelectedIndex_out *out_args)
{
    setComboboxSelectedIndex(p, "simExtCustomUI_setComboboxSelectedIndex", in_args, out_args);
}

void setComboboxSelectedIndex(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents)
{
    setComboboxSelectedIndex_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setComboboxSelectedIndex_out out_args;
    setComboboxSelectedIndex(p, &in_args, &out_args);
}

void setComboboxSelectedIndex(SScriptCallBack *p, setComboboxSelectedIndex_out *out_args, int handle, int id, int index, bool suppressEvents)
{
    setComboboxSelectedIndex_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setComboboxSelectedIndex(p, &in_args, out_args);
}

void setComboboxSelectedIndex_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setComboboxSelectedIndex";

    setComboboxSelectedIndex_in in_args;
    setComboboxSelectedIndex_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setComboboxSelectedIndex(p, cmd, &in_args, &out_args);
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

hide_in::hide_in()
{
}

hide_out::hide_out()
{
}

void hide(SScriptCallBack *p, hide_in *in_args, hide_out *out_args)
{
    hide(p, "simExtCustomUI_hide", in_args, out_args);
}

void hide(SScriptCallBack *p, int handle)
{
    hide_in in_args;
    in_args.handle = handle;
    hide_out out_args;
    hide(p, &in_args, &out_args);
}

void hide(SScriptCallBack *p, hide_out *out_args, int handle)
{
    hide_in in_args;
    in_args.handle = handle;
    hide(p, &in_args, out_args);
}

void hide_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_hide";

    hide_in in_args;
    hide_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        hide(p, cmd, &in_args, &out_args);
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

show_in::show_in()
{
}

show_out::show_out()
{
}

void show(SScriptCallBack *p, show_in *in_args, show_out *out_args)
{
    show(p, "simExtCustomUI_show", in_args, out_args);
}

void show(SScriptCallBack *p, int handle)
{
    show_in in_args;
    in_args.handle = handle;
    show_out out_args;
    show(p, &in_args, &out_args);
}

void show(SScriptCallBack *p, show_out *out_args, int handle)
{
    show_in in_args;
    in_args.handle = handle;
    show(p, &in_args, out_args);
}

void show_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_show";

    show_in in_args;
    show_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        show(p, cmd, &in_args, &out_args);
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

isVisible_in::isVisible_in()
{
}

isVisible_out::isVisible_out()
{
}

void isVisible(SScriptCallBack *p, isVisible_in *in_args, isVisible_out *out_args)
{
    isVisible(p, "simExtCustomUI_isVisible", in_args, out_args);
}

bool isVisible(SScriptCallBack *p, int handle)
{
    isVisible_in in_args;
    in_args.handle = handle;
    isVisible_out out_args;
    isVisible(p, &in_args, &out_args);
    return out_args.visibility;
}

void isVisible(SScriptCallBack *p, isVisible_out *out_args, int handle)
{
    isVisible_in in_args;
    in_args.handle = handle;
    isVisible(p, &in_args, out_args);
}

void isVisible_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_isVisible";

    isVisible_in in_args;
    isVisible_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        isVisible(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (visibility) of type bool
            write__bool(out_args.visibility, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (visibility): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (visibility)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (visibility)): ";
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

getPosition_in::getPosition_in()
{
}

getPosition_out::getPosition_out()
{
}

void getPosition(SScriptCallBack *p, getPosition_in *in_args, getPosition_out *out_args)
{
    getPosition(p, "simExtCustomUI_getPosition", in_args, out_args);
}

void getPosition(SScriptCallBack *p, getPosition_out *out_args, int handle)
{
    getPosition_in in_args;
    in_args.handle = handle;
    getPosition(p, &in_args, out_args);
}

void getPosition_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getPosition";

    getPosition_in in_args;
    getPosition_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getPosition(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (x) of type int
            write__int(out_args.x, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (x): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (x)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (x)): ";
            msg += ss.str();
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (y) of type int
            write__int(out_args.y, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 2 (y): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 2 (y)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 2 (y)): ";
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

setPosition_in::setPosition_in()
{
    suppressEvents = true;
}

setPosition_out::setPosition_out()
{
}

void setPosition(SScriptCallBack *p, setPosition_in *in_args, setPosition_out *out_args)
{
    setPosition(p, "simExtCustomUI_setPosition", in_args, out_args);
}

void setPosition(SScriptCallBack *p, int handle, int x, int y, bool suppressEvents)
{
    setPosition_in in_args;
    in_args.handle = handle;
    in_args.x = x;
    in_args.y = y;
    in_args.suppressEvents = suppressEvents;
    setPosition_out out_args;
    setPosition(p, &in_args, &out_args);
}

void setPosition(SScriptCallBack *p, setPosition_out *out_args, int handle, int x, int y, bool suppressEvents)
{
    setPosition_in in_args;
    in_args.handle = handle;
    in_args.x = x;
    in_args.y = y;
    in_args.suppressEvents = suppressEvents;
    setPosition(p, &in_args, out_args);
}

void setPosition_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPosition";

    setPosition_in in_args;
    setPosition_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (x) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.x));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (x): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (x): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (x): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (y) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.y));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (y): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (y): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (y): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPosition(p, cmd, &in_args, &out_args);
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

getSize_in::getSize_in()
{
}

getSize_out::getSize_out()
{
}

void getSize(SScriptCallBack *p, getSize_in *in_args, getSize_out *out_args)
{
    getSize(p, "simExtCustomUI_getSize", in_args, out_args);
}

void getSize(SScriptCallBack *p, getSize_out *out_args, int handle)
{
    getSize_in in_args;
    in_args.handle = handle;
    getSize(p, &in_args, out_args);
}

void getSize_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getSize";

    getSize_in in_args;
    getSize_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getSize(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (w) of type int
            write__int(out_args.w, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (w): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (w)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (w)): ";
            msg += ss.str();
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (h) of type int
            write__int(out_args.h, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 2 (h): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 2 (h)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 2 (h)): ";
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

setSize_in::setSize_in()
{
    suppressEvents = true;
}

setSize_out::setSize_out()
{
}

void setSize(SScriptCallBack *p, setSize_in *in_args, setSize_out *out_args)
{
    setSize(p, "simExtCustomUI_setSize", in_args, out_args);
}

void setSize(SScriptCallBack *p, int handle, int w, int h, bool suppressEvents)
{
    setSize_in in_args;
    in_args.handle = handle;
    in_args.w = w;
    in_args.h = h;
    in_args.suppressEvents = suppressEvents;
    setSize_out out_args;
    setSize(p, &in_args, &out_args);
}

void setSize(SScriptCallBack *p, setSize_out *out_args, int handle, int w, int h, bool suppressEvents)
{
    setSize_in in_args;
    in_args.handle = handle;
    in_args.w = w;
    in_args.h = h;
    in_args.suppressEvents = suppressEvents;
    setSize(p, &in_args, out_args);
}

void setSize_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setSize";

    setSize_in in_args;
    setSize_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (w) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.w));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (w): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (w): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (w): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (h) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.h));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (h): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (h): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (h): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setSize(p, cmd, &in_args, &out_args);
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

getTitle_in::getTitle_in()
{
}

getTitle_out::getTitle_out()
{
}

void getTitle(SScriptCallBack *p, getTitle_in *in_args, getTitle_out *out_args)
{
    getTitle(p, "simExtCustomUI_getTitle", in_args, out_args);
}

std::string getTitle(SScriptCallBack *p, int handle)
{
    getTitle_in in_args;
    in_args.handle = handle;
    getTitle_out out_args;
    getTitle(p, &in_args, &out_args);
    return out_args.title;
}

void getTitle(SScriptCallBack *p, getTitle_out *out_args, int handle)
{
    getTitle_in in_args;
    in_args.handle = handle;
    getTitle(p, &in_args, out_args);
}

void getTitle_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getTitle";

    getTitle_in in_args;
    getTitle_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getTitle(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (title) of type std::string
            write__std__string(out_args.title, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (title): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (title)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (title)): ";
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

setTitle_in::setTitle_in()
{
    suppressEvents = true;
}

setTitle_out::setTitle_out()
{
}

void setTitle(SScriptCallBack *p, setTitle_in *in_args, setTitle_out *out_args)
{
    setTitle(p, "simExtCustomUI_setTitle", in_args, out_args);
}

void setTitle(SScriptCallBack *p, int handle, std::string title, bool suppressEvents)
{
    setTitle_in in_args;
    in_args.handle = handle;
    in_args.title = title;
    in_args.suppressEvents = suppressEvents;
    setTitle_out out_args;
    setTitle(p, &in_args, &out_args);
}

void setTitle(SScriptCallBack *p, setTitle_out *out_args, int handle, std::string title, bool suppressEvents)
{
    setTitle_in in_args;
    in_args.handle = handle;
    in_args.title = title;
    in_args.suppressEvents = suppressEvents;
    setTitle(p, &in_args, out_args);
}

void setTitle_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setTitle";

    setTitle_in in_args;
    setTitle_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 2)
            throw exception("too few arguments");
        if(numArgs > 3)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (title) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.title));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (title): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (title): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (title): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setTitle(p, cmd, &in_args, &out_args);
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

setImageData_in::setImageData_in()
{
}

setImageData_out::setImageData_out()
{
}

void setImageData(SScriptCallBack *p, setImageData_in *in_args, setImageData_out *out_args)
{
    setImageData(p, "simExtCustomUI_setImageData", in_args, out_args);
}

void setImageData(SScriptCallBack *p, int handle, int id, std::string data, int width, int height)
{
    setImageData_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.data = data;
    in_args.width = width;
    in_args.height = height;
    setImageData_out out_args;
    setImageData(p, &in_args, &out_args);
}

void setImageData(SScriptCallBack *p, setImageData_out *out_args, int handle, int id, std::string data, int width, int height)
{
    setImageData_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.data = data;
    in_args.width = width;
    in_args.height = height;
    setImageData(p, &in_args, out_args);
}

void setImageData_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setImageData";

    setImageData_in in_args;
    setImageData_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 5)
            throw exception("too few arguments");
        if(numArgs > 5)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (data) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.data));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (data): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (data): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (data): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (width) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.width));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (width): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (width): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (width): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (height) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.height));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (height): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (height): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (height): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setImageData(p, cmd, &in_args, &out_args);
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

setEnabled_in::setEnabled_in()
{
    suppressEvents = true;
}

setEnabled_out::setEnabled_out()
{
}

void setEnabled(SScriptCallBack *p, setEnabled_in *in_args, setEnabled_out *out_args)
{
    setEnabled(p, "simExtCustomUI_setEnabled", in_args, out_args);
}

void setEnabled(SScriptCallBack *p, int handle, int id, bool enabled, bool suppressEvents)
{
    setEnabled_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.enabled = enabled;
    in_args.suppressEvents = suppressEvents;
    setEnabled_out out_args;
    setEnabled(p, &in_args, &out_args);
}

void setEnabled(SScriptCallBack *p, setEnabled_out *out_args, int handle, int id, bool enabled, bool suppressEvents)
{
    setEnabled_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.enabled = enabled;
    in_args.suppressEvents = suppressEvents;
    setEnabled(p, &in_args, out_args);
}

void setEnabled_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setEnabled";

    setEnabled_in in_args;
    setEnabled_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (enabled) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.enabled));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (enabled): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (enabled): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (enabled): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setEnabled(p, cmd, &in_args, &out_args);
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

getCurrentTab_in::getCurrentTab_in()
{
}

getCurrentTab_out::getCurrentTab_out()
{
}

void getCurrentTab(SScriptCallBack *p, getCurrentTab_in *in_args, getCurrentTab_out *out_args)
{
    getCurrentTab(p, "simExtCustomUI_getCurrentTab", in_args, out_args);
}

int getCurrentTab(SScriptCallBack *p, int handle, int id)
{
    getCurrentTab_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getCurrentTab_out out_args;
    getCurrentTab(p, &in_args, &out_args);
    return out_args.index;
}

void getCurrentTab(SScriptCallBack *p, getCurrentTab_out *out_args, int handle, int id)
{
    getCurrentTab_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getCurrentTab(p, &in_args, out_args);
}

void getCurrentTab_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getCurrentTab";

    getCurrentTab_in in_args;
    getCurrentTab_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getCurrentTab(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (index) of type int
            write__int(out_args.index, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (index): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (index)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (index)): ";
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

setCurrentTab_in::setCurrentTab_in()
{
    suppressEvents = true;
}

setCurrentTab_out::setCurrentTab_out()
{
}

void setCurrentTab(SScriptCallBack *p, setCurrentTab_in *in_args, setCurrentTab_out *out_args)
{
    setCurrentTab(p, "simExtCustomUI_setCurrentTab", in_args, out_args);
}

void setCurrentTab(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents)
{
    setCurrentTab_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setCurrentTab_out out_args;
    setCurrentTab(p, &in_args, &out_args);
}

void setCurrentTab(SScriptCallBack *p, setCurrentTab_out *out_args, int handle, int id, int index, bool suppressEvents)
{
    setCurrentTab_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.index = index;
    in_args.suppressEvents = suppressEvents;
    setCurrentTab(p, &in_args, out_args);
}

void setCurrentTab_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setCurrentTab";

    setCurrentTab_in in_args;
    setCurrentTab_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (index) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.index));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (index): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (index): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (suppressEvents) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.suppressEvents));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (suppressEvents): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setCurrentTab(p, cmd, &in_args, &out_args);
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

getWidgetVisibility_in::getWidgetVisibility_in()
{
}

getWidgetVisibility_out::getWidgetVisibility_out()
{
}

void getWidgetVisibility(SScriptCallBack *p, getWidgetVisibility_in *in_args, getWidgetVisibility_out *out_args)
{
    getWidgetVisibility(p, "simExtCustomUI_getWidgetVisibility", in_args, out_args);
}

bool getWidgetVisibility(SScriptCallBack *p, int handle, int id)
{
    getWidgetVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getWidgetVisibility_out out_args;
    getWidgetVisibility(p, &in_args, &out_args);
    return out_args.visibility;
}

void getWidgetVisibility(SScriptCallBack *p, getWidgetVisibility_out *out_args, int handle, int id)
{
    getWidgetVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    getWidgetVisibility(p, &in_args, out_args);
}

void getWidgetVisibility_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getWidgetVisibility";

    getWidgetVisibility_in in_args;
    getWidgetVisibility_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getWidgetVisibility(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (visibility) of type bool
            write__bool(out_args.visibility, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (visibility): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (visibility)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (visibility)): ";
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

setWidgetVisibility_in::setWidgetVisibility_in()
{
}

setWidgetVisibility_out::setWidgetVisibility_out()
{
}

void setWidgetVisibility(SScriptCallBack *p, setWidgetVisibility_in *in_args, setWidgetVisibility_out *out_args)
{
    setWidgetVisibility(p, "simExtCustomUI_setWidgetVisibility", in_args, out_args);
}

void setWidgetVisibility(SScriptCallBack *p, int handle, int id, bool visibility)
{
    setWidgetVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.visibility = visibility;
    setWidgetVisibility_out out_args;
    setWidgetVisibility(p, &in_args, &out_args);
}

void setWidgetVisibility(SScriptCallBack *p, setWidgetVisibility_out *out_args, int handle, int id, bool visibility)
{
    setWidgetVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.visibility = visibility;
    setWidgetVisibility(p, &in_args, out_args);
}

void setWidgetVisibility_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setWidgetVisibility";

    setWidgetVisibility_in in_args;
    setWidgetVisibility_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (visibility) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.visibility));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (visibility): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (visibility): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (visibility): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setWidgetVisibility(p, cmd, &in_args, &out_args);
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

getCurrentEditWidget_in::getCurrentEditWidget_in()
{
}

getCurrentEditWidget_out::getCurrentEditWidget_out()
{
    id = -1;
}

void getCurrentEditWidget(SScriptCallBack *p, getCurrentEditWidget_in *in_args, getCurrentEditWidget_out *out_args)
{
    getCurrentEditWidget(p, "simExtCustomUI_getCurrentEditWidget", in_args, out_args);
}

int getCurrentEditWidget(SScriptCallBack *p, int handle)
{
    getCurrentEditWidget_in in_args;
    in_args.handle = handle;
    getCurrentEditWidget_out out_args;
    getCurrentEditWidget(p, &in_args, &out_args);
    return out_args.id;
}

void getCurrentEditWidget(SScriptCallBack *p, getCurrentEditWidget_out *out_args, int handle)
{
    getCurrentEditWidget_in in_args;
    in_args.handle = handle;
    getCurrentEditWidget(p, &in_args, out_args);
}

void getCurrentEditWidget_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getCurrentEditWidget";

    getCurrentEditWidget_in in_args;
    getCurrentEditWidget_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getCurrentEditWidget(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (id) of type int
            write__int(out_args.id, p->stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (id): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (id)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (id)): ";
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

setCurrentEditWidget_in::setCurrentEditWidget_in()
{
}

setCurrentEditWidget_out::setCurrentEditWidget_out()
{
}

void setCurrentEditWidget(SScriptCallBack *p, setCurrentEditWidget_in *in_args, setCurrentEditWidget_out *out_args)
{
    setCurrentEditWidget(p, "simExtCustomUI_setCurrentEditWidget", in_args, out_args);
}

void setCurrentEditWidget(SScriptCallBack *p, int handle, int id)
{
    setCurrentEditWidget_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    setCurrentEditWidget_out out_args;
    setCurrentEditWidget(p, &in_args, &out_args);
}

void setCurrentEditWidget(SScriptCallBack *p, setCurrentEditWidget_out *out_args, int handle, int id)
{
    setCurrentEditWidget_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    setCurrentEditWidget(p, &in_args, out_args);
}

void setCurrentEditWidget_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setCurrentEditWidget";

    setCurrentEditWidget_in in_args;
    setCurrentEditWidget_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setCurrentEditWidget(p, cmd, &in_args, &out_args);
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

replot_in::replot_in()
{
}

replot_out::replot_out()
{
}

void replot(SScriptCallBack *p, replot_in *in_args, replot_out *out_args)
{
    replot(p, "simExtCustomUI_replot", in_args, out_args);
}

void replot(SScriptCallBack *p, int handle, int id)
{
    replot_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    replot_out out_args;
    replot(p, &in_args, &out_args);
}

void replot(SScriptCallBack *p, replot_out *out_args, int handle, int id)
{
    replot_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    replot(p, &in_args, out_args);
}

void replot_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_replot";

    replot_in in_args;
    replot_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        replot(p, cmd, &in_args, &out_args);
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

addCurve_in::addCurve_in()
{
}

addCurve_out::addCurve_out()
{
}

void addCurve(SScriptCallBack *p, addCurve_in *in_args, addCurve_out *out_args)
{
    addCurve(p, "simExtCustomUI_addCurve", in_args, out_args);
}

void addCurve(SScriptCallBack *p, int handle, int id, int type, std::string name, std::vector<int> color, int style, curve_options options)
{
    addCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.type = type;
    in_args.name = name;
    in_args.color = color;
    in_args.style = style;
    in_args.options = options;
    addCurve_out out_args;
    addCurve(p, &in_args, &out_args);
}

void addCurve(SScriptCallBack *p, addCurve_out *out_args, int handle, int id, int type, std::string name, std::vector<int> color, int style, curve_options options)
{
    addCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.type = type;
    in_args.name = name;
    in_args.color = color;
    in_args.style = style;
    in_args.options = options;
    addCurve(p, &in_args, out_args);
}

void addCurve_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_addCurve";

    addCurve_in in_args;
    addCurve_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 7)
            throw exception("too few arguments");
        if(numArgs > 7)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (type) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.type));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (type): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (type): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (type): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (color) of type array of std::vector<int> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.color.resize(sz);
                simGetStackInt32TableE(p->stackID, &(in_args.color[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (color): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (color): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (color): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (style) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.style));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 6 (style): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 6 (style): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 6 (style): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 7)
        {
            try
            {
                // read input argument 7 (options) of type curve_options
                simMoveStackItemToTopE(p->stackID, 0);
                read__curve_options(p->stackID, &(in_args.options));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 7 (options): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 7 (options): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 7 (options): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        addCurve(p, cmd, &in_args, &out_args);
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

addCurveTimePoints_in::addCurveTimePoints_in()
{
}

addCurveTimePoints_out::addCurveTimePoints_out()
{
}

void addCurveTimePoints(SScriptCallBack *p, addCurveTimePoints_in *in_args, addCurveTimePoints_out *out_args)
{
    addCurveTimePoints(p, "simExtCustomUI_addCurveTimePoints", in_args, out_args);
}

void addCurveTimePoints(SScriptCallBack *p, int handle, int id, std::string name, std::vector<double> x, std::vector<double> y)
{
    addCurveTimePoints_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.x = x;
    in_args.y = y;
    addCurveTimePoints_out out_args;
    addCurveTimePoints(p, &in_args, &out_args);
}

void addCurveTimePoints(SScriptCallBack *p, addCurveTimePoints_out *out_args, int handle, int id, std::string name, std::vector<double> x, std::vector<double> y)
{
    addCurveTimePoints_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.x = x;
    in_args.y = y;
    addCurveTimePoints(p, &in_args, out_args);
}

void addCurveTimePoints_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_addCurveTimePoints";

    addCurveTimePoints_in in_args;
    addCurveTimePoints_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 5)
            throw exception("too few arguments");
        if(numArgs > 5)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (x) of type array of std::vector<double> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.x.resize(sz);
                simGetStackDoubleTableE(p->stackID, &(in_args.x[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (x): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (x): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (x): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (y) of type array of std::vector<double> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.y.resize(sz);
                simGetStackDoubleTableE(p->stackID, &(in_args.y[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (y): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (y): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (y): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        addCurveTimePoints(p, cmd, &in_args, &out_args);
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

addCurveXYPoints_in::addCurveXYPoints_in()
{
}

addCurveXYPoints_out::addCurveXYPoints_out()
{
}

void addCurveXYPoints(SScriptCallBack *p, addCurveXYPoints_in *in_args, addCurveXYPoints_out *out_args)
{
    addCurveXYPoints(p, "simExtCustomUI_addCurveXYPoints", in_args, out_args);
}

void addCurveXYPoints(SScriptCallBack *p, int handle, int id, std::string name, std::vector<double> t, std::vector<double> x, std::vector<double> y)
{
    addCurveXYPoints_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.t = t;
    in_args.x = x;
    in_args.y = y;
    addCurveXYPoints_out out_args;
    addCurveXYPoints(p, &in_args, &out_args);
}

void addCurveXYPoints(SScriptCallBack *p, addCurveXYPoints_out *out_args, int handle, int id, std::string name, std::vector<double> t, std::vector<double> x, std::vector<double> y)
{
    addCurveXYPoints_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.t = t;
    in_args.x = x;
    in_args.y = y;
    addCurveXYPoints(p, &in_args, out_args);
}

void addCurveXYPoints_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_addCurveXYPoints";

    addCurveXYPoints_in in_args;
    addCurveXYPoints_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 6)
            throw exception("too few arguments");
        if(numArgs > 6)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (t) of type array of std::vector<double> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.t.resize(sz);
                simGetStackDoubleTableE(p->stackID, &(in_args.t[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (t): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (t): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (t): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (x) of type array of std::vector<double> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.x.resize(sz);
                simGetStackDoubleTableE(p->stackID, &(in_args.x[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (x): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (x): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (x): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (y) of type array of std::vector<double> using fast function
                simMoveStackItemToTopE(p->stackID, 0);
                int sz = simGetStackTableInfoE(p->stackID, 0);
                if(sz < 0)
                    throw exception("expected array");
                if(simGetStackTableInfoE(p->stackID, 2) != 1)
                    throw exception("fast_write_type reader exception #1");
                in_args.y.resize(sz);
                simGetStackDoubleTableE(p->stackID, &(in_args.y[0]), sz);
                simPopStackItemE(p->stackID, 1);
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 6 (y): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 6 (y): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 6 (y): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        addCurveXYPoints(p, cmd, &in_args, &out_args);
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

clearCurve_in::clearCurve_in()
{
}

clearCurve_out::clearCurve_out()
{
}

void clearCurve(SScriptCallBack *p, clearCurve_in *in_args, clearCurve_out *out_args)
{
    clearCurve(p, "simExtCustomUI_clearCurve", in_args, out_args);
}

void clearCurve(SScriptCallBack *p, int handle, int id, std::string name)
{
    clearCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    clearCurve_out out_args;
    clearCurve(p, &in_args, &out_args);
}

void clearCurve(SScriptCallBack *p, clearCurve_out *out_args, int handle, int id, std::string name)
{
    clearCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    clearCurve(p, &in_args, out_args);
}

void clearCurve_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_clearCurve";

    clearCurve_in in_args;
    clearCurve_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        clearCurve(p, cmd, &in_args, &out_args);
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

removeCurve_in::removeCurve_in()
{
}

removeCurve_out::removeCurve_out()
{
}

void removeCurve(SScriptCallBack *p, removeCurve_in *in_args, removeCurve_out *out_args)
{
    removeCurve(p, "simExtCustomUI_removeCurve", in_args, out_args);
}

void removeCurve(SScriptCallBack *p, int handle, int id, std::string name)
{
    removeCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    removeCurve_out out_args;
    removeCurve(p, &in_args, &out_args);
}

void removeCurve(SScriptCallBack *p, removeCurve_out *out_args, int handle, int id, std::string name)
{
    removeCurve_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    removeCurve(p, &in_args, out_args);
}

void removeCurve_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_removeCurve";

    removeCurve_in in_args;
    removeCurve_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        removeCurve(p, cmd, &in_args, &out_args);
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

setPlotRanges_in::setPlotRanges_in()
{
}

setPlotRanges_out::setPlotRanges_out()
{
}

void setPlotRanges(SScriptCallBack *p, setPlotRanges_in *in_args, setPlotRanges_out *out_args)
{
    setPlotRanges(p, "simExtCustomUI_setPlotRanges", in_args, out_args);
}

void setPlotRanges(SScriptCallBack *p, int handle, int id, double xmin, double xmax, double ymin, double ymax)
{
    setPlotRanges_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    setPlotRanges_out out_args;
    setPlotRanges(p, &in_args, &out_args);
}

void setPlotRanges(SScriptCallBack *p, setPlotRanges_out *out_args, int handle, int id, double xmin, double xmax, double ymin, double ymax)
{
    setPlotRanges_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    setPlotRanges(p, &in_args, out_args);
}

void setPlotRanges_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotRanges";

    setPlotRanges_in in_args;
    setPlotRanges_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 6)
            throw exception("too few arguments");
        if(numArgs > 6)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (xmin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (xmin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (xmax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (xmax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (ymin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (ymin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (ymin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (ymin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (ymax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 6 (ymax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 6 (ymax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 6 (ymax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotRanges(p, cmd, &in_args, &out_args);
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

setPlotXRange_in::setPlotXRange_in()
{
}

setPlotXRange_out::setPlotXRange_out()
{
}

void setPlotXRange(SScriptCallBack *p, setPlotXRange_in *in_args, setPlotXRange_out *out_args)
{
    setPlotXRange(p, "simExtCustomUI_setPlotXRange", in_args, out_args);
}

void setPlotXRange(SScriptCallBack *p, int handle, int id, double xmin, double xmax)
{
    setPlotXRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    setPlotXRange_out out_args;
    setPlotXRange(p, &in_args, &out_args);
}

void setPlotXRange(SScriptCallBack *p, setPlotXRange_out *out_args, int handle, int id, double xmin, double xmax)
{
    setPlotXRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    setPlotXRange(p, &in_args, out_args);
}

void setPlotXRange_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotXRange";

    setPlotXRange_in in_args;
    setPlotXRange_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (xmin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (xmin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (xmax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (xmax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotXRange(p, cmd, &in_args, &out_args);
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

setPlotYRange_in::setPlotYRange_in()
{
}

setPlotYRange_out::setPlotYRange_out()
{
}

void setPlotYRange(SScriptCallBack *p, setPlotYRange_in *in_args, setPlotYRange_out *out_args)
{
    setPlotYRange(p, "simExtCustomUI_setPlotYRange", in_args, out_args);
}

void setPlotYRange(SScriptCallBack *p, int handle, int id, double ymin, double ymax)
{
    setPlotYRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    setPlotYRange_out out_args;
    setPlotYRange(p, &in_args, &out_args);
}

void setPlotYRange(SScriptCallBack *p, setPlotYRange_out *out_args, int handle, int id, double ymin, double ymax)
{
    setPlotYRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    setPlotYRange(p, &in_args, out_args);
}

void setPlotYRange_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotYRange";

    setPlotYRange_in in_args;
    setPlotYRange_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (ymin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (ymin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (ymin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (ymin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (ymax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (ymax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (ymax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (ymax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotYRange(p, cmd, &in_args, &out_args);
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

growPlotRanges_in::growPlotRanges_in()
{
}

growPlotRanges_out::growPlotRanges_out()
{
}

void growPlotRanges(SScriptCallBack *p, growPlotRanges_in *in_args, growPlotRanges_out *out_args)
{
    growPlotRanges(p, "simExtCustomUI_growPlotRanges", in_args, out_args);
}

void growPlotRanges(SScriptCallBack *p, int handle, int id, double xmin, double xmax, double ymin, double ymax)
{
    growPlotRanges_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    growPlotRanges_out out_args;
    growPlotRanges(p, &in_args, &out_args);
}

void growPlotRanges(SScriptCallBack *p, growPlotRanges_out *out_args, int handle, int id, double xmin, double xmax, double ymin, double ymax)
{
    growPlotRanges_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    growPlotRanges(p, &in_args, out_args);
}

void growPlotRanges_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_growPlotRanges";

    growPlotRanges_in in_args;
    growPlotRanges_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 6)
            throw exception("too few arguments");
        if(numArgs > 6)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (xmin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (xmin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (xmax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (xmax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (ymin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (ymin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (ymin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (ymin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (ymax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 6 (ymax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 6 (ymax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 6 (ymax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        growPlotRanges(p, cmd, &in_args, &out_args);
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

growPlotXRange_in::growPlotXRange_in()
{
}

growPlotXRange_out::growPlotXRange_out()
{
}

void growPlotXRange(SScriptCallBack *p, growPlotXRange_in *in_args, growPlotXRange_out *out_args)
{
    growPlotXRange(p, "simExtCustomUI_growPlotXRange", in_args, out_args);
}

void growPlotXRange(SScriptCallBack *p, int handle, int id, double xmin, double xmax)
{
    growPlotXRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    growPlotXRange_out out_args;
    growPlotXRange(p, &in_args, &out_args);
}

void growPlotXRange(SScriptCallBack *p, growPlotXRange_out *out_args, int handle, int id, double xmin, double xmax)
{
    growPlotXRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.xmin = xmin;
    in_args.xmax = xmax;
    growPlotXRange(p, &in_args, out_args);
}

void growPlotXRange_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_growPlotXRange";

    growPlotXRange_in in_args;
    growPlotXRange_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (xmin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (xmin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (xmin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (xmax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.xmax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (xmax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (xmax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        growPlotXRange(p, cmd, &in_args, &out_args);
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

growPlotYRange_in::growPlotYRange_in()
{
}

growPlotYRange_out::growPlotYRange_out()
{
}

void growPlotYRange(SScriptCallBack *p, growPlotYRange_in *in_args, growPlotYRange_out *out_args)
{
    growPlotYRange(p, "simExtCustomUI_growPlotYRange", in_args, out_args);
}

void growPlotYRange(SScriptCallBack *p, int handle, int id, double ymin, double ymax)
{
    growPlotYRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    growPlotYRange_out out_args;
    growPlotYRange(p, &in_args, &out_args);
}

void growPlotYRange(SScriptCallBack *p, growPlotYRange_out *out_args, int handle, int id, double ymin, double ymax)
{
    growPlotYRange_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.ymin = ymin;
    in_args.ymax = ymax;
    growPlotYRange(p, &in_args, out_args);
}

void growPlotYRange_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_growPlotYRange";

    growPlotYRange_in in_args;
    growPlotYRange_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (ymin) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymin));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (ymin): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (ymin): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (ymin): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (ymax) of type double
                simMoveStackItemToTopE(p->stackID, 0);
                read__double(p->stackID, &(in_args.ymax));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (ymax): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (ymax): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (ymax): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        growPlotYRange(p, cmd, &in_args, &out_args);
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

setPlotLabels_in::setPlotLabels_in()
{
}

setPlotLabels_out::setPlotLabels_out()
{
}

void setPlotLabels(SScriptCallBack *p, setPlotLabels_in *in_args, setPlotLabels_out *out_args)
{
    setPlotLabels(p, "simExtCustomUI_setPlotLabels", in_args, out_args);
}

void setPlotLabels(SScriptCallBack *p, int handle, int id, std::string x, std::string y)
{
    setPlotLabels_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.x = x;
    in_args.y = y;
    setPlotLabels_out out_args;
    setPlotLabels(p, &in_args, &out_args);
}

void setPlotLabels(SScriptCallBack *p, setPlotLabels_out *out_args, int handle, int id, std::string x, std::string y)
{
    setPlotLabels_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.x = x;
    in_args.y = y;
    setPlotLabels(p, &in_args, out_args);
}

void setPlotLabels_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotLabels";

    setPlotLabels_in in_args;
    setPlotLabels_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 4)
            throw exception("too few arguments");
        if(numArgs > 4)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (x) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.x));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (x): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (x): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (x): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (y) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.y));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (y): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (y): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (y): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotLabels(p, cmd, &in_args, &out_args);
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

setPlotXLabel_in::setPlotXLabel_in()
{
}

setPlotXLabel_out::setPlotXLabel_out()
{
}

void setPlotXLabel(SScriptCallBack *p, setPlotXLabel_in *in_args, setPlotXLabel_out *out_args)
{
    setPlotXLabel(p, "simExtCustomUI_setPlotXLabel", in_args, out_args);
}

void setPlotXLabel(SScriptCallBack *p, int handle, int id, std::string label)
{
    setPlotXLabel_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.label = label;
    setPlotXLabel_out out_args;
    setPlotXLabel(p, &in_args, &out_args);
}

void setPlotXLabel(SScriptCallBack *p, setPlotXLabel_out *out_args, int handle, int id, std::string label)
{
    setPlotXLabel_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.label = label;
    setPlotXLabel(p, &in_args, out_args);
}

void setPlotXLabel_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotXLabel";

    setPlotXLabel_in in_args;
    setPlotXLabel_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (label) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.label));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (label): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (label): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (label): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotXLabel(p, cmd, &in_args, &out_args);
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

setPlotYLabel_in::setPlotYLabel_in()
{
}

setPlotYLabel_out::setPlotYLabel_out()
{
}

void setPlotYLabel(SScriptCallBack *p, setPlotYLabel_in *in_args, setPlotYLabel_out *out_args)
{
    setPlotYLabel(p, "simExtCustomUI_setPlotYLabel", in_args, out_args);
}

void setPlotYLabel(SScriptCallBack *p, int handle, int id, std::string label)
{
    setPlotYLabel_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.label = label;
    setPlotYLabel_out out_args;
    setPlotYLabel(p, &in_args, &out_args);
}

void setPlotYLabel(SScriptCallBack *p, setPlotYLabel_out *out_args, int handle, int id, std::string label)
{
    setPlotYLabel_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.label = label;
    setPlotYLabel(p, &in_args, out_args);
}

void setPlotYLabel_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setPlotYLabel";

    setPlotYLabel_in in_args;
    setPlotYLabel_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (label) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.label));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (label): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (label): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (label): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setPlotYLabel(p, cmd, &in_args, &out_args);
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

rescaleAxes_in::rescaleAxes_in()
{
    onlyEnlargeX = false;
    onlyEnlargeY = false;
}

rescaleAxes_out::rescaleAxes_out()
{
}

void rescaleAxes(SScriptCallBack *p, rescaleAxes_in *in_args, rescaleAxes_out *out_args)
{
    rescaleAxes(p, "simExtCustomUI_rescaleAxes", in_args, out_args);
}

void rescaleAxes(SScriptCallBack *p, int handle, int id, std::string name, bool onlyEnlargeX, bool onlyEnlargeY)
{
    rescaleAxes_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.onlyEnlargeX = onlyEnlargeX;
    in_args.onlyEnlargeY = onlyEnlargeY;
    rescaleAxes_out out_args;
    rescaleAxes(p, &in_args, &out_args);
}

void rescaleAxes(SScriptCallBack *p, rescaleAxes_out *out_args, int handle, int id, std::string name, bool onlyEnlargeX, bool onlyEnlargeY)
{
    rescaleAxes_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    in_args.onlyEnlargeX = onlyEnlargeX;
    in_args.onlyEnlargeY = onlyEnlargeY;
    rescaleAxes(p, &in_args, out_args);
}

void rescaleAxes_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_rescaleAxes";

    rescaleAxes_in in_args;
    rescaleAxes_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 3)
            throw exception("too few arguments");
        if(numArgs > 5)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (onlyEnlargeX) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.onlyEnlargeX));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (onlyEnlargeX): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (onlyEnlargeX): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (onlyEnlargeX): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (onlyEnlargeY) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.onlyEnlargeY));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (onlyEnlargeY): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (onlyEnlargeY): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (onlyEnlargeY): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        rescaleAxes(p, cmd, &in_args, &out_args);
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

rescaleAxesAll_in::rescaleAxesAll_in()
{
    onlyEnlargeX = false;
    onlyEnlargeY = false;
}

rescaleAxesAll_out::rescaleAxesAll_out()
{
}

void rescaleAxesAll(SScriptCallBack *p, rescaleAxesAll_in *in_args, rescaleAxesAll_out *out_args)
{
    rescaleAxesAll(p, "simExtCustomUI_rescaleAxesAll", in_args, out_args);
}

void rescaleAxesAll(SScriptCallBack *p, int handle, int id, bool onlyEnlargeX, bool onlyEnlargeY)
{
    rescaleAxesAll_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.onlyEnlargeX = onlyEnlargeX;
    in_args.onlyEnlargeY = onlyEnlargeY;
    rescaleAxesAll_out out_args;
    rescaleAxesAll(p, &in_args, &out_args);
}

void rescaleAxesAll(SScriptCallBack *p, rescaleAxesAll_out *out_args, int handle, int id, bool onlyEnlargeX, bool onlyEnlargeY)
{
    rescaleAxesAll_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.onlyEnlargeX = onlyEnlargeX;
    in_args.onlyEnlargeY = onlyEnlargeY;
    rescaleAxesAll(p, &in_args, out_args);
}

void rescaleAxesAll_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_rescaleAxesAll";

    rescaleAxesAll_in in_args;
    rescaleAxesAll_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (onlyEnlargeX) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.onlyEnlargeX));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (onlyEnlargeX): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (onlyEnlargeX): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (onlyEnlargeX): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (onlyEnlargeY) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.onlyEnlargeY));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (onlyEnlargeY): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (onlyEnlargeY): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (onlyEnlargeY): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        rescaleAxesAll(p, cmd, &in_args, &out_args);
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

setMouseOptions_in::setMouseOptions_in()
{
}

setMouseOptions_out::setMouseOptions_out()
{
}

void setMouseOptions(SScriptCallBack *p, setMouseOptions_in *in_args, setMouseOptions_out *out_args)
{
    setMouseOptions(p, "simExtCustomUI_setMouseOptions", in_args, out_args);
}

void setMouseOptions(SScriptCallBack *p, int handle, int id, bool panX, bool panY, bool zoomX, bool zoomY)
{
    setMouseOptions_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.panX = panX;
    in_args.panY = panY;
    in_args.zoomX = zoomX;
    in_args.zoomY = zoomY;
    setMouseOptions_out out_args;
    setMouseOptions(p, &in_args, &out_args);
}

void setMouseOptions(SScriptCallBack *p, setMouseOptions_out *out_args, int handle, int id, bool panX, bool panY, bool zoomX, bool zoomY)
{
    setMouseOptions_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.panX = panX;
    in_args.panY = panY;
    in_args.zoomX = zoomX;
    in_args.zoomY = zoomY;
    setMouseOptions(p, &in_args, out_args);
}

void setMouseOptions_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setMouseOptions";

    setMouseOptions_in in_args;
    setMouseOptions_out out_args;

    try
    {
        // check argument count

        int numArgs = simGetStackSizeE(p->stackID);
        if(numArgs < 6)
            throw exception("too few arguments");
        if(numArgs > 6)
            throw exception("too many arguments");

        // read input arguments from stack

        if(numArgs >= 1)
        {
            try
            {
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (panX) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.panX));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (panX): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (panX): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (panX): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 4)
        {
            try
            {
                // read input argument 4 (panY) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.panY));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 4 (panY): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 4 (panY): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 4 (panY): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 5)
        {
            try
            {
                // read input argument 5 (zoomX) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.zoomX));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 5 (zoomX): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 5 (zoomX): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 5 (zoomX): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 6)
        {
            try
            {
                // read input argument 6 (zoomY) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.zoomY));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 6 (zoomY): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 6 (zoomY): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 6 (zoomY): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setMouseOptions(p, cmd, &in_args, &out_args);
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

setLegendVisibility_in::setLegendVisibility_in()
{
}

setLegendVisibility_out::setLegendVisibility_out()
{
}

void setLegendVisibility(SScriptCallBack *p, setLegendVisibility_in *in_args, setLegendVisibility_out *out_args)
{
    setLegendVisibility(p, "simExtCustomUI_setLegendVisibility", in_args, out_args);
}

void setLegendVisibility(SScriptCallBack *p, int handle, int id, bool visible)
{
    setLegendVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.visible = visible;
    setLegendVisibility_out out_args;
    setLegendVisibility(p, &in_args, &out_args);
}

void setLegendVisibility(SScriptCallBack *p, setLegendVisibility_out *out_args, int handle, int id, bool visible)
{
    setLegendVisibility_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.visible = visible;
    setLegendVisibility(p, &in_args, out_args);
}

void setLegendVisibility_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_setLegendVisibility";

    setLegendVisibility_in in_args;
    setLegendVisibility_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (visible) of type bool
                simMoveStackItemToTopE(p->stackID, 0);
                read__bool(p->stackID, &(in_args.visible));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (visible): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (visible): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (visible): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        setLegendVisibility(p, cmd, &in_args, &out_args);
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

getCurveData_in::getCurveData_in()
{
}

getCurveData_out::getCurveData_out()
{
}

void getCurveData(SScriptCallBack *p, getCurveData_in *in_args, getCurveData_out *out_args)
{
    getCurveData(p, "simExtCustomUI_getCurveData", in_args, out_args);
}

void getCurveData(SScriptCallBack *p, getCurveData_out *out_args, int handle, int id, std::string name)
{
    getCurveData_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.name = name;
    getCurveData(p, &in_args, out_args);
}

void getCurveData_callback(SScriptCallBack *p)
{
    const char *cmd = "simExtCustomUI_getCurveData";

    getCurveData_in in_args;
    getCurveData_out out_args;

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
                // read input argument 1 (handle) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.handle));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 1 (handle): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 1 (handle): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 2)
        {
            try
            {
                // read input argument 2 (id) of type int
                simMoveStackItemToTopE(p->stackID, 0);
                read__int(p->stackID, &(in_args.id));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 2 (id): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 2 (id): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        if(numArgs >= 3)
        {
            try
            {
                // read input argument 3 (name) of type std::string
                simMoveStackItemToTopE(p->stackID, 0);
                read__std__string(p->stackID, &(in_args.name));
            }
            catch(std::exception& ex)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += ex.what();
                throw exception(msg);
            }
            catch(std::string& s)
            {
                std::string msg = "read in arg 3 (name): ";
                msg += s;
                throw exception(msg);
            }
            catch(int& n)
            {
                std::stringstream ss;
                ss << "error #" << n;
                std::string msg = "read in arg 3 (name): ";
                msg += ss.str();
                throw exception(msg);
            }
        }


        // clear stack
        simPopStackItemE(p->stackID, 0);


        getCurveData(p, cmd, &in_args, &out_args);
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
            // write output argument 1 (t) of type array of std::vector<double>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.t.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__double(out_args.t[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 1 (t): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 1 (t)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 1 (t)): ";
            msg += ss.str();
            throw exception(msg);
        }
        try
        {
            // write output argument 2 (x) of type array of std::vector<double>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.x.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__double(out_args.x[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 2 (x): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 2 (x)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 2 (x)): ";
            msg += ss.str();
            throw exception(msg);
        }
        try
        {
            // write output argument 3 (y) of type array of std::vector<double>
            simPushTableOntoStackE(p->stackID);
            for(int i = 0; i < out_args.y.size(); i++)
            {
                write__int(i + 1, p->stackID);
                write__double(out_args.y[i], p->stackID);
                simInsertDataIntoStackTableE(p->stackID);
            }
        }
        catch(std::exception& ex)
        {
            std::string msg = "write out arg 3 (y): ";
            msg += ex.what();
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = "write out arg 3 (y)): ";
            msg += s;
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n;
            std::string msg = "write out arg 3 (y)): ";
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

onclickCallback_in::onclickCallback_in()
{
}

onclickCallback_out::onclickCallback_out()
{
}

void onclickCallback(simInt scriptId, const char *func, int handle, int id)
{
    onclickCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    onclickCallback_out out_args;
    if(!onclickCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onclickCallback failed");
}



bool onclickCallback(simInt scriptId, const char *func, onclickCallback_in *in_args, onclickCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

onchangeIntCallback_in::onchangeIntCallback_in()
{
}

onchangeIntCallback_out::onchangeIntCallback_out()
{
}

void onchangeIntCallback(simInt scriptId, const char *func, int handle, int id, int value)
{
    onchangeIntCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    onchangeIntCallback_out out_args;
    if(!onchangeIntCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onchangeIntCallback failed");
}



bool onchangeIntCallback(simInt scriptId, const char *func, onchangeIntCallback_in *in_args, onchangeIntCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (value) of type int
            write__int(in_args->value, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (value)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (value))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (value)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

onchangeDoubleCallback_in::onchangeDoubleCallback_in()
{
}

onchangeDoubleCallback_out::onchangeDoubleCallback_out()
{
}

void onchangeDoubleCallback(simInt scriptId, const char *func, int handle, int id, float value)
{
    onchangeDoubleCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    onchangeDoubleCallback_out out_args;
    if(!onchangeDoubleCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onchangeDoubleCallback failed");
}



bool onchangeDoubleCallback(simInt scriptId, const char *func, onchangeDoubleCallback_in *in_args, onchangeDoubleCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (value) of type float
            write__float(in_args->value, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (value)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (value))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (value)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

onchangeStringCallback_in::onchangeStringCallback_in()
{
}

onchangeStringCallback_out::onchangeStringCallback_out()
{
}

void onchangeStringCallback(simInt scriptId, const char *func, int handle, int id, std::string value)
{
    onchangeStringCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    onchangeStringCallback_out out_args;
    if(!onchangeStringCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onchangeStringCallback failed");
}



bool onchangeStringCallback(simInt scriptId, const char *func, onchangeStringCallback_in *in_args, onchangeStringCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (value) of type std::string
            write__std__string(in_args->value, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (value)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (value))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (value)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

oneditingfinishedCallback_in::oneditingfinishedCallback_in()
{
}

oneditingfinishedCallback_out::oneditingfinishedCallback_out()
{
}

void oneditingfinishedCallback(simInt scriptId, const char *func, int handle, int id, std::string value)
{
    oneditingfinishedCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.value = value;
    oneditingfinishedCallback_out out_args;
    if(!oneditingfinishedCallback(scriptId, func, &in_args, &out_args))
        throw exception("call oneditingfinishedCallback failed");
}



bool oneditingfinishedCallback(simInt scriptId, const char *func, oneditingfinishedCallback_in *in_args, oneditingfinishedCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (value) of type std::string
            write__std__string(in_args->value, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (value)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (value))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (value)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

oncloseCallback_in::oncloseCallback_in()
{
}

oncloseCallback_out::oncloseCallback_out()
{
}

void oncloseCallback(simInt scriptId, const char *func, int handle)
{
    oncloseCallback_in in_args;
    in_args.handle = handle;
    oncloseCallback_out out_args;
    if(!oncloseCallback(scriptId, func, &in_args, &out_args))
        throw exception("call oncloseCallback failed");
}



bool oncloseCallback(simInt scriptId, const char *func, oncloseCallback_in *in_args, oncloseCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

onPlottableClickCallback_in::onPlottableClickCallback_in()
{
}

onPlottableClickCallback_out::onPlottableClickCallback_out()
{
}

void onPlottableClickCallback(simInt scriptId, const char *func, int handle, int id, std::string curve, int index, float x, float y)
{
    onPlottableClickCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.curve = curve;
    in_args.index = index;
    in_args.x = x;
    in_args.y = y;
    onPlottableClickCallback_out out_args;
    if(!onPlottableClickCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onPlottableClickCallback failed");
}



bool onPlottableClickCallback(simInt scriptId, const char *func, onPlottableClickCallback_in *in_args, onPlottableClickCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (curve) of type std::string
            write__std__string(in_args->curve, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (curve)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (curve))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (curve)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 4 (index) of type int
            write__int(in_args->index, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 4 (index)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 4 (index))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 4 (index)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 5 (x) of type float
            write__float(in_args->x, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 5 (x)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 5 (x))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 5 (x)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 6 (y) of type float
            write__float(in_args->y, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 6 (y)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 6 (y))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 6 (y)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

onLegendClickCallback_in::onLegendClickCallback_in()
{
}

onLegendClickCallback_out::onLegendClickCallback_out()
{
}

void onLegendClickCallback(simInt scriptId, const char *func, int handle, int id, std::string curve)
{
    onLegendClickCallback_in in_args;
    in_args.handle = handle;
    in_args.id = id;
    in_args.curve = curve;
    onLegendClickCallback_out out_args;
    if(!onLegendClickCallback(scriptId, func, &in_args, &out_args))
        throw exception("call onLegendClickCallback failed");
}



bool onLegendClickCallback(simInt scriptId, const char *func, onLegendClickCallback_in *in_args, onLegendClickCallback_out *out_args)
{
    int stackID = -1;

    try
    {
        stackID = simCreateStackE();

        // write input arguments to stack

        try
        {
            // write input argument 1 (handle) of type int
            write__int(in_args->handle, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 1 (handle)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 1 (handle))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 1 (handle)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 2 (id) of type int
            write__int(in_args->id, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 2 (id)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 2 (id))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 2 (id)";
            throw exception(ss.str());
        }
        try
        {
            // write input argument 3 (curve) of type std::string
            write__std__string(in_args->curve, stackID);
        }
        catch(std::exception& ex)
        {
            std::string msg = ex.what();
            msg += " when writing input argument 3 (curve)";
            throw exception(msg);
        }
        catch(std::string& s)
        {
            std::string msg = s;
            msg += " (when writing input argument 3 (curve))";
            throw exception(msg);
        }
        catch(int& n)
        {
            std::stringstream ss;
            ss << "error #" << n << " when writing input argument 3 (curve)";
            throw exception(ss.str());
        }


        simCallScriptFunctionExE(scriptId, func, stackID);

        // read output arguments from stack


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
    catch(std::string& s)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        simSetLastError(func, s.c_str());
        return false;
    }
    catch(int& n)
    {
        if(stackID != -1)
            simReleaseStack(stackID); // don't throw
        std::stringstream ss;
        ss << "error #" << n;
        simSetLastError(func, ss.str().c_str());
        return false;
    }

    return true;
}

