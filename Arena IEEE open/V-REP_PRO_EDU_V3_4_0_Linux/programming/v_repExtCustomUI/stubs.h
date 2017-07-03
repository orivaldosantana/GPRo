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

struct curve_options
{
    int scatter_shape;
    int scatter_size;
    int line_size;
    int line_style;
    bool add_to_legend;
    bool selectable;
    bool track;

    curve_options();
};

void read__bool(int stack, bool *value);
void read__int(int stack, int *value);
void read__float(int stack, float *value);
void read__double(int stack, double *value);
void read__std__string(int stack, std::string *value);
void read__curve_options(int stack, curve_options *value);
void write__bool(bool value, int stack);
void write__int(int value, int stack);
void write__float(float value, int stack);
void write__double(double value, int stack);
void write__std__string(std::string value, int stack);
void write__curve_options(int stack, curve_options *value);

bool registerScriptStuff();

enum curve_type
{
    sim_customui_curve_type_time = 20400,
    sim_customui_curve_type_xy,
};

const char* curve_type_string(curve_type x);

enum curve_style
{
    sim_customui_curve_style_scatter = 22400,
    sim_customui_curve_style_line,
    sim_customui_curve_style_line_and_scatter,
    sim_customui_curve_style_step_left,
    sim_customui_curve_style_step_center,
    sim_customui_curve_style_step_right,
    sim_customui_curve_style_impulse,
};

const char* curve_style_string(curve_style x);

enum curve_scatter_shape
{
    sim_customui_curve_scatter_shape_none = 28400,
    sim_customui_curve_scatter_shape_dot,
    sim_customui_curve_scatter_shape_cross,
    sim_customui_curve_scatter_shape_plus,
    sim_customui_curve_scatter_shape_circle,
    sim_customui_curve_scatter_shape_disc,
    sim_customui_curve_scatter_shape_square,
    sim_customui_curve_scatter_shape_diamond,
    sim_customui_curve_scatter_shape_star,
    sim_customui_curve_scatter_shape_triangle,
    sim_customui_curve_scatter_shape_triangle_inverted,
    sim_customui_curve_scatter_shape_cross_square,
    sim_customui_curve_scatter_shape_plus_square,
    sim_customui_curve_scatter_shape_cross_circle,
    sim_customui_curve_scatter_shape_plus_circle,
    sim_customui_curve_scatter_shape_peace,
};

const char* curve_scatter_shape_string(curve_scatter_shape x);

enum line_style
{
    sim_customui_line_style_solid = 32000,
    sim_customui_line_style_dotted,
    sim_customui_line_style_dashed,
};

const char* line_style_string(line_style x);

struct create_in
{
    std::string xml;
    create_in();
};

struct create_out
{
    int uiHandle;
    create_out();
};

void create(SScriptCallBack *p, create_in *in, create_out *out);
int create(SScriptCallBack *p, std::string xml);
void create(SScriptCallBack *p, create_out *out, std::string xml);
void create_callback(SScriptCallBack *p);

struct destroy_in
{
    int handle;
    destroy_in();
};

struct destroy_out
{
    destroy_out();
};

void destroy(SScriptCallBack *p, destroy_in *in, destroy_out *out);
void destroy(SScriptCallBack *p, int handle);
void destroy(SScriptCallBack *p, destroy_out *out, int handle);
void destroy_callback(SScriptCallBack *p);

struct getSliderValue_in
{
    int handle;
    int id;
    getSliderValue_in();
};

struct getSliderValue_out
{
    int value;
    getSliderValue_out();
};

void getSliderValue(SScriptCallBack *p, getSliderValue_in *in, getSliderValue_out *out);
int getSliderValue(SScriptCallBack *p, int handle, int id);
void getSliderValue(SScriptCallBack *p, getSliderValue_out *out, int handle, int id);
void getSliderValue_callback(SScriptCallBack *p);

struct setSliderValue_in
{
    int handle;
    int id;
    int value;
    bool suppressEvents;
    setSliderValue_in();
};

struct setSliderValue_out
{
    setSliderValue_out();
};

void setSliderValue(SScriptCallBack *p, setSliderValue_in *in, setSliderValue_out *out);
void setSliderValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents = true);
void setSliderValue(SScriptCallBack *p, setSliderValue_out *out, int handle, int id, int value, bool suppressEvents = true);
void setSliderValue_callback(SScriptCallBack *p);

struct getEditValue_in
{
    int handle;
    int id;
    getEditValue_in();
};

struct getEditValue_out
{
    std::string value;
    getEditValue_out();
};

void getEditValue(SScriptCallBack *p, getEditValue_in *in, getEditValue_out *out);
std::string getEditValue(SScriptCallBack *p, int handle, int id);
void getEditValue(SScriptCallBack *p, getEditValue_out *out, int handle, int id);
void getEditValue_callback(SScriptCallBack *p);

struct setEditValue_in
{
    int handle;
    int id;
    std::string value;
    bool suppressEvents;
    setEditValue_in();
};

struct setEditValue_out
{
    setEditValue_out();
};

void setEditValue(SScriptCallBack *p, setEditValue_in *in, setEditValue_out *out);
void setEditValue(SScriptCallBack *p, int handle, int id, std::string value, bool suppressEvents = true);
void setEditValue(SScriptCallBack *p, setEditValue_out *out, int handle, int id, std::string value, bool suppressEvents = true);
void setEditValue_callback(SScriptCallBack *p);

struct getSpinboxValue_in
{
    int handle;
    int id;
    getSpinboxValue_in();
};

struct getSpinboxValue_out
{
    float value;
    getSpinboxValue_out();
};

