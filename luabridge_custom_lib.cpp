#include <iostream>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace MyLibrary {
	int multiply(int a, int b) {
		return a * b;
	}

	void printText(const std::string &text) {
		std::cout << "Library Text: " << text << std::endl;
	}
}

int main() {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Expose custom library
	luabridge::getGlobalNamespace(L)
		.beginNamespace("MyLibrary")
		.addFunction("multiply", MyLibrary::multiply)
		.addFunction("printText", MyLibrary::printText)
		.endNamespace();

	// Load and execute Lua script
	luaL_dofile(L, "test_custom_library.lua");

	lua_close(L);
	return 0;
}
