//#include "graphics.h"
//
//#include <SDL_image.h>
//
//#include <iostream> // std::cerr
//#include <map>
//
//extern SDL_Renderer *gRenderer;
//
//static std::map<int, SDL_Texture *> gImages;
//static int gNextImageId = 1;
//
//static std::map<int, SDL_Rect> gQuads;  // Global map for quads
//static int gNextQuadId = 1;             // Counter for generating unique Quad IDs
//
//
//extern SDL_Window *gWindow;
//
//
//#include <vector>
//
//
//
//// Lua: graphics.rectangle(mode, x, y, w, h)
//int lua_drawRectangle(lua_State *L)
//{
//	const char *mode = lua_tostring(L, 1);
//	int x = lua_tointeger(L, 2);
//	int y = lua_tointeger(L, 3);
//	int w = lua_tointeger(L, 4);
//	int h = lua_tointeger(L, 5);
//
//	SDL_Rect rect = { x, y, w, h };
//
//	if (strcmp(mode, "fill") == 0) {
//		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
//		SDL_RenderFillRect(gRenderer, &rect);
//	}
//	else if (strcmp(mode, "line") == 0) {
//		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255); // Set color to white
//		SDL_RenderDrawRect(gRenderer, &rect);
//	}
//	else
//	{
//		std::cerr << "Unknown mode: " << mode << std::endl;
//	}
//
//	return 0;
//}
//
//int lua_setBackgroundColor(lua_State *L)
//{
//	int red = lua_tointeger(L, 1);
//	int green = lua_tointeger(L, 2);
//	int blue = lua_tointeger(L, 3);
//	int alpha = lua_tointeger(L, 4);
//
//
//	SDL_SetRenderDrawColor(gRenderer, red, green, blue, alpha); // Set color to black
//	SDL_RenderClear(gRenderer);
//
//	return 0;
//}
//
//int lua_setDrawColor(lua_State *L)
//{
//
//	int red = lua_tointeger(L, 1);
//	int green = lua_tointeger(L, 2);
//	int blue = lua_tointeger(L, 3);
//	int alpha = lua_tointeger(L, 4);
//	
//	SDL_SetRenderDrawColor(gRenderer, red, green, blue, alpha);
//	return 0;
//}
//
//// mo.graphics.drawLine(100, 100, 200, 200)
//int lua_drawLine(lua_State *L)
//{
//	// Check the number of arguments
//	if (lua_gettop(L) != 4) {
//		lua_pushstring(L, "Invalid number of arguments. Expected 4.");
//		lua_error(L);
//	}
//
//	// Get arguments from the stack
//	float x1 = luaL_checknumber(L, 1);
//	float y1 = luaL_checknumber(L, 2);
//	float x2 = luaL_checknumber(L, 3);
//	float y2 = luaL_checknumber(L, 4);
//
//	SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
//
//	return 0;
//}
//
//
//int lua_drawPolygon(lua_State *L) {
//	int numPoints = lua_gettop(L);  // Number of arguments
//	if (numPoints < 6 || numPoints % 2 != 0) {
//		luaL_error(L, "Invalid number of arguments. Must be pairs of coordinates.");
//	}
//
//	SDL_Point *points = new SDL_Point[numPoints / 2];
//	for (int i = 0; i < numPoints; i += 2) {
//		points[i / 2].x = luaL_checkinteger(L, i + 1);
//		points[i / 2].y = luaL_checkinteger(L, i + 2);
//	}
//
//	
//	SDL_RenderDrawLines(gRenderer, points, numPoints / 2);
//	// Optionally close the polygon by connecting the last point to the first
//	SDL_RenderDrawLine(gRenderer, points[numPoints / 2 - 1].x, points[numPoints / 2 - 1].y, points[0].x, points[0].y);
//
//	delete[] points;
//	return 0;
//}
//
//
//int lua_drawPoint(lua_State *L)
//{
//	int x = luaL_checkinteger(L, 1);
//	int y = luaL_checkinteger(L, 2);
//
//	SDL_RenderDrawPoint(gRenderer, x, y);
//
//	return 0;
//
//}
//
//
//int lua_drawPoints(lua_State *L)
//{
//	if (!gRenderer) {
//		lua_pushstring(L, "Renderer not initialized.");
//		lua_error(L);
//	}
//
//	// Check arguments
//	if (!lua_istable(L, 1))
//	{
//		lua_pushstring(L, "Expected a table of points.");
//		lua_error(L);
//	}
//
//	// Get number of points
//	int numPoints = luaL_len(L, 1);
//	std::vector<SDL_Point> points(numPoints);
//
//	// Retrieve points from the Lua table
//	for (int i = 1; i <= numPoints; ++i) {
//		lua_pushinteger(L, i);
//		lua_gettable(L, 1); // Get the i-th element from the table
//
//		if (lua_istable(L, -1)) {
//			lua_pushinteger(L, 1);
//			lua_gettable(L, -2);
//			points[i - 1].x = lua_tointeger(L, -1);
//			lua_pop(L, 1);
//
//			lua_pushinteger(L, 2);
//			lua_gettable(L, -2);
//			points[i - 1].y = lua_tointeger(L, -1);
//			lua_pop(L, 1);
//		}
//
//		lua_pop(L, 1); // Pop the table
//	}
//
//	
//	// Draw all points
//	SDL_RenderDrawPoints(gRenderer, points.data(), points.size());
//
//	return 0;
//}
//
//
//int lua_getWindowDimentions(lua_State *L)
//{
//	if (!gWindow)
//	{
//		lua_pushstring(L, "Window not initialized.");
//		lua_error(L);
//	}
//
//	int width, height;
//	SDL_GetWindowSize(gWindow, &width, &height);
//
//	lua_pushinteger(L, width);
//	lua_pushinteger(L, height);
//
//	return 2; // Return two values: width and height
//}
//
//
//int lua_loadImage(lua_State *L)
//{
//	const char *path = luaL_checkstring(L, 1);
//
//	SDL_Surface *surface = IMG_Load(path);
//	if (!surface) {
//		lua_pushnil(L);
//		lua_pushstring(L, IMG_GetError());
//		return 2;
//	}
//
//	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
//	SDL_FreeSurface(surface);
//
//	if (!texture) {
//		lua_pushnil(L);
//		lua_pushstring(L, SDL_GetError());
//		return 2;
//	}
//
//	int imageId = gNextImageId++;
//	gImages[imageId] = texture;
//	lua_pushinteger(L, imageId);
//
//	return 1;
//}
//
//
//int lua_drawImage(lua_State *L) {
//	int imageId = luaL_checkinteger(L, 1);
//	int x = luaL_checkinteger(L, 2);
//	int y = luaL_checkinteger(L, 3);
//
//	auto it = gImages.find(imageId);
//	if (it == gImages.end()) {
//		lua_pushboolean(L, 0);
//		lua_pushstring(L, "Image not found");
//		return 2;
//	}
//
//	SDL_Texture *texture = it->second;
//	SDL_Rect dstRect = { x, y, 0, 0 };
//	SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
//
//	SDL_RenderCopy(gRenderer, texture, nullptr, &dstRect);
//	lua_pushboolean(L, 1);
//
//	return 1;
//}
//
//int lua_newQuad(lua_State *L) {
//	int x = luaL_checkinteger(L, 1);
//	int y = luaL_checkinteger(L, 2);
//	int width = luaL_checkinteger(L, 3);
//	int height = luaL_checkinteger(L, 4);
//	
//
//	// Create and store the SDL_Rect for the quad
//	SDL_Rect quad = { x, y, width, height };
//	int quadId = gNextQuadId++;
//	gQuads[quadId] = quad;
//
//	lua_pushinteger(L, quadId);
//	return 1;
//}
//
//// Function to draw texture (entire or partial) using a quad
//// Function to draw texture (entire or partial) using a quad
//int lua_draw(lua_State *L) {
//	if (!gRenderer) {
//		lua_pushstring(L, "Renderer not initialized.");
//		lua_error(L);
//	}
//
//	// Ensure at least 3 arguments (texture, x, y)
//	if (lua_gettop(L) < 3) {
//		lua_pushstring(L, "Not enough arguments.");
//		lua_error(L);
//	}
//
//	// Check if the first argument is a texture
//	int textureId = luaL_checkinteger(L, 1);
//	auto imgIt = gImages.find(textureId);
//	if (imgIt == gImages.end()) {
//		lua_pushboolean(L, 0);
//		lua_pushstring(L, "Texture not found");
//		return 2;
//	}
//	SDL_Texture *texture = imgIt->second;
//
//	int x = luaL_checkinteger(L, 2);
//	int y = luaL_checkinteger(L, 3);
//
//	// Default values
//	double angle = 0.0;
//	double scaleX = 1.0;
//	double scaleY = 1.0;
//	SDL_Rect srcRect = { 0, 0, 0, 0 };
//
//	// Check if the fourth argument is a quad ID or a table
//	if (lua_isnumber(L, 4)) {
//		// Fourth argument is a quad ID
//		int quadId = luaL_checkinteger(L, 4);
//		auto quadIt = gQuads.find(quadId);
//		if (quadIt != gQuads.end()) {
//			srcRect = quadIt->second;
//		}
//		else {
//			lua_pushboolean(L, 0);
//			lua_pushstring(L, "Quad not found");
//			return 2;
//		}
//
//		// Handle remaining optional parameters (angle, scaleX, scaleY)
//		if (lua_gettop(L) >= 5) angle = luaL_optnumber(L, 5, angle);
//		if (lua_gettop(L) >= 6) scaleX = luaL_optnumber(L, 6, scaleX);
//		if (lua_gettop(L) >= 7) scaleY = luaL_optnumber(L, 7, scaleY);
//
//	}
//	else if (lua_istable(L, 4)) {
//		// Fourth argument is a table (quad definition)
//		lua_pushstring(L, "x");
//		lua_gettable(L, 4); // Get x from table
//		int quadX = luaL_checkinteger(L, -1);
//		lua_pop(L, 1);
//
//		lua_pushstring(L, "y");
//		lua_gettable(L, 4); // Get y from table
//		int quadY = luaL_checkinteger(L, -1);
//		lua_pop(L, 1);
//
//		lua_pushstring(L, "w");
//		lua_gettable(L, 4); // Get width from table
//		int width = luaL_checkinteger(L, -1);
//		lua_pop(L, 1);
//
//		lua_pushstring(L, "h");
//		lua_gettable(L, 4); // Get height from table
//		int height = luaL_checkinteger(L, -1);
//		lua_pop(L, 1);
//
//		srcRect = { quadX, quadY, width, height };
//
//		// Handle remaining optional parameters (angle, scaleX, scaleY)
//		if (lua_gettop(L) >= 5) angle = luaL_optnumber(L, 5, angle);
//		if (lua_gettop(L) >= 6) scaleX = luaL_optnumber(L, 6, scaleX);
//		if (lua_gettop(L) >= 7) scaleY = luaL_optnumber(L, 7, scaleY);
//
//	}
//	else {
//		// No quad ID provided; default to full texture
//		if (lua_gettop(L) >= 4) angle = luaL_optnumber(L, 4, angle);
//		if (lua_gettop(L) >= 5) scaleX = luaL_optnumber(L, 5, scaleX);
//		if (lua_gettop(L) >= 6) scaleY = luaL_optnumber(L, 6, scaleY);
//
//		// Query texture size if no quad is used
//		SDL_QueryTexture(texture, nullptr, nullptr, &srcRect.w, &srcRect.h);
//	}
//
//	SDL_Rect dstRect = { x, y, static_cast<int>(srcRect.w * scaleX), static_cast<int>(srcRect.h * scaleY) };
//	SDL_Point center = { dstRect.w / 2, dstRect.h / 2 }; // Center for rotation
//
//	SDL_RenderCopyEx(gRenderer, texture, &srcRect, &dstRect, angle, &center, SDL_FLIP_NONE);
//
//	return 0;
//}
//
//
//
//int lua_renderQuad(lua_State *L) {
//	// Retrieve arguments from Lua
//	int imageId = luaL_checkinteger(L, 1);
//	int quadId = luaL_checkinteger(L, 2);
//	int x = luaL_checkinteger(L, 3);
//	int y = luaL_checkinteger(L, 4);
//
//
//	// Find the texture and quad in the global maps
//	auto imgIt = gImages.find(imageId);
//	auto quadIt = gQuads.find(quadId);
//
//	if (imgIt == gImages.end() || quadIt == gQuads.end()) {
//		// Push an error message to Lua and return
//		lua_pushboolean(L, 0);
//		lua_pushstring(L, "Texture or Quad not found");
//		return 2;
//	}
//
//	SDL_Texture *texture = imgIt->second;
//	SDL_Rect srcRect = quadIt->second;
//	SDL_Rect dstRect = { x, y, srcRect.w, srcRect.h };
//
//	// Perform the rendering
//	SDL_RenderCopy(gRenderer, texture, &srcRect, &dstRect);
//
//	// Indicate success
//	lua_pushboolean(L, 1);
//	return 1;  // Return a success indicator
//}
//
//// Function to draw points with a custom size
//int lua_drawPointsWithSize(lua_State *L) {
//	if (!gRenderer) {
//		lua_pushstring(L, "Renderer not initialized.");
//		lua_error(L);
//	}
//
//	// Check arguments
//	if (!lua_istable(L, 1)) {
//		lua_pushstring(L, "Expected a table of points.");
//		lua_error(L);
//	}
//
//	// Get the point size
//	int pointSize = luaL_checkinteger(L, 2);
//
//	// Get number of points
//	int numPoints = luaL_len(L, 1);
//	std::vector<SDL_Rect> rects(numPoints);
//
//	// Retrieve points from the Lua table
//	for (int i = 1; i <= numPoints; ++i) {
//		lua_pushinteger(L, i);
//		lua_gettable(L, 1); // Get the i-th element from the table
//
//		if (lua_istable(L, -1)) {
//			lua_pushinteger(L, 1);
//			lua_gettable(L, -2);
//			int x = lua_tointeger(L, -1);
//			lua_pop(L, 1);
//
//			lua_pushinteger(L, 2);
//			lua_gettable(L, -2);
//			int y = lua_tointeger(L, -1);
//			lua_pop(L, 1);
//
//			rects[i - 1] = { x - pointSize / 2, y - pointSize / 2, pointSize, pointSize };
//		}
//
//		lua_pop(L, 1); // Pop the table
//	}
//
//	
//
//	// Draw all rectangles
//	for (const auto &rect : rects) {
//		SDL_RenderFillRect(gRenderer, &rect);
//	}
//
//	return 0;
//}
//
//// Resources https://github.com/jairomer/embedded-lua-cpp/blob/master/src/main.cpp
//// https://www.youtube.com/watch?v=xrLQ0OXfjaI&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk&index=1
//
//extern "C" int luaopen_graphics(lua_State * L)
//{
//	static const luaL_Reg graphicsLib[] = {
//		{"rectangle", lua_drawRectangle},
//		{"setBackgroundColor", lua_setBackgroundColor},
//		{"newImage", lua_loadImage},
//		{"draw", lua_draw},
//		{"drawQuad", lua_renderQuad},
//		{"newQuad", lua_newQuad},
//		{"drawLine", lua_drawLine},
//		{"setDrawColor", lua_setDrawColor},
//		{"points", lua_drawPoints},
//		{"getWindowDimensions", lua_getWindowDimentions},
//		{"drawPointsWithSize", lua_drawPointsWithSize},
//		{"drawPolygon", lua_drawPolygon},
//		{"point", lua_drawPoint},
//		{NULL, NULL}
//	};
//
//
//	luaL_newlib(L, graphicsLib);
//	return 1;
//}