void getSpinboxValue(SScriptCallBack *p, getSpinboxValue_in *in, getSpinboxValue_out *out);
float getSpinboxValue(SScriptCallBack *p, int handle, int id);
void getSpinboxValue(SScriptCallBack *p, getSpinboxValue_out *out, int handle, int id);
void getSpinboxValue_callback(SScriptCallBack *p);

struct setSpinboxValue_in
{
    int handle;
    int id;
    float value;
    bool suppressEvents;
    setSpinboxValue_in();
};

struct setSpinboxValue_out
{
    setSpinboxValue_out();
};

void setSpinboxValue(SScriptCallBack *p, setSpinboxValue_in *in, setSpinboxValue_out *out);
void setSpinboxValue(SScriptCallBack *p, int handle, int id, float value, bool suppressEvents = true);
void setSpinboxValue(SScriptCallBack *p, setSpinboxValue_out *out, int handle, int id, float value, bool suppressEvents = true);
void setSpinboxValue_callback(SScriptCallBack *p);

struct getCheckboxValue_in
{
    int handle;
    int id;
    getCheckboxValue_in();
};

struct getCheckboxValue_out
{
    int value;
    getCheckboxValue_out();
};

void getCheckboxValue(SScriptCallBack *p, getCheckboxValue_in *in, getCheckboxValue_out *out);
int getCheckboxValue(SScriptCallBack *p, int handle, int id);
void getCheckboxValue(SScriptCallBack *p, getCheckboxValue_out *out, int handle, int id);
void getCheckboxValue_callback(SScriptCallBack *p);

struct setCheckboxValue_in
{
    int handle;
    int id;
    int value;
    bool suppressEvents;
    setCheckboxValue_in();
};

struct setCheckboxValue_out
{
    setCheckboxValue_out();
};

void setCheckboxValue(SScriptCallBack *p, setCheckboxValue_in *in, setCheckboxValue_out *out);
void setCheckboxValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents = true);
void setCheckboxValue(SScriptCallBack *p, setCheckboxValue_out *out, int handle, int id, int value, bool suppressEvents = true);
void setCheckboxValue_callback(SScriptCallBack *p);

struct getRadiobuttonValue_in
{
    int handle;
    int id;
    getRadiobuttonValue_in();
};

struct getRadiobuttonValue_out
{
    int value;
    getRadiobuttonValue_out();
};

void getRadiobuttonValue(SScriptCallBack *p, getRadiobuttonValue_in *in, getRadiobuttonValue_out *out);
int getRadiobuttonValue(SScriptCallBack *p, int handle, int id);
void getRadiobuttonValue(SScriptCallBack *p, getRadiobuttonValue_out *out, int handle, int id);
void getRadiobuttonValue_callback(SScriptCallBack *p);

struct setRadiobuttonValue_in
{
    int handle;
    int id;
    int value;
    bool suppressEvents;
    setRadiobuttonValue_in();
};

struct setRadiobuttonValue_out
{
    setRadiobuttonValue_out();
};

void setRadiobuttonValue(SScriptCallBack *p, setRadiobuttonValue_in *in, setRadiobuttonValue_out *out);
void setRadiobuttonValue(SScriptCallBack *p, int handle, int id, int value, bool suppressEvents = true);
void setRadiobuttonValue(SScriptCallBack *p, setRadiobuttonValue_out *out, int handle, int id, int value, bool suppressEvents = true);
void setRadiobuttonValue_callback(SScriptCallBack *p);

struct getLabelText_in
{
    int handle;
    int id;
    getLabelText_in();
};

struct getLabelText_out
{
    std::string text;
    getLabelText_out();
};

void getLabelText(SScriptCallBack *p, getLabelText_in *in, getLabelText_out *out);
std::string getLabelText(SScriptCallBack *p, int handle, int id);
void getLabelText(SScriptCallBack *p, getLabelText_out *out, int handle, int id);
void getLabelText_callback(SScriptCallBack *p);

struct setLabelText_in
{
    int handle;
    int id;
    std::string text;
    bool suppressEvents;
    setLabelText_in();
};

struct setLabelText_out
{
    setLabelText_out();
};

void setLabelText(SScriptCallBack *p, setLabelText_in *in, setLabelText_out *out);
void setLabelText(SScriptCallBack *p, int handle, int id, std::string text, bool suppressEvents = true);
void setLabelText(SScriptCallBack *p, setLabelText_out *out, int handle, int id, std::string text, bool suppressEvents = true);
void setLabelText_callback(SScriptCallBack *p);

struct insertComboboxItem_in
{
    int handle;
    int id;
    int index;
    std::string text;
    bool suppressEvents;
    insertComboboxItem_in();
};

struct insertComboboxItem_out
{
    insertComboboxItem_out();
};

void insertComboboxItem(SScriptCallBack *p, insertComboboxItem_in *in, insertComboboxItem_out *out);
void insertComboboxItem(SScriptCallBack *p, int handle, int id, int index, std::string text, bool suppressEvents = true);
void insertComboboxItem(SScriptCallBack *p, insertComboboxItem_out *out, int handle, int id, int index, std::string text, bool suppressEvents = true);
void insertComboboxItem_callback(SScriptCallBack *p);

struct removeComboboxItem_in
{
    int handle;
    int id;
    int index;
    bool suppressEvents;
    removeComboboxItem_in();
};

struct removeComboboxItem_out
{
    removeComboboxItem_out();
};

void removeComboboxItem(SScriptCallBack *p, removeComboboxItem_in *in, removeComboboxItem_out *out);
void removeComboboxItem(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents = true);
void removeComboboxItem(SScriptCallBack *p, removeComboboxItem_out *out, int handle, int id, int index, bool suppressEvents = true);
void removeComboboxItem_callback(SScriptCallBack *p);

struct getComboboxItemCount_in
{
    int handle;
    int id;
    getComboboxItemCount_in();
};

