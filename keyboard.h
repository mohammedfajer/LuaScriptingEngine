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

// SDL2 keyboard functions
SDL_Keycode getKeyFromScancode(SDL_Scancode scancode);
SDL_Scancode getScancodeFromKey(SDL_Keycode key);
bool hasKeyRepeat();
bool hasScreenKeyboard();
bool hasTextInput();
bool isKeyDown(SDL_Keycode key);
bool isScancodeDown(SDL_Scancode scancode);
void setKeyRepeat(bool repeat);
void setTextInput(bool enabled);

// Lua bindings
int lua_getKeyFromScancode(lua_State *L);
int lua_getScancodeFromKey(lua_State *L);
int lua_hasKeyRepeat(lua_State *L);
int lua_hasScreenKeyboard(lua_State *L);
int lua_hasTextInput(lua_State *L);
int lua_isKeyDown(lua_State *L);
int lua_isScancodeDown(lua_State *L);
int lua_setKeyRepeat(lua_State *L);
int lua_setTextInput(lua_State *L);




void lua_registerEnums(lua_State *L);

// Function to register the library with Lua
extern "C" int luaopen_keyboard(lua_State * L);

#endif // KEYBOARD_H
