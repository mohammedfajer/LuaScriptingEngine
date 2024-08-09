#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>


extern "C"
{
	#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>


}

// NOTE(MO) it worked when I changed c++ version to 17.
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <vector>

#include "audio.h"

// Global pointer to the SFML window
sf::RenderWindow *g_window = nullptr;
sf::Color backgroundColor(sf::Color::Black); // Default color
// Font to use for rendering text
sf::Font font;
// Load the font
bool load_font(const std::string &font_path) {
	return font.loadFromFile(font_path);
}


#pragma region Graphics Module

#include <unordered_map>
#include <memory>

// Function to convert Lua table to sf::Color
sf::Color lua_to_color(const sol::table &color_table) {
	uint8_t r = color_table.get_or("r", 255);
	uint8_t g = color_table.get_or("g", 255);
	uint8_t b = color_table.get_or("b", 255);
	uint8_t a = color_table.get_or("a", 255);
	return sf::Color(r, g, b, a);
}

// Container to hold loaded textures
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

// Function to load a texture from a file
std::shared_ptr<sf::Texture> load_texture(const std::string &file_path) {
	auto texture = std::make_shared<sf::Texture>();
	if (texture->loadFromFile(file_path)) {
		textures[file_path] = texture;
		return texture;
	}
	std::cerr << "Failed to load texture: " << file_path << std::endl;
	return nullptr;
}

// Function to get a texture by file path
std::shared_ptr<sf::Texture> get_texture(const std::string &file_path) {
	auto it = textures.find(file_path);
	if (it != textures.end()) {
		return it->second;
	}
	return load_texture(file_path);
}

// Dummy module namespace
namespace graphics {


	enum class AlignMode
	{
		center,
		left,
		right,
		justify
	};

	void draw_rectangle(float x, float y, float width, float height, sol::optional<sol::table> color_table = sol::nullopt) {
		if (g_window == nullptr) return; // Ensure g_window is valid

		sf::Color color = color_table ? lua_to_color(*color_table) : sf::Color::White;

		sf::RectangleShape rectangle(sf::Vector2f(width, height));
		rectangle.setPosition(x, y);
		rectangle.setFillColor(color);
		g_window->draw(rectangle);
	}

	void draw_circle(float x, float y, float radius, sol::optional<sol::table> color_table = sol::nullopt) {
		if (g_window == nullptr) return; // Ensure g_window is valid

		sf::Color color = color_table ? lua_to_color(*color_table) : sf::Color::White;

		sf::CircleShape circle(radius);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		g_window->draw(circle);
	}

	void draw(sf::Texture *texture, float x, float y,
		sol::optional<float> rotation = sol::nullopt,
		sol::optional<float> scale_x = sol::nullopt,
		sol::optional<float> scale_y = sol::nullopt,
		sol::optional<float> origin_x = sol::nullopt,
		sol::optional<float> origin_y = sol::nullopt)
	{
		if (g_window == nullptr || texture == nullptr) return;

		sf::Sprite sprite;
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);

		if (rotation) sprite.setRotation(*rotation);
		if (scale_x && scale_y) sprite.setScale(*scale_x, *scale_y);
		if (origin_x && origin_y) sprite.setOrigin(*origin_x, *origin_y);

		g_window->draw(sprite);
	}

	void print(const std::string &text, float x, float y, unsigned int size = 30, const sol::table &color_table = sol::nil) {
		if (g_window == nullptr) return; // Ensure g_window is valid

		sf::Text sf_text(text, font, size);
		sf_text.setPosition(x, y);
		if (color_table.valid()) {
			sf::Color color = lua_to_color(color_table);
			sf_text.setFillColor(color);
		}
		else {
			sf_text.setFillColor(sf::Color::White);
		}

		g_window->draw(sf_text);
	}

	void printf(const std::string &text, float x, float y, float wrapWidth = 0.0f, unsigned int size = 30, const sol::table &color_table = sol::nil) {
		if (g_window == nullptr) return; // Ensure g_window is valid

		sf::Text sf_text(text, font, size);
		sf_text.setPosition(x, y);
		if (color_table.valid()) {
			sf::Color color = lua_to_color(color_table);
			sf_text.setFillColor(color);
		}
		else {
			sf_text.setFillColor(sf::Color::White);
		}

		if (wrapWidth > 0.0f) {
			// For simplicity, we'll just use the full string without wrapping.
			// SFML does not support text wrapping natively.
		}

		g_window->draw(sf_text);
	}

	void set_background_color(sol::table color_table) {
		backgroundColor = lua_to_color(color_table);
	}
}