struct getComboboxItemCount_out
{
    int count;
    getComboboxItemCount_out();
};

void getComboboxItemCount(SScriptCallBack *p, getComboboxItemCount_in *in, getComboboxItemCount_out *out);
int getComboboxItemCount(SScriptCallBack *p, int handle, int id);
void getComboboxItemCount(SScriptCallBack *p, getComboboxItemCount_out *out, int handle, int id);
void getComboboxItemCount_callback(SScriptCallBack *p);

struct getComboboxItemText_in
{
    int handle;
    int id;
    int index;
    getComboboxItemText_in();
};

struct getComboboxItemText_out
{
    std::string text;
    getComboboxItemText_out();
};

void getComboboxItemText(SScriptCallBack *p, getComboboxItemText_in *in, getComboboxItemText_out *out);
std::string getComboboxItemText(SScriptCallBack *p, int handle, int id, int index);
void getComboboxItemText(SScriptCallBack *p, getComboboxItemText_out *out, int handle, int id, int index);
void getComboboxItemText_callback(SScriptCallBack *p);

struct getComboboxItems_in
{
    int handle;
    int id;
    getComboboxItems_in();
};

struct getComboboxItems_out
{
    std::vector<std::string> items;
    getComboboxItems_out();
};

void getComboboxItems(SScriptCallBack *p, getComboboxItems_in *in, getComboboxItems_out *out);
std::vector<std::string> getComboboxItems(SScriptCallBack *p, int handle, int id);
void getComboboxItems(SScriptCallBack *p, getComboboxItems_out *out, int handle, int id);
void getComboboxItems_callback(SScriptCallBack *p);

struct setComboboxItems_in
{
    int handle;
    int id;
    std::vector<std::string> items;
    int index;
    bool suppressEvents;
    setComboboxItems_in();
};

struct setComboboxItems_out
{
    setComboboxItems_out();
};

void setComboboxItems(SScriptCallBack *p, setComboboxItems_in *in, setComboboxItems_out *out);
void setComboboxItems(SScriptCallBack *p, int handle, int id, std::vector<std::string> items, int index, bool suppressEvents = true);
void setComboboxItems(SScriptCallBack *p, setComboboxItems_out *out, int handle, int id, std::vector<std::string> items, int index, bool suppressEvents = true);
void setComboboxItems_callback(SScriptCallBack *p);

struct setComboboxSelectedIndex_in
{
    int handle;
    int id;
    int index;
    bool suppressEvents;
    setComboboxSelectedIndex_in();
};

struct setComboboxSelectedIndex_out
{
    setComboboxSelectedIndex_out();
};

void setComboboxSelectedIndex(SScriptCallBack *p, setComboboxSelectedIndex_in *in, setComboboxSelectedIndex_out *out);
void setComboboxSelectedIndex(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents = true);
void setComboboxSelectedIndex(SScriptCallBack *p, setComboboxSelectedIndex_out *out, int handle, int id, int index, bool suppressEvents = true);
void setComboboxSelectedIndex_callback(SScriptCallBack *p);

struct hide_in
{
    int handle;
    hide_in();
};

struct hide_out
{
    hide_out();
};

void hide(SScriptCallBack *p, hide_in *in, hide_out *out);
void hide(SScriptCallBack *p, int handle);
void hide(SScriptCallBack *p, hide_out *out, int handle);
void hide_callback(SScriptCallBack *p);

struct show_in
{
    int handle;
    show_in();
};

struct show_out
{
    show_out();
};

void show(SScriptCallBack *p, show_in *in, show_out *out);
void show(SScriptCallBack *p, int handle);
void show(SScriptCallBack *p, show_out *out, int handle);
void show_callback(SScriptCallBack *p);

struct isVisible_in
{
    int handle;
    isVisible_in();
};

struct isVisible_out
{
    bool visibility;
    isVisible_out();
};

void isVisible(SScriptCallBack *p, isVisible_in *in, isVisible_out *out);
bool isVisible(SScriptCallBack *p, int handle);
void isVisible(SScriptCallBack *p, isVisible_out *out, int handle);
void isVisible_callback(SScriptCallBack *p);

struct getPosition_in
{
    int handle;
    getPosition_in();
};

struct getPosition_out
{
    int x;
    int y;
    getPosition_out();
};

void getPosition(SScriptCallBack *p, getPosition_in *in, getPosition_out *out);
void getPosition(SScriptCallBack *p, getPosition_out *out, int handle);
void getPosition_callback(SScriptCallBack *p);

struct setPosition_in
{
    int handle;
    int x;
    int y;
    bool suppressEvents;
    setPosition_in();
};

struct setPosition_out
{
    setPosition_out();
};

void setPosition(SScriptCallBack *p, setPosition_in *in, setPosition_out *out);
void setPosition(SScriptCallBack *p, int handle, int x, int y, bool suppressEvents = true);
void setPosition(SScriptCallBack *p, setPosition_out *out, int handle, int x, int y, bool suppressEvents = true);
void setPosition_callback(SScriptCallBack *p);

struct getSize_in
{
    int handle;
    getSize_in();
};

struct getSize_out
{
    int w;
    int h;
    getSize_out();
};

void getSize(SScriptCallBack *p, getSize_in *in, getSize_out *out);
void getSize(SScriptCallBack *p, getSize_out *out, int handle);
void getSize_callback(SScriptCallBack *p);

struct setSize_in
{
    int handle;
    int w;
    int h;
    bool suppressEvents;
    setSize_in();
};

struct setSize_out
{
    setSize_out();
};

void setSize(SScriptCallBack *p, setSize_in *in, setSize_out *out);
void setSize(SScriptCallBack *p, int handle, int w, int h, bool suppressEvents = true);
void setSize(SScriptCallBack *p, setSize_out *out, int handle, int w, int h, bool suppressEvents = true);
void setSize_callback(SScriptCallBack *p);

