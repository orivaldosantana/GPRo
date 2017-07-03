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

#include "v_repLua.h"

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}


LUALIB_DLLEXPORT int luaLibGet_LUA_MULTRET()
{
    return(LUA_MULTRET);
}

LUALIB_DLLEXPORT int luaLibGet_LUA_MASKCOUNT()
{
    return(LUA_MASKCOUNT);
}

LUALIB_DLLEXPORT int luaLibGet_LUA_GLOBALSINDEX()
{
    return(LUA_GLOBALSINDEX);
}

LUALIB_DLLEXPORT luaWrap_lua_State* luaLib_luaL_newstate()
{
    return((luaWrap_lua_State*)luaL_newstate());
}

LUALIB_DLLEXPORT void luaLib_lua_close(luaWrap_lua_State* L)
{
    lua_close((lua_State*)L);
}

LUALIB_DLLEXPORT void luaLib_luaL_openlibs(luaWrap_lua_State* L)
{
    luaL_openlibs((lua_State*)L);
}

LUALIB_DLLEXPORT int luaLib_lua_sethook(luaWrap_lua_State* L,luaWrap_lua_Hook func,int mask,int cnt)
{
    return(lua_sethook((lua_State*)L,(lua_Hook)func,mask,cnt));
}

LUALIB_DLLEXPORT void luaLib_lua_register(luaWrap_lua_State* L,const char* name,luaWrap_lua_CFunction func)
{
    lua_register((lua_State*)L,name,(lua_CFunction)func);
}

LUALIB_DLLEXPORT void luaLib_lua_pushnumber(luaWrap_lua_State* L,luaWrap_lua_Number n)
{
    lua_pushnumber((lua_State*)L,n);
}

LUALIB_DLLEXPORT void luaLib_lua_pushnil(luaWrap_lua_State* L)
{
    lua_pushnil((lua_State*)L);
}

LUALIB_DLLEXPORT void luaLib_lua_pushboolean(luaWrap_lua_State* L,int b)
{
    lua_pushboolean((lua_State*)L,b);
}

LUALIB_DLLEXPORT void luaLib_lua_pushinteger(luaWrap_lua_State* L,int n)
{
    lua_pushinteger((lua_State*)L,n);
}

LUALIB_DLLEXPORT void luaLib_lua_pushstring(luaWrap_lua_State* L,const char* str)
{
    lua_pushstring((lua_State*)L,str);
}

LUALIB_DLLEXPORT void luaLib_lua_pushlstring(luaWrap_lua_State* L,const char* str,size_t l)
{
    lua_pushlstring((lua_State*)L,str,l);
}

LUALIB_DLLEXPORT void luaLib_lua_pushcclosure(luaWrap_lua_State* L,luaWrap_lua_CFunction func,int n)
{
    lua_pushcclosure((lua_State*)L,(lua_CFunction)func,n);
}

LUALIB_DLLEXPORT void luaLib_lua_pushvalue(luaWrap_lua_State* L,int idx)
{
    lua_pushvalue((lua_State*)L,idx);
}

LUALIB_DLLEXPORT int luaLib_lua_tointeger(luaWrap_lua_State* L,int idx)
{
    return((int)lua_tointeger((lua_State*)L,idx));
}

