#include "keyboard.h"

// SDL2 keyboard functions

SDL_Keycode getKeyFromScancode(SDL_Scancode scancode) {
	return SDL_GetKeyFromScancode(scancode);
}

SDL_Scancode getScancodeFromKey(SDL_Keycode key) {
	return SDL_GetScancodeFromKey(key);
}

bool hasKeyRepeat() {
	//int repeat;
	//SDL_GetKeyboardRepeat(&repeat, nullptr);
	//return repeat != 0;
	return false;
}

bool hasScreenKeyboard() {
	// SDL2 does not provide a native way to check for screen keyboard support
	return false;
}

bool hasTextInput() {
	return SDL_IsTextInputActive();
}

bool isKeyDown(SDL_Keycode key) {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	return state[SDL_GetScancodeFromKey(key)];
}

bool isScancodeDown(SDL_Scancode scancode) {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	return state[scancode];
}

void setKeyRepeat(bool repeat) {
	//SDL_SetKeyboardRepeat(repeat ? SDL_DEFAULT_REPEAT_DELAY : 0, SDL_DEFAULT_REPEAT_INTERVAL);
}

void setTextInput(bool enabled) {
	if (enabled) {
		SDL_StartTextInput();
	}
	else {
		SDL_StopTextInput();
	}
}

// Lua bindings

int lua_getKeyFromScancode(lua_State *L) {
	SDL_Scancode scancode = static_cast<SDL_Scancode>(luaL_checkinteger(L, 1));
	lua_pushinteger(L, getKeyFromScancode(scancode));
	return 1;
}

int lua_getScancodeFromKey(lua_State *L) {
	SDL_Keycode key = static_cast<SDL_Keycode>(luaL_checkinteger(L, 1));
	lua_pushinteger(L, getScancodeFromKey(key));
	return 1;
}

int lua_hasKeyRepeat(lua_State *L) {
	lua_pushboolean(L, hasKeyRepeat());
	return 1;
}

int lua_hasScreenKeyboard(lua_State *L) {
	lua_pushboolean(L, hasScreenKeyboard());
	return 1;
}

int lua_hasTextInput(lua_State *L) {
	lua_pushboolean(L, hasTextInput());
	return 1;
}

int lua_isKeyDown(lua_State *L) {
	SDL_Keycode key = static_cast<SDL_Keycode>(luaL_checkinteger(L, 1));
	lua_pushboolean(L, isKeyDown(key));
	return 1;
}

int lua_isScancodeDown(lua_State *L) {
	SDL_Scancode scancode = static_cast<SDL_Scancode>(luaL_checkinteger(L, 1));
	lua_pushboolean(L, isScancodeDown(scancode));
	return 1;
}

int lua_setKeyRepeat(lua_State *L) {
	bool repeat = lua_toboolean(L, 1);
	setKeyRepeat(repeat);
	return 0;
}

int lua_setTextInput(lua_State *L) {
	bool enabled = lua_toboolean(L, 1);
	setTextInput(enabled);
	return 0;
}

void lua_registerEnums(lua_State *L)
{
	lua_newtable(L); // Create a new table for the enums
	lua_pushinteger(L, KEY_A);
	lua_setfield(L, -2, "KEY_A");
	lua_pushinteger(L, KEY_B);
	lua_setfield(L, -2, "KEY_B");
	// Add other key codes as needed

	lua_setglobal(L, "KeyCode"); // Set the table as a global variable
}



// Library function to open the keyboard module
extern "C" int luaopen_keyboard(lua_State * L) {
	static const luaL_Reg keyboardLib[] = {
		{"getKeyFromScancode", lua_getKeyFromScancode},
		{"getScancodeFromKey", lua_getScancodeFromKey},
		{"hasKeyRepeat", lua_hasKeyRepeat},
		{"hasScreenKeyboard", lua_hasScreenKeyboard},
		{"hasTextInput", lua_hasTextInput},
		{"isKeyDown", lua_isKeyDown},
		{"isScancodeDown", lua_isScancodeDown},
		{"setKeyRepeat", lua_setKeyRepeat},
		{"setTextInput", lua_setTextInput},
		{nullptr, nullptr} // Sentinel to mark the end of the array
	};

	luaL_newlib(L, keyboardLib); // Create a new table and add functions to it

	// Register enums
	lua_registerEnums(L);


	return 1; // Number of return values
}
