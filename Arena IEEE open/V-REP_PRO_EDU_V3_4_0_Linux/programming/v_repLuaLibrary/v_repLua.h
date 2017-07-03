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

#include <cstring>

typedef void luaWrap_lua_State;
typedef void luaWrap_lua_Debug;
typedef double luaWrap_lua_Number;
typedef void (*luaWrap_lua_Hook) (luaWrap_lua_State* L,luaWrap_lua_Debug* ar);
typedef int (*luaWrap_lua_CFunction) (luaWrap_lua_State* L);

#ifdef _WIN32
    #define LUALIB_DLLEXPORT extern "C" __declspec(dllexport)
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
    #define LUALIB_DLLEXPORT extern "C"
#endif /* __linux || __APPLE__ */

LUALIB_DLLEXPORT int luaLibGet_LUA_MULTRET();
LUALIB_DLLEXPORT int luaLibGet_LUA_MASKCOUNT();
LUALIB_DLLEXPORT int luaLibGet_LUA_GLOBALSINDEX();
LUALIB_DLLEXPORT luaWrap_lua_State* luaLib_luaL_newstate();
LUALIB_DLLEXPORT void luaLib_lua_close(luaWrap_lua_State* L);
LUALIB_DLLEXPORT void luaLib_luaL_openlibs(luaWrap_lua_State* L);
LUALIB_DLLEXPORT int luaLib_lua_sethook(luaWrap_lua_State* L,luaWrap_lua_Hook func,int mask,int cnt);
LUALIB_DLLEXPORT void luaLib_lua_register(luaWrap_lua_State* L,const char* name,luaWrap_lua_CFunction func);
LUALIB_DLLEXPORT void luaLib_lua_pushnumber(luaWrap_lua_State* L,luaWrap_lua_Number n);
LUALIB_DLLEXPORT void luaLib_lua_pushnil(luaWrap_lua_State* L);
LUALIB_DLLEXPORT void luaLib_lua_pushboolean(luaWrap_lua_State* L,int b);
LUALIB_DLLEXPORT void luaLib_lua_pushinteger(luaWrap_lua_State* L,int n);
LUALIB_DLLEXPORT void luaLib_lua_pushstring(luaWrap_lua_State* L,const char* str);
LUALIB_DLLEXPORT void luaLib_lua_pushlstring(luaWrap_lua_State* L,const char* str,size_t l);
LUALIB_DLLEXPORT void luaLib_lua_pushcclosure(luaWrap_lua_State* L,luaWrap_lua_CFunction func,int n);
LUALIB_DLLEXPORT void luaLib_lua_pushvalue(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_tointeger(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT luaWrap_lua_Number luaLib_lua_tonumber(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_toboolean(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT const char* luaLib_lua_tostring(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT const char* luaLib_lua_tolstring(luaWrap_lua_State* L,int idx,size_t* len);
LUALIB_DLLEXPORT int luaLib_lua_isnumber(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_isstring(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT bool luaLib_lua_isnil(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT bool luaLib_lua_isboolean(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT bool luaLib_lua_istable(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT bool luaLib_lua_isfunction(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_getstack(luaWrap_lua_State* L,int level,luaWrap_lua_Debug* deb);
LUALIB_DLLEXPORT int luaLib_lua_getinfo(luaWrap_lua_State* L,const char* what,luaWrap_lua_Debug* deb);
LUALIB_DLLEXPORT void luaLib_lua_setglobal(luaWrap_lua_State* L,const char* name);
LUALIB_DLLEXPORT void luaLib_lua_getfield(luaWrap_lua_State* L,int idx,const char* name);
LUALIB_DLLEXPORT void luaLib_lua_setfield(luaWrap_lua_State* L,int idx,const char* name);
LUALIB_DLLEXPORT void luaLib_lua_yield(luaWrap_lua_State* L,int nresults);
LUALIB_DLLEXPORT int luaLib_luaL_dostring(luaWrap_lua_State* L,const char* str);
LUALIB_DLLEXPORT void luaLib_lua_getglobal(luaWrap_lua_State* L,const char* str);
LUALIB_DLLEXPORT void luaLib_lua_pop(luaWrap_lua_State* L,int n);
LUALIB_DLLEXPORT int luaLib_lua_gettop(luaWrap_lua_State* L);
LUALIB_DLLEXPORT void luaLib_lua_settop(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT size_t luaLib_lua_objlen(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT void luaLib_lua_rawgeti(luaWrap_lua_State* L,int idx,int n);
LUALIB_DLLEXPORT void luaLib_lua_rawseti(luaWrap_lua_State* L,int idx,int n);
LUALIB_DLLEXPORT void luaLib_lua_newtable(luaWrap_lua_State* L);
LUALIB_DLLEXPORT int luaLib_luaL_loadbuffer(luaWrap_lua_State* L,const char* buff,size_t sz,const char* name);
LUALIB_DLLEXPORT int luaLib_lua_pcall(luaWrap_lua_State* L,int nargs,int nresult,int errfunc);
LUALIB_DLLEXPORT void luaLib_lua_remove(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT void luaLib_lua_insert(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_isuserdata(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_upvalueindex(int i);
LUALIB_DLLEXPORT int luaLib_getCurrentCodeLine(luaWrap_lua_State* L);
LUALIB_DLLEXPORT void luaLib_lua_settable(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_next(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_type(luaWrap_lua_State* L,int idx);
LUALIB_DLLEXPORT int luaLib_lua_error(luaWrap_lua_State* L);
