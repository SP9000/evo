#include "luainterface.h"

void LuaInterfaceInit(LuaInterface *L)
{
    /* Initialize Lua. */
    L->state = luaL_newstate();

    /* Load Lua base librarires. */
    luaL_openlibs(L->state);

    /* open the tolua generated bindings. */
    //TODO:
    tolua_toluabind_open(L->state);

    int s = luaL_loadfile(L->state, "start.lua");
    if(s == 0) {
        /* execute start lua program. */
        s = lua_pcall(L->state, 0, LUA_MULTRET, 0);
    }
}

void LuaInterfaceQuit(LuaInterface *L)
{
    lua_close(L->state);
}
