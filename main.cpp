#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// NOTE(MO) it worked when I changed c++ version to 17.
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <vector>
#include "audio.h"
#include <unordered_map>
#include <memory>
#include "core.h"


// Struct to encapsulate font and its size
struct FontData {
	std::shared_ptr<sf::Font> font;
	int size;

	FontData() : font(nullptr), size(0) {}

	FontData(std::shared_ptr<sf::Font> f, int s) : font(f), size(s) {}
};

struct GlobalContext {
	sf::RenderWindow window;
	sf::Color backgroundColor = sf::Color::Black;
	FontData activeFont;

	GlobalContext() : activeFont(nullptr, 0) {}
};

global_variable GlobalContext gContext;



// Load the font
bool loadFont(const std::string &fontpath) {
	// Ensure the font is initialized
	if (gContext.activeFont.font) {
		return gContext.activeFont.font->loadFromFile(fontpath);
	}
	return false;  // Return false if the font pointer is not initialized
}


#pragma region Graphics Module


// Function to convert Lua table to sf::Color
sf::Color luaToColor(const sol::table &colorTable) {
	uint8_t r = colorTable.get_or("r", 255);
	uint8_t g = colorTable.get_or("g", 255);
	uint8_t b = colorTable.get_or("b", 255);
	uint8_t a = colorTable.get_or("a", 255);
	return sf::Color(r, g, b, a);
}

// Container to hold loaded textures
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

// Function to load a texture from a file
std::shared_ptr<sf::Texture> loadTexture(const std::string &filepath) {
	auto texture = std::make_shared<sf::Texture>();
	if (texture->loadFromFile(filepath)) {
		textures[filepath] = texture;
		return texture;
	}
	std::cerr << "Failed to load texture: " << filepath << std::endl;
	return nullptr;
}

// Function to get a texture by file path
std::shared_ptr<sf::Texture> getTexture(const std::string &filepath) {
	auto it = textures.find(filepath);
	if (it != textures.end()) {
		return it->second;
	}
	return loadTexture(filepath);
}

// Dummy module namespace
namespace graphics {

	// NOTE(MO): not used yet!
	enum class AlignMode {
		center,
		left,
		right,
		justify
	};

	
	// Create and return a FontData object
	FontData newFont(const std::string &fontpath, int size) {

		std::cout << "newFont called from lua.\n";

		auto font = std::make_shared<sf::Font>();
		if (font->loadFromFile(fontpath)) {
			return FontData(font, size);
		}
		return FontData(nullptr, 0);  // Return invalid FontData if loading fails
	}


	// Set the active font in the global context
	
	void setActiveFont(const FontData &fontData) {
		std::cout << "setActiveFont called from lua.\n";

		if (fontData.font) {
			gContext.activeFont = fontData;  // Directly set FontData
		}
		else {
			gContext.activeFont = FontData(nullptr, 0);  // Handle invalid FontData
		}
	}

	// Draw text using the active font
	
	void drawText(const std::string &text, float x, float y) {
		std::cout << "drawText called from lua.\n";

		if (gContext.activeFont.font) {
			sf::Text sfText;
			sfText.setFont(*gContext.activeFont.font);
			sfText.setString(text);
			sfText.setCharacterSize(gContext.activeFont.size);
			sfText.setPosition(x, y);
			sfText.setFillColor(sf::Color::White);

			if (gContext.window.isOpen()) {
				gContext.window.draw(sfText);
			}
			else {
				std::cerr << "Error: Render window is not open.\n";
			}
		}
		else {
			std::cerr << "Error: No valid font set.\n";
		}
	}





	void drawRectangle(f32 x, f32 y, f32 width, f32 height, sol::optional<sol::table> colorTable = sol::nullopt) {
		if (!gContext.window.isOpen()) return;
		sf::Color color = colorTable ? luaToColor(*colorTable) : sf::Color::White;
		sf::RectangleShape rectangle(sf::Vector2f(width, height));
		rectangle.setPosition(x, y);
		rectangle.setFillColor(color);
		gContext.window.draw(rectangle);
	}

