#include "ScriptLoader.h"
#include "Bindings.h"

#include <stdio.h>
#include <string.h>

#include <lua/lauxlib.h>

static int HaruLuaPcallReport(lua_State *State, const char *Label) {
    if (lua_pcall(State, 0, LUA_MULTRET, 0) != LUA_OK) {
        const char *Message = lua_tostring(State, -1);
        
        fprintf(stderr, "%s: %s\n", Label ? Label : "script", Message ? Message : "unknown error");
        
        lua_pop(State, 1);
        
        return 0;
    }

    return 1;
}

int HaruLuaLoadString(HaruLuaRuntime *Runtime, const char *Code, const char *ChunkName) {
    if (!Runtime || !Runtime -> State || !Code)
        return 0;

    const char *Label = (ChunkName && ChunkName[0] != '\0') ? ChunkName : "=HaruhiString";

    if (luaL_loadbuffer(Runtime -> State, Code, strlen(Code), Label) != LUA_OK) {
        const char *Message = lua_tostring(Runtime -> State, -1);

        fprintf(stderr, "%s: %s\n", Label, Message ? Message : "unknown load error");

        lua_pop(Runtime -> State, 1);

        return 0;
    }

    return HaruLuaPcallReport(Runtime -> State, Label);
}

int HaruLuaLoadFile(HaruLuaRuntime *Runtime, const char *path) {
    if (!Runtime || !Runtime -> State || !path)
        return 0;

    if (luaL_loadfile(Runtime -> State, path) != LUA_OK) {
        const char *Message = lua_tostring(Runtime -> State, -1);
        
        fprintf(stderr, "%s: %s\n", path, Message ? Message : "unknown load error");
        
        lua_pop(Runtime -> State, 1);
        
        return 0;
    }

    return HaruLuaPcallReport(Runtime -> State, path);
}

int HaruLuaCallGlobal0(HaruLuaRuntime *Runtime, const char *functionName) {
    if (!Runtime || !Runtime -> State || !functionName)
        return 0;

    lua_getglobal(Runtime -> State, functionName);
    if (!lua_isfunction(Runtime -> State, -1)) {
        lua_pop(Runtime -> State, 1);
        
        return 0;
    }

    if (lua_pcall(Runtime -> State, 0, 0, 0) != LUA_OK) {
        const char *Message = lua_tostring(Runtime -> State, -1);
        
        fprintf(stderr, "%s: %s\n", functionName, Message ? Message : "unknown call error");
        
        lua_pop(Runtime -> State, 1);
        
        return 0;
    }

    return 1;
}
