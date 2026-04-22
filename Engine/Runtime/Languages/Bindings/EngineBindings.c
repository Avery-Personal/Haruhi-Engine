#include <stdio.h>
#include <time.h>

#include <lua/lauxlib.h>

#include "EngineBindings.h"
#include "../Lua/Bindings.h"

static void DefaultLog(void *UserData, const char *message) {
    (void) UserData;

    printf("%s\n", message ? message : "(null)");
}

static double DefaultTimeSeconds(void *UserData) {
    (void) UserData;

    return (double) clock() / (double) CLOCKS_PER_SEC;
}

static void DefaultRequestQuit(void *UserData, int Code) {
    (void) UserData;

    printf("Quit requested with Code %d\n", Code);
}

struct HaruContext {
    void *UserData;
    void (*Log)(void *UserData, const char *message);
    double (*GetTimeSeconds)(void *UserData);
    void (*RequestQuit)(void *UserData, int Code);
};

static HaruContext *GetEffectiveContext(lua_State *State) {
    HaruContext *Context = HaruLuaGetContext(State);

    return Context;
}

static int LuaHaruLog(lua_State *State) {
    HaruContext *Context = GetEffectiveContext(State);
    const char *Message = luaL_checkstring(State, 1);

    if (Context && Context  -> Log)
        Context  -> Log(Context  -> UserData, Message);
    else
        DefaultLog(NULL, Message);

    return 0;
}

static int LuaHaruTime(lua_State *State) {
    HaruContext *Context = GetEffectiveContext(State);
    
    double Time = Context && Context  -> GetTimeSeconds ? Context  -> GetTimeSeconds(Context  -> UserData) : DefaultTimeSeconds(NULL);
    
    lua_pushnumber(State, (lua_Number)Time);
    
    return 1;
}

static int LuaHaruQuit(lua_State *State) {
    HaruContext *Context = GetEffectiveContext(State);
    int Code = (int) luaL_optinteger(State, 1, 0);

    if (Context && Context  -> RequestQuit)
        Context  -> RequestQuit(Context  -> UserData, Code);
    else
        DefaultRequestQuit(NULL, Code);

    return 0;
}

static int LuaHaruDebugStack(lua_State *State) {
    const char *Label = luaL_optstring(State, 1, "Lua stack");

    HaruLuaDebugDumpStack(State, Label);

    return 0;
}

static int LuaHaruDebugGlobals(lua_State *State) {
    const char *Label = luaL_optstring(State, 1, "Globals");

    int MaxEntries = (int) luaL_optinteger(State, 2, 16);

    HaruLuaDebugDumpGlobals(State, Label, MaxEntries);

    return 0;
}

void HaruLuaRegisterEngineBindings(lua_State *State, HaruContext *Context) {
    (void) Context;

    lua_newtable(State);

    lua_pushcfunction(State, LuaHaruLog);
    lua_setfield(State, -2, "log");

    lua_pushcfunction(State, LuaHaruTime);
    lua_setfield(State, -2, "time");

    lua_pushcfunction(State, LuaHaruQuit);
    lua_setfield(State, -2, "quit");

    lua_newtable(State);

    lua_pushcfunction(State, LuaHaruDebugStack);
    lua_setfield(State, -2, "stack");

    lua_pushcfunction(State, LuaHaruDebugGlobals);
    lua_setfield(State, -2, "globals");

    lua_setfield(State, -2, "debug");

    lua_setglobal(State, "Haru");
}
