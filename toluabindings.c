/*
** Lua binding: toluabind
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
TOLUA_API int tolua_toluabind_open (lua_State* tolua_S);
LUALIB_API int luaopen_toluabind (lua_State* tolua_S);

#include "toluaheader2.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Model");
}

/* function: ModelNew */
static int tolua_toluabind_Model_New00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isnumber(tolua_S,1,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  int numVertices = ((int)  tolua_tonumber(tolua_S,1,0));
 {
  Model* tolua_ret = (Model*)  ModelNew(numVertices);
 tolua_pushusertype(tolua_S,(void*)tolua_ret,"Model");
 }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'New'.",&tolua_err);
 return 0;
#endif
}

/* function: ModelAddVertex */
static int tolua_toluabind_Model_AddVertex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isusertype(tolua_S,1,"Model",0,&tolua_err) || 
 !tolua_istable(tolua_S,2,0,&tolua_err) || 
 !tolua_istable(tolua_S,3,0,&tolua_err) || 
 !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  Model* m = ((Model*)  tolua_tousertype(tolua_S,1,0));
  float v[3];
  float c[4];
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,2,3,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<3;i++)
  v[i] = ((float)  tolua_tofieldnumber(tolua_S,2,i+1,0));
 }
 }
 {
#ifndef TOLUA_RELEASE
 if (!tolua_isnumberarray(tolua_S,3,4,0,&tolua_err))
 goto tolua_lerror;
 else
#endif
 {
 int i;
 for(i=0; i<4;i++)
  c[i] = ((float)  tolua_tofieldnumber(tolua_S,3,i+1,0));
 }
 }
 {
  ModelAddVertex(m,v,c);
 }
 {
 int i;
 for(i=0; i<3;i++)
 tolua_pushfieldnumber(tolua_S,2,i+1,(lua_Number) v[i]);
 }
 {
 int i;
 for(i=0; i<4;i++)
 tolua_pushfieldnumber(tolua_S,3,i+1,(lua_Number) c[i]);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddVertex'.",&tolua_err);
 return 0;
#endif
}

/* Open lib function */
LUALIB_API int luaopen_toluabind (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"Model",0);
 tolua_beginmodule(tolua_S,"Model");
 tolua_function(tolua_S,"New",tolua_toluabind_Model_New00);
 tolua_function(tolua_S,"AddVertex",tolua_toluabind_Model_AddVertex00);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_toluabind_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_toluabind);
 lua_pushstring(tolua_S, "toluabind");
 lua_call(tolua_S, 1, 0);
 return 1;
}
