#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>


extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <iostream>

#include <chrono>
#include <string>
#include <windows.h>

#include "keyboard.h"
#include "graphics.h"

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;


std::chrono::system_clock::time_point getFileModificationTime(const std::string &filePath) {
	struct stat fileInfo;
	if (stat(filePath.c_str(), &fileInfo) == 0) {
		std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(fileInfo.st_mtime);
		return tp;
	}
	else {
		std::cerr << "Error: Unable to get file status for " << filePath << std::endl;
		return std::chrono::system_clock::now(); // Return current time in case of error
	}
}



extern "C"
{
	//
	void reloadLuaScript(lua_State *&L, const std::string &scriptPath, std::chrono::system_clock::time_point &lastModifiedTime) {

		std::chrono::system_clock::time_point currentModifiedTime = getFileModificationTime(scriptPath);

		if (currentModifiedTime > lastModifiedTime) {
			std::cout << "Reloading Lua script..." << std::endl;

			lua_close(L);
			L = luaL_newstate();
			luaL_openlibs(L);
			luaL_requiref(L, "graphics", luaopen_graphics, 1);
			lua_pop(L, 1);

			// TODO(mo): other libs such as keyboard, etc..

			if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK) {
				std::cerr << "Error reloading script: " << lua_tostring(L, -1) << std::endl;
			}

			lastModifiedTime = currentModifiedTime;

			// Update load
				// Ensure `mo` table and functions exist
			lua_getglobal(L, "mo");
			if (!lua_istable(L, -1)) {
				std::cerr << "Error: `mo` is not a table." << std::endl;
				lua_close(L);
				SDL_DestroyRenderer(gRenderer);
				SDL_DestroyWindow(gWindow);
				SDL_Quit();
				return ;
			}
			lua_pop(L, 1);

			lua_getglobal(L, "mo");
			lua_getfield(L, -1, "load");
			if (lua_isfunction(L, -1)) {
				lua_pcall(L, 0, 0, 0);
			}
			else {
				std::cerr << "Error: `mo.load` function is not defined." << std::endl;
			}
			lua_pop(L, 1);
		}
	}
}


int main(int argc, char *argv[])
{

#pragma region SDL2 and SDL2 Image Setup (Window and Renderer)

	SDL_Init(SDL_INIT_VIDEO);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	gWindow = SDL_CreateWindow("Simple SDL2 Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800, // width
		600, // height
		SDL_WINDOW_SHOWN);

	if (gWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// Create a renderer
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}
#pragma endregion


	// Lua State and Stack
	lua_State *L = luaL_newstate();
	luaL_openlibs(L); // Standard Libraries

	// Load the custom graphics library
	luaL_requiref(L, "graphics", luaopen_graphics, 1);
	lua_pop(L, 1); // Remove the module from the stack

	// Load the custom keyboard library
	luaL_requiref(L, "keyboard", luaopen_keyboard, 1);
	lua_pop(L, 1); // Remove the module from the stack


	// Create the "mo" table implicitly
	lua_newtable(L); // create a new table

	// Add the custom libraries as sub-tables implicitly
	lua_getglobal(L, "graphics"); // Load the graphics library
	lua_setfield(L, -2, "graphics"); // Set it as the field of the `mo` table



	lua_setglobal(L, "mo"); // Push the table onto the stack


	std::string scriptPath = "script.lua";
	std::chrono::system_clock::time_point lastModifiedTime = getFileModificationTime(scriptPath);

	// Entry Point to the scripts
	if (luaL_dofile(L, "script.lua") != LUA_OK) {
		std::cerr << "Error loading script.lua: " << lua_tostring(L, -1) << std::endl;
		lua_close(L);
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}

	// Ensure `mo` table and functions exist
	lua_getglobal(L, "mo");
	if (!lua_istable(L, -1)) {
		std::cerr << "Error: `mo` is not a table." << std::endl;
		lua_close(L);
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		SDL_Quit();
		return 1;
	}
	lua_pop(L, 1);




	// Call mo.load
	lua_getglobal(L, "mo");
	lua_getfield(L, -1, "load");
	if (lua_isfunction(L, -1)) {
		lua_pcall(L, 0, 0, 0);
	}
	else {
		std::cerr << "Error: `mo.load` function is not defined." << std::endl;
	}
	lua_pop(L, 1);
	// end!



	// Main loop flag
	int quit = 0;

	// Event handler
	SDL_Event e;
	Uint32 lastTime = SDL_GetTicks();

	// While the application is running
	while (!quit) {
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		// Handle events on the queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
		}

		reloadLuaScript(L, scriptPath, lastModifiedTime);

		// Call mo.update(dt)
		lua_getglobal(L, "mo");
		lua_getfield(L, -1, "update");
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, deltaTime);
			if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
				std::cerr << "Error calling mo.update: " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
		}
		else {
			std::cerr << "Error: `mo.update` function is not defined." << std::endl;
		}
		lua_pop(L, 1);
		// end!


		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set color to black
		SDL_RenderClear(gRenderer);

		// Call mo.draw
		lua_getglobal(L, "mo");
		lua_getfield(L, -1, "draw");
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
				std::cerr << "Error calling mo.draw: " << lua_tostring(L, -1) << std::endl;
				lua_pop(L, 1);
			}
		}
		else {
			std::cerr << "Error: `mo.draw` function is not defined." << std::endl;
		}
		lua_pop(L, 1);
		// end!


		// Update screen
		SDL_RenderPresent(gRenderer);

		
	}

#pragma region Cleanup
	lua_close(L);

	// Destroy renderer and window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	// Quit SDL subsystems
	SDL_Quit();
#pragma endregion


	return 0;
}


