//extern "C"
//{
//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>
//}
//
//#include <LuaBridge/LuaBridge.h>
//#include <iostream>
//
//
//
//int add(int a, int b)
//{
//	return a + b;
//}
//
//void printMessage(const std::string &message)
//{
//	std::cout << "Message from Lua: " << message << std::endl;
//}
//
//
//int main()
//{
//	std::cout << "Hello LuaBridge, Show us what you can do baby.\n";
//
//	lua_State *L = luaL_newstate();
//	luaL_openlibs(L);
//
//	// Expose functions to lua
//	luabridge::getGlobalNamespace(L)
//		.addFunction("add", add)
//		.addFunction("printMessage", printMessage);
//
//
//
//	// Load and execute Lua script
//	luaL_dofile(L, "test_functions.lua");
//
//	lua_close(L);
//
//	return 0;
//}