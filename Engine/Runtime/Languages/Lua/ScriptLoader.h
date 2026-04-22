#ifndef SCRIPT_LOADER_H
#define SCRIPT_LOADER_H

    #include "LuaRuntime.h"

    int HaruLua_LoadFile(HaruLuaRuntime *Runtime, const char *Path);
    int HaruLua_LoadString(HaruLuaRuntime *Runtime, const char *Code, const char *ChunkName);
    int HaruLua_CallGlobal0(HaruLuaRuntime *Runtime, const char *FunctionName);

#endif
