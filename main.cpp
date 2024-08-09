#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
extern "C"
{
#include <lua.hpp>
}


#include <vector>

sf::RenderWindow *g_window = nullptr;


int lua_drawAnything(lua_State *L) {
	if (g_window == nullptr) {
		return luaL_error(L, "RenderWindow is not initialized!");
	}

	sf::CircleShape shape(50);
	shape.setFillColor(sf::Color::Green);
	g_window->draw(shape);
	return 0; // Number of return values to Lua
}

// Create a new image from a file and return a reference to it
static int lua_newImage(lua_State *L) {
	const char *filename = luaL_checkstring(L, 1);
	sf::Texture *texture = new sf::Texture();
	if (!texture->loadFromFile(filename)) {
		delete texture;
		return luaL_error(L, "Failed to load image: %s", filename);
	}
	// Store texture in userdata
	sf::Texture **userdata = static_cast<sf::Texture **>(lua_newuserdata(L, sizeof(sf::Texture *)));
	*userdata = texture;
	luaL_getmetatable(L, "sfTexture");
	lua_setmetatable(L, -2);
	return 1;
}

// Draw an image onto the global SFML window
//static int lua_draw(lua_State *L) {
//	sf::Texture *texture = *static_cast<sf::Texture **>(luaL_checkudata(L, 1, "sfTexture"));
//	sf::Sprite sprite(*texture);
//	sprite.setPosition(static_cast<float>(luaL_checknumber(L, 2)), static_cast<float>(luaL_checknumber(L, 3)));
//
//	g_window->draw(sprite);
//	return 0;
//}

// Draw an image onto the global SFML window
static int lua_draw(lua_State *L) {
	// Check the type of the first argument and ensure it's userdata for sf::Texture
	sf::Texture *texture = *static_cast<sf::Texture **>(luaL_checkudata(L, 1, "sfTexture"));

	// Create a sprite from the texture
	sf::Sprite sprite(*texture);

	// Get position from the second and third arguments
	float x = static_cast<float>(luaL_checknumber(L, 2));
	float y = static_cast<float>(luaL_checknumber(L, 3));

	// Check if a fourth argument is provided (rotation angle)
	float rotation = 0.0f; // Default value
	if (lua_gettop(L) >= 4) {
		rotation = static_cast<float>(luaL_checknumber(L, 4));
	}

	float desiredWidth = texture->getSize().x;
	float desiredHeight = texture->getSize().y;

	if (lua_gettop(L) >= 6)
	{
		desiredWidth = static_cast<float>(luaL_checknumber(L, 5));
		desiredHeight = static_cast<float>(luaL_checknumber(L, 6));
	}

	// Get the original size of the texture
	sf::Vector2u textureSize = texture->getSize();

	// Calculate the scale factors
	float scaleX = desiredWidth / textureSize.x;
	float scaleY = desiredHeight / textureSize.y;

	// Set the sprite scale
	sprite.setScale(scaleX, scaleY);

	bool centerIt = false;
	if (lua_gettop(L) >= 7)
	{
		centerIt = static_cast<bool>( lua_toboolean(L, 7) );
	}

	if (centerIt)
	{
		// Set the origin to the center of the sprite
		sprite.setOrigin(desiredWidth / 2.0f, desiredHeight / 2.0f);
	}

	// Set sprite properties
	sprite.setPosition(x, y);
	sprite.setRotation(rotation);

	// Draw the sprite onto the global SFML window
	g_window->draw(sprite);
	return 0;
}

static const luaL_Reg graphicsLib[] = {
		{"drawSomething", lua_drawAnything },
		{"newImage", lua_newImage},
		{"draw", lua_draw},
		{NULL, NULL}
};


extern "C" int luaopen_graphics(lua_State * L)
{
	

	luaL_newlib(L, graphicsLib);
	
	return 1;
}

// Setup the metatables for SFML types
void registerGraphicsMetatables(lua_State *L) {
	luaL_newmetatable(L, "sfTexture");
	lua_pop(L, 1);
}

void callLuaFunction(lua_State *L, const char *tableName, const char *functionName, int numArgs, int numResults) {
	// 1. Push the table onto the stack
	lua_getglobal(L, tableName);  // Pushes `mytable` onto the stack

	// 2. Push the function onto the stack
	lua_getfield(L, -1, functionName); // Pushes `mytable[functionName]` onto the stack

	// 3. Check if the function exists and is callable
	if (!lua_isfunction(L, -1)) {
		std::cerr << "Function '" << functionName << "' is not found in table '" << tableName << "'" << std::endl;
		lua_pop(L, 2); // Pop function and table
		return;
	}

	// 4. Push arguments onto the stack
	// Note: This example assumes you have pushed arguments earlier

	// 5. Call the function
	if (lua_pcall(L, numArgs, numResults, 0) != LUA_OK) {
		std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // Pop error message
		return;
	}

	// 6. Retrieve the results
	for (int i = 0; i < numResults; ++i) {
		if (lua_isstring(L, -numResults + i)) {
			std::cout << "Result " << i + 1 << ": " << lua_tostring(L, -numResults + i) << std::endl;
		}
		else if (lua_isnumber(L, -numResults + i)) {
			std::cout << "Result " << i + 1 << ": " << lua_tonumber(L, -numResults + i) << std::endl;
		}
	}

	// 7. Clean up the stack
	lua_pop(L, numResults + 1); // Pop results and the function
}

int main(int argc, char **argv) {

	g_window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML Window");

	// Initialize Lua
	lua_State *L = luaL_newstate();
	luaL_openlibs(L); // Load standard libraries

	luaL_requiref(L, "graphics", luaopen_graphics, 1);
	lua_pop(L, 1); // Remove the module from the stack

	// Initialize the 'mo' table
	// Create a new table and set it as the global `mo` table
	lua_newtable(L);                    // Create a new table and push it onto the stack

	// Custom Libraries
	lua_getglobal(L, "graphics"); // Load the graphics library
	lua_setfield(L, -2, "graphics"); // Set it as the field of the `mo` table
	
	lua_setglobal(L, "mo");            // Set the table as a global variable 

	// Register SFML metatables
	registerGraphicsMetatables(L);

	// Load and execute the Lua script
	std::string scriptPath = "script.lua";
	if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK) {
		std::cerr << "Error loading script.lua: " << lua_tostring(L, -1) << std::endl;
		lua_close(L);
		return 1;
	}

	// SFML Setup
	sf::Clock clock;

	
	// Call `mo.load`

	callLuaFunction(L, "mo", "load", 0, 0);


	// Main loop
	while (g_window->isOpen()) {
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();

		// Event handling
		sf::Event event;
		while (g_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				g_window->close();
			}
		}

		// Call `mo.update` with deltaTime
		lua_getglobal(L, "mo");  

		// 2. Push the function onto the stack
		lua_getfield(L, -1, "update"); 

		// 3. Check if the function exists and is callable
		if (!lua_isfunction(L, -1)) {
			std::cerr << "mo.update does not exist\n";
			lua_pop(L, 2); // Pop function and table
			return -1;
		}

		lua_pushnumber(L, deltaTime);

		// 5. Call the function
		if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
			std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1); // Pop error message
			return -1;
		}

		// 7. Clean up the stack
		lua_pop(L, 1); // Pop results and the function



		

		// Clear the window
		g_window->clear();

		callLuaFunction(L, "mo", "draw", 0, 0);



		
	

		
		// Display the contents
		g_window->display();
	}

	// Clean up Lua
	lua_close(L);

	return 0;
}
