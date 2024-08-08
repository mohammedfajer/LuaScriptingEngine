#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>
#include <lua.hpp>

// Enum for key codes
enum KeyCode {
	KEY_A = 97, // Replace with the actual value or mapping if needed
	KEY_B = 98,
	// Add other key codes as needed
};



// Function to register the library with Lua
extern "C" int luaopen_keyboard(lua_State * L);

#endif // KEYBOARD_H