// Binding without lambdas, assuming default values are handled in the function
void bind_graphics_module(sol::state &lua) {
	sol::table graphics = lua.create_table();
	graphics.set_function("draw_rectangle", graphics::draw_rectangle);
	graphics.set_function("draw_circle", graphics::draw_circle);
	graphics.set_function("set_background_color", graphics::set_background_color);

	// Expose the draw function
	graphics.set_function("draw", graphics::draw);

	// Expose newImage to load textures
	// Capture the lua state in the lambda
	graphics.set_function("newImage", [&lua](const std::string &file_path) -> sol::object {
		auto texture = get_texture(file_path);
		if (texture) {
			// Return the texture as a userdata
			return sol::make_object(lua, texture.get());
		}
		return sol::nil;
	});


	// Expose the print function
	graphics.set_function("print", graphics::print);

	// Expose the printf function
	graphics.set_function("printf", graphics::printf);

	lua["mo"]["graphics"] = graphics;
}



#pragma endregion


#pragma region Window Module


struct Window_Settings {
	bool fullscreen = false;
	bool resizable = false;
	bool vsync = false;

	int width = 800;
	int height = 600;

	std::string title = "SFML Window";

	sf::ContextSettings sfmlSettings;
};

Window_Settings gWindowSettings;


// Function to create the window initially
void create_window(int width, int height) {

	sf::Uint32 style = gWindowSettings.fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
	if (gWindowSettings.resizable) {
		style |= sf::Style::Resize;
	}

	gWindowSettings.sfmlSettings.antialiasingLevel = 8; // Example for anti-aliasing

	g_window = new sf::RenderWindow(sf::VideoMode(width, height), gWindowSettings.title.c_str(), style, gWindowSettings.sfmlSettings);
	g_window->setVerticalSyncEnabled(gWindowSettings.vsync);
}

void set_title(const std::string &title)
{
	gWindowSettings.title = title;
}

// Function to update window settings
// Function to update window settings based on Lua table
void set_window_mode(int width, int height, sol::table settings) {


		bool fullscreen = settings.get_or("fullscreen", false);
		bool resizable = settings.get_or("resizable", false);
		bool vsync = settings.get_or("vsync", true);

		gWindowSettings.fullscreen = fullscreen;
		gWindowSettings.resizable = resizable;
		gWindowSettings.vsync = vsync;
		gWindowSettings.width = width;
		gWindowSettings.height = height;

}

void bind_window_module(sol::state &lua) {
	sol::table window = lua.create_table();

	// Expose the set_window_mode function
	  // Expose the set_window_mode function
	window.set_function("setMode", [](int width, int height, sol::table settings) {
		set_window_mode(width, height, settings);
	});

	window.set_function("setTitle", set_title);

	lua["mo"]["window"] = window;
}

#pragma endregion
#include <sstream>
#include <vector>
#include <string>
sf::Text createWrappedText(const sf::Font &font, const std::string &text, unsigned int characterSize, float width) {
	// Create a text object to measure text size
	sf::Text tempText(text, font, characterSize);
	tempText.setCharacterSize(characterSize);

	// Split the text into words and wrap it
	std::stringstream ss(text);
	std::string word;
	std::string line;
	std::vector<std::string> lines;

	while (ss >> word) {
		std::string testLine = line.empty() ? word : line + " " + word;
		tempText.setString(testLine);

		sf::FloatRect textRect = tempText.getLocalBounds();
		// Use the width of the text to determine if it fits within the given width
		if (textRect.width > width) {
			// If it doesn't fit, save the current line and start a new one
			lines.push_back(line);
			line = word;
		}
		else {
			// Otherwise, continue adding words to the current line
			line = testLine;
		}
	}

	// Add the last line
	if (!line.empty()) {
		lines.push_back(line);
	}

	// Create a final text object to hold the wrapped text
	sf::Text wrappedText;
	wrappedText.setFont(font);
	wrappedText.setCharacterSize(characterSize);
	wrappedText.setString("");

	std::string wrappedTextStr;
	for (const std::string &l : lines) {
		wrappedTextStr += l + "\n";
	}

	wrappedText.setString(wrappedTextStr);
	return wrappedText;
}


#pragma region Keyboard

