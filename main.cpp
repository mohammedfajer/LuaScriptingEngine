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

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

#include <map>

std::map<int, SDL_Texture *> gImages;
int gNextImageId = 1;





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

// Lua: graphics.rectangle(mode, x, y, w, h)
int lua_drawRectangle(lua_State *L)
{
	const char *mode = lua_tostring(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int w = lua_tointeger(L, 4);
	int h = lua_tointeger(L, 5);

	SDL_Rect rect = { x, y, w, h };

	if (strcmp(mode, "fill") == 0) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &rect);
	}
	else if (strcmp(mode, "line") == 0) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // Set color to white
		SDL_RenderDrawRect(gRenderer, &rect);
	}
	else
	{
		std::cerr << "Unknown mode: " << mode << std::endl;
	}

	return 0;
}

int lua_setBackgroundColor(lua_State *L)
{
	int red = lua_tointeger(L, 1);
	int green = lua_tointeger(L, 2);
	int blue = lua_tointeger(L, 3);
	int alpha = lua_tointeger(L, 4);


	SDL_SetRenderDrawColor(gRenderer, red, green, blue, alpha); // Set color to black
	SDL_RenderClear(gRenderer);

	return 0;
}


int lua_loadImage(lua_State *L)
{
	const char *path = luaL_checkstring(L, 1);

	SDL_Surface *surface = IMG_Load(path);
	if (!surface) {
		lua_pushnil(L);
		lua_pushstring(L, IMG_GetError());
		return 2;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		lua_pushnil(L);
		lua_pushstring(L, SDL_GetError());
		return 2;
	}

	int imageId = gNextImageId++;
	gImages[imageId] = texture;
	lua_pushinteger(L, imageId);

	return 1;
}


int lua_drawImage(lua_State *L) {
	int imageId = luaL_checkinteger(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	auto it = gImages.find(imageId);
	if (it == gImages.end()) {
		lua_pushboolean(L, 0);
		lua_pushstring(L, "Image not found");
		return 2;
	}

	SDL_Texture *texture = it->second;
	SDL_Rect dstRect = { x, y, 0, 0 };
	SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);

	SDL_RenderCopy(gRenderer, texture, nullptr, &dstRect);
	lua_pushboolean(L, 1);

	return 1;
}




std::map<int, SDL_Rect> gQuads;  // Global map for quads
int gNextQuadId = 1;             // Counter for generating unique Quad IDs

int lua_newQuad(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int width = luaL_checkinteger(L, 3);
	int height = luaL_checkinteger(L, 4);
	int imageId = luaL_checkinteger(L, 5);

	// Check if the image exists
	if (gImages.find(imageId) == gImages.end()) {
		lua_pushnil(L);
		lua_pushstring(L, "Image not found");
		return 2;
	}

	// Create and store the SDL_Rect for the quad
	SDL_Rect quad = { x, y, width, height };
	int quadId = gNextQuadId++;
	gQuads[quadId] = quad;

	lua_pushinteger(L, quadId);
	return 1;
}

int lua_renderQuad(lua_State *L) {
	// Retrieve arguments from Lua
	int imageId = luaL_checkinteger(L, 1);
	int quadId = luaL_checkinteger(L, 2);
	int x = luaL_checkinteger(L, 3);
	int y = luaL_checkinteger(L, 4);


	// Find the texture and quad in the global maps
	auto imgIt = gImages.find(imageId);
	auto quadIt = gQuads.find(quadId);

	if (imgIt == gImages.end() || quadIt == gQuads.end()) {
		// Push an error message to Lua and return
		lua_pushboolean(L, 0);
		lua_pushstring(L, "Texture or Quad not found");
		return 2;
	}

	SDL_Texture *texture = imgIt->second;
	SDL_Rect srcRect = quadIt->second;
	SDL_Rect dstRect = { x, y, srcRect.w, srcRect.h };

	// Perform the rendering
	SDL_RenderCopy(gRenderer, texture, &srcRect, &dstRect);

	// Indicate success
	lua_pushboolean(L, 1);
	return 1;  // Return a success indicator
}



// Lua library functions
static const luaL_Reg graphicsLib[] = {
	{"rectangle", lua_drawRectangle},
	{"setBackgroundColor", lua_setBackgroundColor},
	{"newImage", lua_loadImage},
	{"draw", lua_drawImage},
	{"drawQuad", lua_renderQuad},
	{"newQuad", lua_newQuad},
	{NULL, NULL}

};

extern "C" int luaopen_graphics(lua_State * L)
{
	luaL_newlib(L, graphicsLib);
	return 1;
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

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Load the custom graphics library
	
	luaL_requiref(L, "graphics", luaopen_graphics, 1);
	lua_pop(L, 1); // Remove the module from the stack

	// Load the custom keyboard library
	luaL_requiref(L, "keyboard", luaopen_keyboard, 1);
	lua_pop(L, 1); // Remove the module from the stack


	std::string scriptPath = "script.lua";
	std::chrono::system_clock::time_point lastModifiedTime = getFileModificationTime(scriptPath);

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

	lua_getglobal(L, "mo");
	lua_getfield(L, -1, "load");
	if (lua_isfunction(L, -1)) {
		lua_pcall(L, 0, 0, 0);
	}
	else {
		std::cerr << "Error: `mo.load` function is not defined." << std::endl;
	}
	lua_pop(L, 1);




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

		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Set color to black
		SDL_RenderClear(gRenderer);

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


		// Update screen
		SDL_RenderPresent(gRenderer);


		
	}

	lua_close(L);

	// Destroy renderer and window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	// Quit SDL subsystems
	SDL_Quit();


	return 0;
}