LUALIB_DLLEXPORT luaWrap_lua_Number luaLib_lua_tonumber(luaWrap_lua_State* L,int idx)
{
    return((luaWrap_lua_Number)lua_tonumber((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_toboolean(luaWrap_lua_State* L,int idx)
{
    return(lua_toboolean((lua_State*)L,idx));
}

LUALIB_DLLEXPORT const char* luaLib_lua_tostring(luaWrap_lua_State* L,int idx)
{
    return(lua_tostring((lua_State*)L,idx));
}

LUALIB_DLLEXPORT const char* luaLib_lua_tolstring(luaWrap_lua_State* L,int idx,size_t* len)
{
    return(lua_tolstring((lua_State*)L,idx,len));
}

LUALIB_DLLEXPORT int luaLib_lua_isnumber(luaWrap_lua_State* L,int idx)
{
    return(lua_isnumber((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_isstring(luaWrap_lua_State* L,int idx)
{
    return(lua_isstring((lua_State*)L,idx));
}

LUALIB_DLLEXPORT bool luaLib_lua_isboolean(luaWrap_lua_State* L,int idx)
{
    return(lua_isboolean((lua_State*)L,idx));
}

LUALIB_DLLEXPORT bool luaLib_lua_isnil(luaWrap_lua_State* L,int idx)
{
    return(lua_isnil((lua_State*)L,idx));
}

LUALIB_DLLEXPORT bool luaLib_lua_istable(luaWrap_lua_State* L,int idx)
{
    return(lua_istable((lua_State*)L,idx));
}

LUALIB_DLLEXPORT bool luaLib_lua_isfunction(luaWrap_lua_State* L,int idx)
{
    return(lua_isfunction((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_getstack(luaWrap_lua_State* L,int level,luaWrap_lua_Debug* deb)
{
    return(lua_getstack((lua_State*)L,level,(lua_Debug*)deb));
}

LUALIB_DLLEXPORT int luaLib_lua_getinfo(luaWrap_lua_State* L,const char* what,luaWrap_lua_Debug* deb)
{
    return(lua_getinfo((lua_State*)L,what,(lua_Debug*)deb));
}

LUALIB_DLLEXPORT void luaLib_lua_setglobal(luaWrap_lua_State* L,const char* name)
{
    lua_setglobal((lua_State*)L,name);
}

LUALIB_DLLEXPORT void luaLib_lua_getfield(luaWrap_lua_State* L,int idx,const char* name)
{
    lua_getfield((lua_State*)L,idx,name);
}

LUALIB_DLLEXPORT void luaLib_lua_setfield(luaWrap_lua_State* L,int idx,const char* name)
{
    lua_setfield((lua_State*)L,idx,name);
}

LUALIB_DLLEXPORT void luaLib_lua_yield(luaWrap_lua_State* L,int nresults)
{
    lua_yield((lua_State*)L,nresults);
}

LUALIB_DLLEXPORT int luaLib_luaL_dostring(luaWrap_lua_State* L,const char* str)
{
    return(luaL_dostring((lua_State*)L,str));
}

LUALIB_DLLEXPORT void luaLib_lua_getglobal(luaWrap_lua_State* L,const char* str)
{
    lua_getglobal((lua_State*)L,str);
}

LUALIB_DLLEXPORT void luaLib_lua_pop(luaWrap_lua_State* L,int n)
{
    lua_pop((lua_State*)L,n);
}

LUALIB_DLLEXPORT int luaLib_lua_gettop(luaWrap_lua_State* L)
{
    return(lua_gettop((lua_State*)L));
}

LUALIB_DLLEXPORT void luaLib_lua_settop(luaWrap_lua_State* L,int idx)
{
    lua_settop((lua_State*)L,idx);
}

LUALIB_DLLEXPORT size_t luaLib_lua_objlen(luaWrap_lua_State* L,int idx)
{
    return(lua_objlen((lua_State*)L,idx));
}

LUALIB_DLLEXPORT void luaLib_lua_rawgeti(luaWrap_lua_State* L,int idx,int n)
{
    lua_rawgeti((lua_State*)L,idx,n);
}

LUALIB_DLLEXPORT void luaLib_lua_rawseti(luaWrap_lua_State* L,int idx,int n)
{
    lua_rawseti((lua_State*)L,idx,n);
}

LUALIB_DLLEXPORT void luaLib_lua_newtable(luaWrap_lua_State* L)
{
    lua_newtable((lua_State*)L);
}

LUALIB_DLLEXPORT int luaLib_luaL_loadbuffer(luaWrap_lua_State* L,const char* buff,size_t sz,const char* name)
{
    return(luaL_loadbuffer((lua_State*)L,buff,sz,name));
}

LUALIB_DLLEXPORT void luaLib_lua_remove(luaWrap_lua_State* L,int idx)
{
    lua_remove((lua_State*)L,idx);
}

LUALIB_DLLEXPORT void luaLib_lua_insert(luaWrap_lua_State* L,int idx)
{
    lua_insert((lua_State*)L,idx);
}

LUALIB_DLLEXPORT int luaLib_lua_pcall(luaWrap_lua_State* L,int nargs,int nresult,int errfunc)
{
    return(lua_pcall((lua_State*)L,nargs,nresult,errfunc));
}

LUALIB_DLLEXPORT int luaLib_lua_isuserdata(luaWrap_lua_State* L,int idx)
{
    return(lua_isuserdata((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_upvalueindex(int i)
{
    return(lua_upvalueindex(i));
}

LUALIB_DLLEXPORT int luaLib_getCurrentCodeLine(luaWrap_lua_State* L)
{
    int r=-1;
    lua_Debug ar;
    if (lua_getstack((lua_State*)L,1,&ar)==1)
    {
        if (lua_getinfo((lua_State*)L,"l",&ar)==1)
            r=ar.currentline;
    }
    return(r);
}

LUALIB_DLLEXPORT void luaLib_lua_settable(luaWrap_lua_State* L,int idx)
{
    lua_settable((lua_State*)L,idx);
}

LUALIB_DLLEXPORT int luaLib_lua_next(luaWrap_lua_State* L,int idx)
{
    return(lua_next((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_type(luaWrap_lua_State* L,int idx)
{
    return(lua_type((lua_State*)L,idx));
}

LUALIB_DLLEXPORT int luaLib_lua_error(luaWrap_lua_State* L)
{
    return(lua_error((lua_State*)L));
}