std::unordered_map<std::string, sf::Keyboard::Key> keyMap = {
	{"A", sf::Keyboard::A},
	{"B", sf::Keyboard::B},
	{"C", sf::Keyboard::C},
	{"D", sf::Keyboard::D},
	// Add other keys as needed
	{"Enter", sf::Keyboard::Enter},
	{"Space", sf::Keyboard::Space},
	{"Escape", sf::Keyboard::Escape},
	{"escape", sf::Keyboard::Escape}
	// You can add all keys from sf::Keyboard::Key
};

void handleKeyPressed(const sf::Event &event, sol::state &lua) {
	auto it = std::find_if(keyMap.begin(), keyMap.end(),
		[&](const auto &pair) { return pair.second == event.key.code; });

	if (it != keyMap.end()) {
		lua["mo"]["keypressed"](it->first);
	}
}

#pragma endregion


#pragma region Events

void event_quit() {
	if (!g_window) return;
	g_window->close();
}

void bind_event_module(sol::state &lua) {
	sol::table event = lua.create_table();

	// Expose the set_window_mode function
	  // Expose the set_window_mode function
	event.set_function("quit", &event_quit);

	lua["mo"]["event"] = event;
}


#pragma endregion






int main() {
	
	



	//g_window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML with Lua");

	sf::Clock clock;

	// Load the font
	if (!load_font("./data/arial.ttf")) {
		std::cerr << "Failed to load font." << std::endl;
		return -1;
	}

	sf::Text text("Hello Pong!", font, 30);

	// For left alignment
	//text.setOrigin(0, text.getLocalBounds().height / 2);
	//text.setPosition(100, 300);

	// For center alignment
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(1280/2.0, 720/2.0 - 6);

	//// For right alignment
	//text.setOrigin(text.getLocalBounds().width, text.getLocalBounds().height / 2);
	//text.setPosition(100, 300);

	//std::string text = "This is an example of text wrapping in SFML. The text will be split into lines based on the given width.";
	//float wrapWidth = 700.0f;
	//sf::Text wrappedText = createWrappedText(font, text, 30, wrapWidth);
	//wrappedText.setPosition(50, 50);

	sol::state lua;
	lua.open_libraries(sol::lib::base);


	// Create the global `mo` table
	lua["mo"] = lua.create_table();

	//lua["mo"]["keypressed"] = [](const std::string &key) {
	//	// This function can be used in Lua to handle key presses
	//	};

	
	// Bind graphics functions to Lua state
	bind_graphics_module(lua);
	// Bind window functions to Lua state
	bind_window_module(lua);
	bind_event_module(lua);
	bind_audio_module(lua);



	
	// Load and execute Lua script from file
	//sol::protected_function_result result = lua.script_file("script.lua");

	//if (!result.valid()) {
	//	sol::error err = result;
	//	std::cerr << "Lua error: " << err.what() << std::endl;
	//	return -1;
	//}

	// Load and run your Lua script
	try {
		lua.script_file("script.lua");
	}
	catch (const sol::error &e) {
		std::cerr << "Lua script error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown error occurred during Lua script execution" << std::endl;
	}

	

	// Call Lua functions defined in the script
	sol::function load_function = lua["mo"]["load"];
	if (load_function.valid()) {
		load_function();  // Call the Lua-defined function
	}
	else {
		std::cerr << "Error: 'mo.load' is not defined in Lua" << std::endl;
	}

	create_window(gWindowSettings.width, gWindowSettings.height);


	// Main loop
	while (g_window->isOpen()) {
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();

		// Event handling
		sf::Event event;
		while (g_window->pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				handleKeyPressed(event, lua);
			}
			if (event.type == sf::Event::Closed) {
				g_window->close();
			}
		}

		// Call `mo.update` with deltaTime
		sol::function update_function = lua["mo"]["update"];
		if (update_function.valid()) {
			update_function(deltaTime);
		}
		else {
			std::cerr << "Error: 'mo.update' is not defined in Lua" << std::endl;
		}
		
		// Clear the window
		g_window->clear(backgroundColor);

		// Call 'mo.draw'
		sol::function draw_function = lua["mo"]["draw"];
		if (draw_function.valid()) {
			//draw_function();
		}
		else {
			std::cerr << "Error: 'mo.draw' is not defined in Lua" << std::endl;
		}

		g_window->draw(text);

		// Display the contents
		g_window->display();
	}
	return 0;
}
