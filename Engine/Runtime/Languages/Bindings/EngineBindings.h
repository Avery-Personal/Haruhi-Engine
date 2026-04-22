#ifndef ENGINE_BINDINGS_H
#define ENGINE_BINDINGS_H

    #include <lua/lua.h>

    typedef struct HaruContext HaruContext;

    void HaruLuaRegisterEngineBindings(lua_State *State, HaruContext *Context);

#endif