	void drawCircle(f32 x, f32 y, f32 radius, sol::optional<sol::table> colorTable = sol::nullopt) {
		if (!gContext.window.isOpen()) return;
		sf::Color color = colorTable ? luaToColor(*colorTable) : sf::Color::White;
		sf::CircleShape circle(radius);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		gContext.window.draw(circle);
	}

	void draw(sf::Texture *texture, f32 x, f32 y,
		sol::optional<float> rotation = sol::nullopt,
		sol::optional<float> scaleX = sol::nullopt,
		sol::optional<float> scaleY = sol::nullopt,
		sol::optional<float> originX = sol::nullopt,
		sol::optional<float> originY = sol::nullopt) {
		if (!gContext.window.isOpen() || texture == nullptr) return;
		sf::Sprite sprite;
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
		if (rotation) sprite.setRotation(*rotation);
		if (scaleX && scaleY) sprite.setScale(*scaleX, *scaleY);
		if (originX && originY) sprite.setOrigin(*originX, *originY);
		gContext.window.draw(sprite);
	}

	void print(const std::string &text, f32 x, f32 y,
		u32 size = 30, const sol::table &colorTable = sol::nil) {
		if(!gContext.window.isOpen()) return;
		auto &font = *gContext.activeFont.font;
		sf::Text sfText(text, font, size);
		sfText.setPosition(x, y);
		if (colorTable.valid()) {
			sf::Color color = luaToColor(colorTable);
			sfText.setFillColor(color);
		}
		else {
			sfText.setFillColor(sf::Color::White);
		}
		gContext.window.draw(sfText);
	}

	void printf(const std::string &text, f32 x, f32 y, f32 wrapWidth = 0.0f,
		u32 size = 30, const sol::table &colorTable = sol::nil) {
		if (!gContext.window.isOpen()) return;
		auto &font = *gContext.activeFont.font;
		sf::Text sfText(text, font, size);
		sfText.setPosition(x, y);
		if (colorTable.valid()) {
			sf::Color color = luaToColor(colorTable);
			sfText.setFillColor(color);
		}
		else {
			sfText.setFillColor(sf::Color::White);
		}

		if (wrapWidth > 0.0f) {
			// For simplicity, we'll just use the full string without wrapping.
			// SFML does not support text wrapping natively.
		}

		//g_window->draw(sf_text);
		gContext.window.draw(sfText);
	}

	void setBackgroundColor(sol::table colorTable) {
		gContext.backgroundColor = luaToColor(colorTable);
	}
}