struct getTitle_in
{
    int handle;
    getTitle_in();
};

struct getTitle_out
{
    std::string title;
    getTitle_out();
};

void getTitle(SScriptCallBack *p, getTitle_in *in, getTitle_out *out);
std::string getTitle(SScriptCallBack *p, int handle);
void getTitle(SScriptCallBack *p, getTitle_out *out, int handle);
void getTitle_callback(SScriptCallBack *p);

struct setTitle_in
{
    int handle;
    std::string title;
    bool suppressEvents;
    setTitle_in();
};

struct setTitle_out
{
    setTitle_out();
};

void setTitle(SScriptCallBack *p, setTitle_in *in, setTitle_out *out);
void setTitle(SScriptCallBack *p, int handle, std::string title, bool suppressEvents = true);
void setTitle(SScriptCallBack *p, setTitle_out *out, int handle, std::string title, bool suppressEvents = true);
void setTitle_callback(SScriptCallBack *p);

struct setImageData_in
{
    int handle;
    int id;
    std::string data;
    int width;
    int height;
    setImageData_in();
};

struct setImageData_out
{
    setImageData_out();
};

void setImageData(SScriptCallBack *p, setImageData_in *in, setImageData_out *out);
void setImageData(SScriptCallBack *p, int handle, int id, std::string data, int width, int height);
void setImageData(SScriptCallBack *p, setImageData_out *out, int handle, int id, std::string data, int width, int height);
void setImageData_callback(SScriptCallBack *p);

struct setEnabled_in
{
    int handle;
    int id;
    bool enabled;
    bool suppressEvents;
    setEnabled_in();
};

struct setEnabled_out
{
    setEnabled_out();
};

void setEnabled(SScriptCallBack *p, setEnabled_in *in, setEnabled_out *out);
void setEnabled(SScriptCallBack *p, int handle, int id, bool enabled, bool suppressEvents = true);
void setEnabled(SScriptCallBack *p, setEnabled_out *out, int handle, int id, bool enabled, bool suppressEvents = true);
void setEnabled_callback(SScriptCallBack *p);

struct getCurrentTab_in
{
    int handle;
    int id;
    getCurrentTab_in();
};

struct getCurrentTab_out
{
    int index;
    getCurrentTab_out();
};

void getCurrentTab(SScriptCallBack *p, getCurrentTab_in *in, getCurrentTab_out *out);
int getCurrentTab(SScriptCallBack *p, int handle, int id);
void getCurrentTab(SScriptCallBack *p, getCurrentTab_out *out, int handle, int id);
void getCurrentTab_callback(SScriptCallBack *p);

struct setCurrentTab_in
{
    int handle;
    int id;
    int index;
    bool suppressEvents;
    setCurrentTab_in();
};

struct setCurrentTab_out
{
    setCurrentTab_out();
};

void setCurrentTab(SScriptCallBack *p, setCurrentTab_in *in, setCurrentTab_out *out);
void setCurrentTab(SScriptCallBack *p, int handle, int id, int index, bool suppressEvents = true);
void setCurrentTab(SScriptCallBack *p, setCurrentTab_out *out, int handle, int id, int index, bool suppressEvents = true);
void setCurrentTab_callback(SScriptCallBack *p);

struct getWidgetVisibility_in
{
    int handle;
    int id;
    getWidgetVisibility_in();
};

struct getWidgetVisibility_out
{
    bool visibility;
    getWidgetVisibility_out();
};

void getWidgetVisibility(SScriptCallBack *p, getWidgetVisibility_in *in, getWidgetVisibility_out *out);
bool getWidgetVisibility(SScriptCallBack *p, int handle, int id);
void getWidgetVisibility(SScriptCallBack *p, getWidgetVisibility_out *out, int handle, int id);
void getWidgetVisibility_callback(SScriptCallBack *p);

struct setWidgetVisibility_in
{
    int handle;
    int id;
    bool visibility;
    setWidgetVisibility_in();
};

struct setWidgetVisibility_out
{
    setWidgetVisibility_out();
};

void setWidgetVisibility(SScriptCallBack *p, setWidgetVisibility_in *in, setWidgetVisibility_out *out);
void setWidgetVisibility(SScriptCallBack *p, int handle, int id, bool visibility);
void setWidgetVisibility(SScriptCallBack *p, setWidgetVisibility_out *out, int handle, int id, bool visibility);
void setWidgetVisibility_callback(SScriptCallBack *p);

struct getCurrentEditWidget_in
{
    int handle;
    getCurrentEditWidget_in();
};

struct getCurrentEditWidget_out
{
    int id;
    getCurrentEditWidget_out();
};

void getCurrentEditWidget(SScriptCallBack *p, getCurrentEditWidget_in *in, getCurrentEditWidget_out *out);
int getCurrentEditWidget(SScriptCallBack *p, int handle);
void getCurrentEditWidget(SScriptCallBack *p, getCurrentEditWidget_out *out, int handle);
void getCurrentEditWidget_callback(SScriptCallBack *p);

struct setCurrentEditWidget_in
{
    int handle;
    int id;
    setCurrentEditWidget_in();
};

struct setCurrentEditWidget_out
{
    setCurrentEditWidget_out();
};

void setCurrentEditWidget(SScriptCallBack *p, setCurrentEditWidget_in *in, setCurrentEditWidget_out *out);
void setCurrentEditWidget(SScriptCallBack *p, int handle, int id);
void setCurrentEditWidget(SScriptCallBack *p, setCurrentEditWidget_out *out, int handle, int id);
void setCurrentEditWidget_callback(SScriptCallBack *p);

struct replot_in
{
    int handle;
    int id;
    replot_in();
};

struct replot_out
{
    replot_out();
};

