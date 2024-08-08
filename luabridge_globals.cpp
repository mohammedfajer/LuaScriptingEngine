//#include <iostream>
//extern "C"
//{
//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>
//}
//#include <LuaBridge/LuaBridge.h>
//
//int globalValue = 42;
//
//int main() {
//	lua_State *L = luaL_newstate();
//	luaL_openlibs(L);
//
//	// Expose global variable to Lua
//	luabridge::getGlobalNamespace(L)
//		.beginNamespace("test")
//			.addProperty("globalValue", &globalValue)
//			
//		.endNamespace();
//
//
//	// Load and execute Lua script
//	luaL_dofile(L, "test_globals.lua");
//
//	lua_close(L);
//	return 0;
//}
