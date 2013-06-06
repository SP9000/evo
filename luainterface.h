#ifndef _LUAINTERFACE_H
#define _LUAINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <tolua.h>


TOLUA_API int tolua_luabindings_open (lua_State* tolua_S);

typedef struct tagLuaInterface {
    lua_State *state;
}LuaInterface;

void LuaInterfaceInit(LuaInterface *l);

void LuaInterfaceQuit();
    
#ifdef __cplusplus
}
#endif

#endif
