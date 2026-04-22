#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

    #include <stdarg.h>

    #include <lua/lua.h>
    #include <lua/lauxlib.h>

    typedef struct HaruContext HaruContext;

    #define HARU_LUA_CONTEXT_REGISTRY_KEY "Haruhi.Context"

    void HaruLuaPushContext(lua_State *State, HaruContext *Context);
    HaruContext *HaruLuaGetContext(lua_State *State);
    void HaruLuaSetContext(lua_State *State, HaruContext *Context);

    int HaruLuaThrow(lua_State *State, const char *Format, ...);
    int HaruLuaCheckArgCount(lua_State *State, int expected);

    const char *HaruLuaOptString(lua_State *State, int Index, const char *DefaultValue);
    const char *HaruLuaCheckString(lua_State *State, int Index);
    lua_Integer HaruLuaCheckInteger(lua_State *State, int Index);
    lua_Number HaruLuaCheckNumber(lua_State *State, int Index);
    int HaruLuaCheckBoolean(lua_State *State, int Index);

    void *HaruLuaCheckLightUserData(lua_State *State, int Index);
    void HaruLuaPushLightUserData(lua_State *State, void *Pointer);

    int HaruLuaRegisterMetatable(lua_State *State, const char *Name, const luaL_Reg *Methods);
    int HaruLuaRegisterLibrary(lua_State *State, const char *Name, const luaL_Reg *Functions);

    void HaruLuaDebugDumpStack(lua_State *State, const char *Label);
    void HaruLuaDebugDumpGlobals(lua_State *State, const char *Label, int MaxEntries);
    int HaruLuaDebugTraceback(lua_State *State);

#endif
