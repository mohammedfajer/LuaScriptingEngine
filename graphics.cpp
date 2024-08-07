#include "graphics.h"

#include <SDL_image.h>

#include <iostream> // std::cerr
#include <map>

extern SDL_Renderer *gRenderer;

static std::map<int, SDL_Texture *> gImages;
static int gNextImageId = 1;

static std::map<int, SDL_Rect> gQuads;  // Global map for quads
static int gNextQuadId = 1;             // Counter for generating unique Quad IDs

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


extern "C" int luaopen_graphics(lua_State * L)
{
	static const luaL_Reg graphicsLib[] = {
	{"rectangle", lua_drawRectangle},
	{"setBackgroundColor", lua_setBackgroundColor},
	{"newImage", lua_loadImage},
	{"draw", lua_drawImage},
	{"drawQuad", lua_renderQuad},
	{"newQuad", lua_newQuad},
	{NULL, NULL}

	};


	luaL_newlib(L, graphicsLib);
	return 1;
}
