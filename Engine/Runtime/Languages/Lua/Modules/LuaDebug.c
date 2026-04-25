#include <stdio.h>

#include "../Bindings.h"

static void HaruLuaPrintValue(lua_State *State, int Index) {
    int Type = lua_type(State, Index);

    switch (Type) {
        case LUA_TSTRING:
            printf("%s", lua_tostring(State, Index));
            
            break;

        case LUA_TBOOLEAN:
            printf(lua_toboolean(State, Index) ? "true" : "false");
            
            break;

        case LUA_TNUMBER:
            printf("%g", lua_tonumber(State, Index));
            
            break;

        case LUA_TNIL:
            printf("nil");
            
            break;

        default:
            printf("%s:%p", lua_typename(State, Type), lua_topointer(State, Index));
            
            break;
    }
}

void HaruLuaDebugDumpStack(lua_State *State, const char *Label) {
    int Top = lua_gettop(State);

    printf("%s | stack size = %d\n", Label ? Label : "stack", Top);

    for (int i = 1; i <= Top; ++i) {
        printf("  [%d] ", i);

        HaruLuaPrintValue(State, i);

        printf("\n");
    }
}

void HaruLuaDebugDumpGlobals(lua_State *State, const char *Label, int MaxEntries) {
    int Count = 0;

    printf("%s\n", Label ? Label : "globals");

    lua_pushglobaltable(State);
    lua_pushnil(State);

    while (lua_next(State, -2) != 0) {
        printf("  ");

        HaruLuaPrintValue(State, -2);

        printf(" = ");

        HaruLuaPrintValue(State, -1);

        printf("\n");

        lua_pop(State, 1);

        if (++Count >= MaxEntries)
            break;
    }

    lua_pop(State, 1);
}

int HaruLuaDebugTraceback(lua_State *State) {
    const char *Message = lua_tostring(State, 1);
    if (Message)
        lua_pushfstring(State, "\nstack traceback:\n%s", Message);
    else
        lua_pushliteral(State, "\nstack traceback:");

    return 1;
}
