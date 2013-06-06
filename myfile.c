/*
** Lua binding: pkgname
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_pkgname_open (lua_State* tolua_S);
LUALIB_API int luaopen_pkgname (lua_State* tolua_S);

#include "toluaheader.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* Open lib function */
LUALIB_API int luaopen_pkgname (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_pkgname_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_pkgname);
 lua_pushstring(tolua_S, "pkgname");
 lua_call(tolua_S, 1, 0);
 return 1;
}