void replot(SScriptCallBack *p, replot_in *in, replot_out *out);
void replot(SScriptCallBack *p, int handle, int id);
void replot(SScriptCallBack *p, replot_out *out, int handle, int id);
void replot_callback(SScriptCallBack *p);

struct addCurve_in
{
    int handle;
    int id;
    int type;
    std::string name;
    std::vector<int> color;
    int style;
    curve_options options;
    addCurve_in();
};

struct addCurve_out
{
    addCurve_out();
};

void addCurve(SScriptCallBack *p, addCurve_in *in, addCurve_out *out);
void addCurve(SScriptCallBack *p, int handle, int id, int type, std::string name, std::vector<int> color, int style, curve_options options);
void addCurve(SScriptCallBack *p, addCurve_out *out, int handle, int id, int type, std::string name, std::vector<int> color, int style, curve_options options);
void addCurve_callback(SScriptCallBack *p);

struct addCurveTimePoints_in
{
    int handle;
    int id;
    std::string name;
    std::vector<double> x;
    std::vector<double> y;
    addCurveTimePoints_in();
};

struct addCurveTimePoints_out
{
    addCurveTimePoints_out();
};

void addCurveTimePoints(SScriptCallBack *p, addCurveTimePoints_in *in, addCurveTimePoints_out *out);
void addCurveTimePoints(SScriptCallBack *p, int handle, int id, std::string name, std::vector<double> x, std::vector<double> y);
void addCurveTimePoints(SScriptCallBack *p, addCurveTimePoints_out *out, int handle, int id, std::string name, std::vector<double> x, std::vector<double> y);
void addCurveTimePoints_callback(SScriptCallBack *p);

struct addCurveXYPoints_in
{
    int handle;
    int id;
    std::string name;
    std::vector<double> t;
    std::vector<double> x;
    std::vector<double> y;
    addCurveXYPoints_in();
};

struct addCurveXYPoints_out
{
    addCurveXYPoints_out();
};

void addCurveXYPoints(SScriptCallBack *p, addCurveXYPoints_in *in, addCurveXYPoints_out *out);
void addCurveXYPoints(SScriptCallBack *p, int handle, int id, std::string name, std::vector<double> t, std::vector<double> x, std::vector<double> y);
void addCurveXYPoints(SScriptCallBack *p, addCurveXYPoints_out *out, int handle, int id, std::string name, std::vector<double> t, std::vector<double> x, std::vector<double> y);
void addCurveXYPoints_callback(SScriptCallBack *p);

struct clearCurve_in
{
    int handle;
    int id;
    std::string name;
    clearCurve_in();
};

struct clearCurve_out
{
    clearCurve_out();
};

void clearCurve(SScriptCallBack *p, clearCurve_in *in, clearCurve_out *out);
void clearCurve(SScriptCallBack *p, int handle, int id, std::string name);
void clearCurve(SScriptCallBack *p, clearCurve_out *out, int handle, int id, std::string name);
void clearCurve_callback(SScriptCallBack *p);

struct removeCurve_in
{
    int handle;
    int id;
    std::string name;
    removeCurve_in();
};

struct removeCurve_out
{
    removeCurve_out();
};

void removeCurve(SScriptCallBack *p, removeCurve_in *in, removeCurve_out *out);
void removeCurve(SScriptCallBack *p, int handle, int id, std::string name);
void removeCurve(SScriptCallBack *p, removeCurve_out *out, int handle, int id, std::string name);
void removeCurve_callback(SScriptCallBack *p);

struct setPlotRanges_in
{
    int handle;
    int id;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    setPlotRanges_in();
};

struct setPlotRanges_out
{
    setPlotRanges_out();
};

void setPlotRanges(SScriptCallBack *p, setPlotRanges_in *in, setPlotRanges_out *out);
void setPlotRanges(SScriptCallBack *p, int handle, int id, double xmin, double xmax, double ymin, double ymax);
void setPlotRanges(SScriptCallBack *p, setPlotRanges_out *out, int handle, int id, double xmin, double xmax, double ymin, double ymax);
void setPlotRanges_callback(SScriptCallBack *p);

struct setPlotXRange_in
{
    int handle;
    int id;
    double xmin;
    double xmax;
    setPlotXRange_in();
};

struct setPlotXRange_out
{
    setPlotXRange_out();
};

void setPlotXRange(SScriptCallBack *p, setPlotXRange_in *in, setPlotXRange_out *out);
void setPlotXRange(SScriptCallBack *p, int handle, int id, double xmin, double xmax);
void setPlotXRange(SScriptCallBack *p, setPlotXRange_out *out, int handle, int id, double xmin, double xmax);
void setPlotXRange_callback(SScriptCallBack *p);

struct setPlotYRange_in
{
    int handle;
    int id;
    double ymin;
    double ymax;
    setPlotYRange_in();
};

struct setPlotYRange_out
{
    setPlotYRange_out();
};

void setPlotYRange(SScriptCallBack *p, setPlotYRange_in *in, setPlotYRange_out *out);
void setPlotYRange(SScriptCallBack *p, int handle, int id, double ymin, double ymax);
void setPlotYRange(SScriptCallBack *p, setPlotYRange_out *out, int handle, int id, double ymin, double ymax);
void setPlotYRange_callback(SScriptCallBack *p);

struct growPlotRanges_in
{
    int handle;
    int id;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    growPlotRanges_in();
};

struct growPlotRanges_out
{
    growPlotRanges_out();
};

void growPlotRanges(SScriptCallBack *p, growPlotRanges_in *in, growPlotRanges_out *out);
void growPlotRanges(SScriptCallBack *p, int handle, int id, double xmin, double xmax, double ymin, double ymax);
void growPlotRanges(SScriptCallBack *p, growPlotRanges_out *out, int handle, int id, double xmin, double xmax, double ymin, double ymax);
void growPlotRanges_callback(SScriptCallBack *p);

