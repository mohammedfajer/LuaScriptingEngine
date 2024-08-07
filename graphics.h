#pragma once



#include <SDL.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}




int lua_drawRectangle(lua_State *L);
int lua_setBackgroundColor(lua_State *L);
int lua_loadImage(lua_State *L);
int lua_drawImage(lua_State *L);
int lua_newQuad(lua_State *L);
int lua_renderQuad(lua_State *L);


extern "C" int luaopen_graphics(lua_State * L);