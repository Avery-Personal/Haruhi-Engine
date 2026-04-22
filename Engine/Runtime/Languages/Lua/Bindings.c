#include <stdio.h>
#include <string.h>

#include "Bindings.h"

void HaruLuaPushContext(lua_State *State, HaruContext *Context) {
    lua_pushlightuserdata(State, Context);
}

HaruContext *HaruLuaGetContext(lua_State *State) {
    lua_getfield(State, LUA_REGISTRYINDEX, HARU_LUA_CONTEXT_REGISTRY_KEY);
    
    HaruContext *Context = (HaruContext *) lua_touserdata(State, -1);
    
    lua_pop(State, 1);
    
    return Context;
}

void HaruLuaSetContext(lua_State *State, HaruContext *Context) {
    lua_pushlightuserdata(State, Context);
    lua_setfield(State, LUA_REGISTRYINDEX, HARU_LUA_CONTEXT_REGISTRY_KEY);
}

int HaruLuaThrow(lua_State *State, const char *Format, ...) {
    char Buffer[1024];

    va_list args;

    va_start(args, Format);
    vsnprintf(Buffer, sizeof(Buffer), Format, args);
    va_end(args);

    return luaL_error(State, "%s", Buffer);
}

int HaruLuaCheckArgCount(lua_State *State, int Expected) {
    return lua_gettop(State) >= Expected;
}

const char *HaruLuaOptString(lua_State *State, int Index, const char *DefaultValue) {
    return luaL_optstring(State, Index, DefaultValue);
}

const char *HaruLuaCheckString(lua_State *State, int Index) {
    return luaL_checkstring(State, Index);
}

lua_Integer HaruLuaCheckInteger(lua_State *State, int Index) {
    return luaL_checkinteger(State, Index);
}

lua_Number HaruLuaCheckNumber(lua_State *State, int Index) {
    return luaL_checknumber(State, Index);
}

int HaruLuaCheckBoolean(lua_State *State, int Index) {
    return lua_toboolean(State, Index) != 0;
}

void *HaruLuaCheckLightUserData(lua_State *State, int Index) {
    return lua_touserdata(State, Index);
}

void HaruLuaPushLightUserData(lua_State *State, void *Pointer) {
    lua_pushlightuserdata(State, Pointer);
}

int HaruLuaRegisterMetatable(lua_State *State, const char *Name, const luaL_Reg *Methods) {
    if (!State || !Name || !Methods)
        return 0;

    if (luaL_newmetatable(State, Name) == 0)
        return 1;

    luaL_setfuncs(State, Methods, 0);
    lua_pushvalue(State, -1);
    lua_setfield(State, -2, "__index");
    lua_pop(State, 1);

    return 1;
}

int HaruLuaRegisterLibrary(lua_State *State, const char *Name, const luaL_Reg *Functions) {
    if (!State || !Name || !Functions)
        return 0;

    luaL_newlib(State, Functions);
    lua_setglobal(State, Name);

    return 1;
}