struct growPlotXRange_in
{
    int handle;
    int id;
    double xmin;
    double xmax;
    growPlotXRange_in();
};

struct growPlotXRange_out
{
    growPlotXRange_out();
};

void growPlotXRange(SScriptCallBack *p, growPlotXRange_in *in, growPlotXRange_out *out);
void growPlotXRange(SScriptCallBack *p, int handle, int id, double xmin, double xmax);
void growPlotXRange(SScriptCallBack *p, growPlotXRange_out *out, int handle, int id, double xmin, double xmax);
void growPlotXRange_callback(SScriptCallBack *p);

struct growPlotYRange_in
{
    int handle;
    int id;
    double ymin;
    double ymax;
    growPlotYRange_in();
};

struct growPlotYRange_out
{
    growPlotYRange_out();
};

void growPlotYRange(SScriptCallBack *p, growPlotYRange_in *in, growPlotYRange_out *out);
void growPlotYRange(SScriptCallBack *p, int handle, int id, double ymin, double ymax);
void growPlotYRange(SScriptCallBack *p, growPlotYRange_out *out, int handle, int id, double ymin, double ymax);
void growPlotYRange_callback(SScriptCallBack *p);

struct setPlotLabels_in
{
    int handle;
    int id;
    std::string x;
    std::string y;
    setPlotLabels_in();
};

struct setPlotLabels_out
{
    setPlotLabels_out();
};

void setPlotLabels(SScriptCallBack *p, setPlotLabels_in *in, setPlotLabels_out *out);
void setPlotLabels(SScriptCallBack *p, int handle, int id, std::string x, std::string y);
void setPlotLabels(SScriptCallBack *p, setPlotLabels_out *out, int handle, int id, std::string x, std::string y);
void setPlotLabels_callback(SScriptCallBack *p);

struct setPlotXLabel_in
{
    int handle;
    int id;
    std::string label;
    setPlotXLabel_in();
};

struct setPlotXLabel_out
{
    setPlotXLabel_out();
};

void setPlotXLabel(SScriptCallBack *p, setPlotXLabel_in *in, setPlotXLabel_out *out);
void setPlotXLabel(SScriptCallBack *p, int handle, int id, std::string label);
void setPlotXLabel(SScriptCallBack *p, setPlotXLabel_out *out, int handle, int id, std::string label);
void setPlotXLabel_callback(SScriptCallBack *p);

struct setPlotYLabel_in
{
    int handle;
    int id;
    std::string label;
    setPlotYLabel_in();
};

struct setPlotYLabel_out
{
    setPlotYLabel_out();
};

void setPlotYLabel(SScriptCallBack *p, setPlotYLabel_in *in, setPlotYLabel_out *out);
void setPlotYLabel(SScriptCallBack *p, int handle, int id, std::string label);
void setPlotYLabel(SScriptCallBack *p, setPlotYLabel_out *out, int handle, int id, std::string label);
void setPlotYLabel_callback(SScriptCallBack *p);

struct rescaleAxes_in
{
    int handle;
    int id;
    std::string name;
    bool onlyEnlargeX;
    bool onlyEnlargeY;
    rescaleAxes_in();
};

struct rescaleAxes_out
{
    rescaleAxes_out();
};

void rescaleAxes(SScriptCallBack *p, rescaleAxes_in *in, rescaleAxes_out *out);
void rescaleAxes(SScriptCallBack *p, int handle, int id, std::string name, bool onlyEnlargeX = false, bool onlyEnlargeY = false);
void rescaleAxes(SScriptCallBack *p, rescaleAxes_out *out, int handle, int id, std::string name, bool onlyEnlargeX = false, bool onlyEnlargeY = false);
void rescaleAxes_callback(SScriptCallBack *p);

struct rescaleAxesAll_in
{
    int handle;
    int id;
    bool onlyEnlargeX;
    bool onlyEnlargeY;
    rescaleAxesAll_in();
};

struct rescaleAxesAll_out
{
    rescaleAxesAll_out();
};

void rescaleAxesAll(SScriptCallBack *p, rescaleAxesAll_in *in, rescaleAxesAll_out *out);
void rescaleAxesAll(SScriptCallBack *p, int handle, int id, bool onlyEnlargeX = false, bool onlyEnlargeY = false);
void rescaleAxesAll(SScriptCallBack *p, rescaleAxesAll_out *out, int handle, int id, bool onlyEnlargeX = false, bool onlyEnlargeY = false);
void rescaleAxesAll_callback(SScriptCallBack *p);

struct setMouseOptions_in
{
    int handle;
    int id;
    bool panX;
    bool panY;
    bool zoomX;
    bool zoomY;
    setMouseOptions_in();
};

struct setMouseOptions_out
{
    setMouseOptions_out();
};

void setMouseOptions(SScriptCallBack *p, setMouseOptions_in *in, setMouseOptions_out *out);
void setMouseOptions(SScriptCallBack *p, int handle, int id, bool panX, bool panY, bool zoomX, bool zoomY);
void setMouseOptions(SScriptCallBack *p, setMouseOptions_out *out, int handle, int id, bool panX, bool panY, bool zoomX, bool zoomY);
void setMouseOptions_callback(SScriptCallBack *p);

struct setLegendVisibility_in
{
    int handle;
    int id;
    bool visible;
    setLegendVisibility_in();
};

struct setLegendVisibility_out
{
    setLegendVisibility_out();
};

void setLegendVisibility(SScriptCallBack *p, setLegendVisibility_in *in, setLegendVisibility_out *out);
void setLegendVisibility(SScriptCallBack *p, int handle, int id, bool visible);
void setLegendVisibility(SScriptCallBack *p, setLegendVisibility_out *out, int handle, int id, bool visible);
void setLegendVisibility_callback(SScriptCallBack *p);

