#include <stdio.h>
#include <string.h>

#include <lua/lualib.h>

#include "LuaRuntime.h"
#include "Bindings.h"
#include "../Bindings/EngineBindings.h"
#include "ScriptLoader.h"

static int HaruLuaReportError(lua_State *State, const char *Context) {
    const char *Message = lua_tostring(State, -1);
    if (!Message)
        Message = "(no error message)";

    if (Context && Context[0] != '\0')
        fprintf(stderr, "%s: %s\n", Context, Message);
    else
        fprintf(stderr, "%s\n", Message);

    lua_pop(State, 1);

    return 0;
}

int HaruLuaInit(HaruLuaRuntime *Runtime, HaruContext *Context) {
    if (!Runtime)
        return 0;

    memset(Runtime, 0, sizeof(*Runtime));

    Runtime -> Context = Context;
    Runtime -> State = luaL_newstate();
    if (!Runtime -> State)
        return 0;

    luaL_openlibs(Runtime -> State);

    HaruLuaSetContext(Runtime -> State, Context);
    HaruLuaRegisterEngineBindings(Runtime -> State, Context);

    Runtime -> IsInitialized = 1;

    return 1;
}

void HaruLuaShutdown(HaruLuaRuntime *Runtime) {
    if (!Runtime || !Runtime -> State)
        return;

    lua_close(Runtime -> State);

    Runtime -> State = NULL;
    Runtime -> Context = NULL;
    Runtime -> IsInitialized = 0;
}

int HaruLuaRunString(HaruLuaRuntime *Runtime, const char *code, const char *chunkName) {
    if (!Runtime || !Runtime -> State || !code)
        return 0;

    const char *Name = (chunkName && chunkName[0] != '\0') ? chunkName : "=HaruhiChunk";

    if (luaL_loadbuffer(Runtime -> State, code, strlen(code), Name) != LUA_OK) {
        HaruLuaReportError(Runtime -> State, Name);

        return 0;
    }

    if (lua_pcall(Runtime -> State, 0, LUA_MULTRET, 0) != LUA_OK) {
        HaruLuaReportError(Runtime -> State, Name);

        return 0;
    }

    return 1;
}

int HaruLuaRunFile(HaruLuaRuntime *Runtime, const char *Path) {
    if (!Runtime || !Runtime -> State || !Path)
        return 0;

    if (luaL_loadfile(Runtime -> State, Path) != LUA_OK) {
        HaruLuaReportError(Runtime -> State, Path);

        return 0;
    }

    if (lua_pcall(Runtime -> State, 0, LUA_MULTRET, 0) != LUA_OK) {
        HaruLuaReportError(Runtime -> State, Path);

        return 0;
    }

    return 1;
}

int HaruLuaCallGlobal(HaruLuaRuntime *Runtime, const char *Name, int ArgumentCount, int ResultCount) {
    if (!Runtime || !Runtime -> State || !Name)
        return 0;

    lua_getglobal(Runtime -> State, Name);

    if (!lua_isfunction(Runtime -> State, -1)) {
        lua_pop(Runtime -> State, 1);

        return 0;
    }

    if (lua_pcall(Runtime -> State, ArgumentCount, ResultCount, 0) != LUA_OK) {
        HaruLuaReportError(Runtime -> State, Name);

        return 0;
    }

    return 1;
}