// Binding without lambdas, assuming default values are handled in the function
void bindGraphicsModule(sol::state &lua) {
	sol::table graphics = lua.create_table();
	graphics.set_function("rectangle", graphics::drawRectangle);
	graphics.set_function("circle", graphics::drawCircle);
	graphics.set_function("setBackgroundColor", graphics::setBackgroundColor);

	// Expose the draw function
	graphics.set_function("draw", graphics::draw);

	// Expose newImage to load textures
	// Capture the lua state in the lambda
	graphics.set_function("newImage", [&lua](const std::string &filepath) -> sol::object {
		auto texture = getTexture(filepath);
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

	// FONTS HERE!
	// Expose FontData to Lua
	//lua.new_usertype<FontData>("FontData",
	//	sol::constructors<FontData(), FontData(std::shared_ptr<sf::Font>, int)>(),
	//	"font", &FontData::font,
	//	"size", &FontData::size
	//);

	//// Bind functions
	//graphics.set_function("newFont", [](const std::string &fontpath, int size) -> std::shared_ptr<FontData> {
	//	return std::make_shared<FontData>(graphics::newFont(fontpath, size));
	//	});

	//graphics.set_function("setFont", graphics::setActiveFont);
	//graphics.set_function("drawText", graphics::drawText);

	lua["mo"]["graphics"] = graphics;
}

#pragma endregion

#pragma region Window Module

struct WindowSettings {
	bool fullscreen = false;
	bool resizable = false;
	bool vsync = false;

	i32 width = 800;
	i32 height = 600;

	std::string title = "SFML Window";

	sf::ContextSettings sfmlSettings;
};

global_variable WindowSettings gWindowSettings;


// Function to create the window initially
void createWindow(i32 width, i32 height) {

	sf::Uint32 style = gWindowSettings.fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
	if (gWindowSettings.resizable) {
		style |= sf::Style::Resize;
	}

	gWindowSettings.sfmlSettings.antialiasingLevel = 8; // Example for anti-aliasing
	gContext.window.create(sf::VideoMode(width, height), gWindowSettings.title.c_str(), style, gWindowSettings.sfmlSettings);
	gContext.window.setVerticalSyncEnabled(gWindowSettings.vsync);
}

inline void setTitle(const std::string &title) { gWindowSettings.title = title; }

// Function to update window settings based on Lua table
void setWindowMode(int width, int height, sol::table settings) {
	bool fullscreen = settings.get_or("fullscreen", false);
	bool resizable = settings.get_or("resizable", false);
	bool vsync = settings.get_or("vsync", true);

	gWindowSettings.fullscreen = fullscreen;
	gWindowSettings.resizable = resizable;
	gWindowSettings.vsync = vsync;
	gWindowSettings.width = width;
	gWindowSettings.height = height;
}

void bindWindowModule(sol::state &lua) {
	sol::table window = lua.create_table();
	// Expose the set_window_mode function
	window.set_function("setMode", [](int width, int height, sol::table settings) {
		setWindowMode(width, height, settings);
	});
	window.set_function("setTitle", setTitle);
	lua["mo"]["window"] = window;
}

#pragma endregion
#include <sstream>
#include <vector>
#include <string>

sf::Text createWrappedText(const sf::Font &font, const std::string &text, u32 characterSize, f32 width) {
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
void eventQuit() {
	if (!gContext.window.isOpen()) return;
	gContext.window.close();
}

void bindEventModule(sol::state &lua) {
	sol::table event = lua.create_table();
	// Expose the set_window_mode function
	event.set_function("quit", &eventQuit);
	lua["mo"]["event"] = event;
}
#pragma endregion


int main() {
	sf::Clock clock;

	// Initialize the font in the global context
	gContext.activeFont.font = std::make_shared<sf::Font>();

	// Load the font
	if (!loadFont("./data/arial.ttf")) {
		std::cerr << "Failed to load font." << std::endl;
		return -1;
	}

	auto &font = *gContext.activeFont.font;

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

	// Bind graphics functions to Lua state
	bindGraphicsModule(lua);
	// Bind window functions to Lua state
	bindWindowModule(lua);
	bindEventModule(lua);

	bind_audio_module(lua);

	//Load and execute Lua script from file
	sol::protected_function_result result = lua.script_file("script.lua");

	if (!result.valid()) {
		sol::error err = result;
		std::cerr << "Lua error: " << err.what() << std::endl;
		return -1;
	}

	// Call Lua functions defined in the script
	sol::function load_function = lua["mo"]["load"];
	if (load_function.valid()) {
		load_function();  // Call the Lua-defined function
	}
	else {
		std::cerr << "Error: 'mo.load' is not defined in Lua" << std::endl;
	}

	createWindow(gWindowSettings.width, gWindowSettings.height);

	// Main loop
	while (gContext.window.isOpen()) {
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();

		// Event handling
		sf::Event event;
		while (gContext.window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				handleKeyPressed(event, lua);
			}
			if (event.type == sf::Event::Closed) {
				gContext.window.close();
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
		gContext.window.clear(gContext.backgroundColor);

		// Call 'mo.draw'
		sol::function draw_function = lua["mo"]["draw"];
		if (draw_function.valid()) {
			draw_function();
		}
		else {
			std::cerr << "Error: 'mo.draw' is not defined in Lua" << std::endl;
		}

		gContext.window.draw(text);

		// Display the contents
		gContext.window.display();
	}
	return 0;
}