struct getCurveData_in
{
    int handle;
    int id;
    std::string name;
    getCurveData_in();
};

struct getCurveData_out
{
    std::vector<double> t;
    std::vector<double> x;
    std::vector<double> y;
    getCurveData_out();
};

void getCurveData(SScriptCallBack *p, getCurveData_in *in, getCurveData_out *out);
void getCurveData(SScriptCallBack *p, getCurveData_out *out, int handle, int id, std::string name);
void getCurveData_callback(SScriptCallBack *p);

struct onclickCallback_in
{
    int handle;
    int id;
    onclickCallback_in();
};

struct onclickCallback_out
{
    onclickCallback_out();
};

void onclickCallback(simInt scriptId, const char *func, int handle, int id);

bool onclickCallback(simInt scriptId, const char *func, onclickCallback_in *in_args, onclickCallback_out *out_args);

struct onchangeIntCallback_in
{
    int handle;
    int id;
    int value;
    onchangeIntCallback_in();
};

struct onchangeIntCallback_out
{
    onchangeIntCallback_out();
};

void onchangeIntCallback(simInt scriptId, const char *func, int handle, int id, int value);

bool onchangeIntCallback(simInt scriptId, const char *func, onchangeIntCallback_in *in_args, onchangeIntCallback_out *out_args);

struct onchangeDoubleCallback_in
{
    int handle;
    int id;
    float value;
    onchangeDoubleCallback_in();
};

struct onchangeDoubleCallback_out
{
    onchangeDoubleCallback_out();
};

void onchangeDoubleCallback(simInt scriptId, const char *func, int handle, int id, float value);

bool onchangeDoubleCallback(simInt scriptId, const char *func, onchangeDoubleCallback_in *in_args, onchangeDoubleCallback_out *out_args);

struct onchangeStringCallback_in
{
    int handle;
    int id;
    std::string value;
    onchangeStringCallback_in();
};

struct onchangeStringCallback_out
{
    onchangeStringCallback_out();
};

void onchangeStringCallback(simInt scriptId, const char *func, int handle, int id, std::string value);

bool onchangeStringCallback(simInt scriptId, const char *func, onchangeStringCallback_in *in_args, onchangeStringCallback_out *out_args);

struct oneditingfinishedCallback_in
{
    int handle;
    int id;
    std::string value;
    oneditingfinishedCallback_in();
};

struct oneditingfinishedCallback_out
{
    oneditingfinishedCallback_out();
};

void oneditingfinishedCallback(simInt scriptId, const char *func, int handle, int id, std::string value);

bool oneditingfinishedCallback(simInt scriptId, const char *func, oneditingfinishedCallback_in *in_args, oneditingfinishedCallback_out *out_args);

struct oncloseCallback_in
{
    int handle;
    oncloseCallback_in();
};

struct oncloseCallback_out
{
    oncloseCallback_out();
};

void oncloseCallback(simInt scriptId, const char *func, int handle);

bool oncloseCallback(simInt scriptId, const char *func, oncloseCallback_in *in_args, oncloseCallback_out *out_args);

struct onPlottableClickCallback_in
{
    int handle;
    int id;
    std::string curve;
    int index;
    float x;
    float y;
    onPlottableClickCallback_in();
};

struct onPlottableClickCallback_out
{
    onPlottableClickCallback_out();
};

void onPlottableClickCallback(simInt scriptId, const char *func, int handle, int id, std::string curve, int index, float x, float y);

bool onPlottableClickCallback(simInt scriptId, const char *func, onPlottableClickCallback_in *in_args, onPlottableClickCallback_out *out_args);

struct onLegendClickCallback_in
{
    int handle;
    int id;
    std::string curve;
    onLegendClickCallback_in();
};

struct onLegendClickCallback_out
{
    onLegendClickCallback_out();
};

void onLegendClickCallback(simInt scriptId, const char *func, int handle, int id, std::string curve);

bool onLegendClickCallback(simInt scriptId, const char *func, onLegendClickCallback_in *in_args, onLegendClickCallback_out *out_args);


// following functions must be implemented in the plugin

