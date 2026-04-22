#ifndef LUA_RUNTIME_H
#define LUA_RUNTIME_H

    #include <lua/lua.h>

    typedef struct HaruContext HaruContext;

    typedef struct HaruLuaRuntime {
        lua_State *State;
        HaruContext *Context;

        int IsInitialized;
    } HaruLuaRuntime;

    int HaruLuaInit(HaruLuaRuntime *Runtime, HaruContext *Context);
    void HaruLuaShutdown(HaruLuaRuntime *Runtime);
    int HaruLuaRunFile(HaruLuaRuntime *Runtime, const char *Path);
    int HaruLuaRunString(HaruLuaRuntime *Runtime, const char *Code, const char *ChunkName);
    int HaruLuaCallGlobal(HaruLuaRuntime *Runtime, const char *Name, int ArgumentCount, int ResultCount);

#endif
