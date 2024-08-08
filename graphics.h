#pragma once



#include <SDL.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}




extern "C" int luaopen_graphics(lua_State * L);