void create(SScriptCallBack *p, const char *cmd, create_in *in, create_out *out);
void destroy(SScriptCallBack *p, const char *cmd, destroy_in *in, destroy_out *out);
void getSliderValue(SScriptCallBack *p, const char *cmd, getSliderValue_in *in, getSliderValue_out *out);
void setSliderValue(SScriptCallBack *p, const char *cmd, setSliderValue_in *in, setSliderValue_out *out);
void getEditValue(SScriptCallBack *p, const char *cmd, getEditValue_in *in, getEditValue_out *out);
void setEditValue(SScriptCallBack *p, const char *cmd, setEditValue_in *in, setEditValue_out *out);
void getSpinboxValue(SScriptCallBack *p, const char *cmd, getSpinboxValue_in *in, getSpinboxValue_out *out);
void setSpinboxValue(SScriptCallBack *p, const char *cmd, setSpinboxValue_in *in, setSpinboxValue_out *out);
void getCheckboxValue(SScriptCallBack *p, const char *cmd, getCheckboxValue_in *in, getCheckboxValue_out *out);
void setCheckboxValue(SScriptCallBack *p, const char *cmd, setCheckboxValue_in *in, setCheckboxValue_out *out);
void getRadiobuttonValue(SScriptCallBack *p, const char *cmd, getRadiobuttonValue_in *in, getRadiobuttonValue_out *out);
void setRadiobuttonValue(SScriptCallBack *p, const char *cmd, setRadiobuttonValue_in *in, setRadiobuttonValue_out *out);
void getLabelText(SScriptCallBack *p, const char *cmd, getLabelText_in *in, getLabelText_out *out);
void setLabelText(SScriptCallBack *p, const char *cmd, setLabelText_in *in, setLabelText_out *out);
void insertComboboxItem(SScriptCallBack *p, const char *cmd, insertComboboxItem_in *in, insertComboboxItem_out *out);
void removeComboboxItem(SScriptCallBack *p, const char *cmd, removeComboboxItem_in *in, removeComboboxItem_out *out);
void getComboboxItemCount(SScriptCallBack *p, const char *cmd, getComboboxItemCount_in *in, getComboboxItemCount_out *out);
void getComboboxItemText(SScriptCallBack *p, const char *cmd, getComboboxItemText_in *in, getComboboxItemText_out *out);
void getComboboxItems(SScriptCallBack *p, const char *cmd, getComboboxItems_in *in, getComboboxItems_out *out);
void setComboboxItems(SScriptCallBack *p, const char *cmd, setComboboxItems_in *in, setComboboxItems_out *out);
void setComboboxSelectedIndex(SScriptCallBack *p, const char *cmd, setComboboxSelectedIndex_in *in, setComboboxSelectedIndex_out *out);
void hide(SScriptCallBack *p, const char *cmd, hide_in *in, hide_out *out);
void show(SScriptCallBack *p, const char *cmd, show_in *in, show_out *out);
void isVisible(SScriptCallBack *p, const char *cmd, isVisible_in *in, isVisible_out *out);
void getPosition(SScriptCallBack *p, const char *cmd, getPosition_in *in, getPosition_out *out);
void setPosition(SScriptCallBack *p, const char *cmd, setPosition_in *in, setPosition_out *out);
void getSize(SScriptCallBack *p, const char *cmd, getSize_in *in, getSize_out *out);
void setSize(SScriptCallBack *p, const char *cmd, setSize_in *in, setSize_out *out);
void getTitle(SScriptCallBack *p, const char *cmd, getTitle_in *in, getTitle_out *out);
void setTitle(SScriptCallBack *p, const char *cmd, setTitle_in *in, setTitle_out *out);
void setImageData(SScriptCallBack *p, const char *cmd, setImageData_in *in, setImageData_out *out);
void setEnabled(SScriptCallBack *p, const char *cmd, setEnabled_in *in, setEnabled_out *out);
void getCurrentTab(SScriptCallBack *p, const char *cmd, getCurrentTab_in *in, getCurrentTab_out *out);
void setCurrentTab(SScriptCallBack *p, const char *cmd, setCurrentTab_in *in, setCurrentTab_out *out);
void getWidgetVisibility(SScriptCallBack *p, const char *cmd, getWidgetVisibility_in *in, getWidgetVisibility_out *out);
void setWidgetVisibility(SScriptCallBack *p, const char *cmd, setWidgetVisibility_in *in, setWidgetVisibility_out *out);
void getCurrentEditWidget(SScriptCallBack *p, const char *cmd, getCurrentEditWidget_in *in, getCurrentEditWidget_out *out);
void setCurrentEditWidget(SScriptCallBack *p, const char *cmd, setCurrentEditWidget_in *in, setCurrentEditWidget_out *out);
void replot(SScriptCallBack *p, const char *cmd, replot_in *in, replot_out *out);
void addCurve(SScriptCallBack *p, const char *cmd, addCurve_in *in, addCurve_out *out);
void addCurveTimePoints(SScriptCallBack *p, const char *cmd, addCurveTimePoints_in *in, addCurveTimePoints_out *out);
void addCurveXYPoints(SScriptCallBack *p, const char *cmd, addCurveXYPoints_in *in, addCurveXYPoints_out *out);
void clearCurve(SScriptCallBack *p, const char *cmd, clearCurve_in *in, clearCurve_out *out);
void removeCurve(SScriptCallBack *p, const char *cmd, removeCurve_in *in, removeCurve_out *out);
void setPlotRanges(SScriptCallBack *p, const char *cmd, setPlotRanges_in *in, setPlotRanges_out *out);
void setPlotXRange(SScriptCallBack *p, const char *cmd, setPlotXRange_in *in, setPlotXRange_out *out);
void setPlotYRange(SScriptCallBack *p, const char *cmd, setPlotYRange_in *in, setPlotYRange_out *out);
void growPlotRanges(SScriptCallBack *p, const char *cmd, growPlotRanges_in *in, growPlotRanges_out *out);
void growPlotXRange(SScriptCallBack *p, const char *cmd, growPlotXRange_in *in, growPlotXRange_out *out);
void growPlotYRange(SScriptCallBack *p, const char *cmd, growPlotYRange_in *in, growPlotYRange_out *out);
void setPlotLabels(SScriptCallBack *p, const char *cmd, setPlotLabels_in *in, setPlotLabels_out *out);
void setPlotXLabel(SScriptCallBack *p, const char *cmd, setPlotXLabel_in *in, setPlotXLabel_out *out);
void setPlotYLabel(SScriptCallBack *p, const char *cmd, setPlotYLabel_in *in, setPlotYLabel_out *out);
void rescaleAxes(SScriptCallBack *p, const char *cmd, rescaleAxes_in *in, rescaleAxes_out *out);
void rescaleAxesAll(SScriptCallBack *p, const char *cmd, rescaleAxesAll_in *in, rescaleAxesAll_out *out);
void setMouseOptions(SScriptCallBack *p, const char *cmd, setMouseOptions_in *in, setMouseOptions_out *out);
void setLegendVisibility(SScriptCallBack *p, const char *cmd, setLegendVisibility_in *in, setLegendVisibility_out *out);
void getCurveData(SScriptCallBack *p, const char *cmd, getCurveData_in *in, getCurveData_out *out);

#endif // STUBS_H__INCLUDED
