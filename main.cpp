
#pragma region Includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
// NOTE(MO) it worked when I changed c++ version to 17.
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include "core.h"
#include <sstream>
#include <vector>
#include <string>
#pragma endregion

struct Font {
	sf::Font font;
	u32 size;
	Font(u32 fontSize) : size(fontSize) {}
};

struct WindowSettings {
	bool fullscreen = false;
	bool resizable = false;
	bool vsync = false;
	i32 width = 800;
	i32 height = 600;
	std::string title = "SFML Window";
	sf::ContextSettings sfmlSettings;
};

struct GlobalContext {
	sf::RenderWindow window;
	sf::Color backgroundColor = sf::Color::Black;
	Font *activeFont = nullptr;
	WindowSettings settings;

	// Maybe this is a bad idea
	bool pushModuleUsed = false;
	sf::RenderTexture *canvas = nullptr;
};

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

	Font *newFont(const std::string &filepath, unsigned int fontSize) {
		std::cout << "Creating font with size, PATH = " << filepath << ", SIZE = " << fontSize << std::endl;
		Font *fontData = new Font(fontSize);
		if (fontData->font.loadFromFile(filepath)) {
			return fontData;
		}
		else {
			delete fontData;
			return nullptr;
		}
	}

	void setFont(Font *fontData, sol::state &lua) {
		if (!fontData) {
			std::cerr << "Font data is null from setFont()" << std::endl;
			return;
		}
		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
		if (!context) {
			std::cerr << "Global context is null" << std::endl;
			return;
		}
		context->activeFont = fontData;
	}

	// Dummy draw text Remove Later
	void drawText(const std::string &text, float x, float y, sol::state &lua) {

		std::cout << "drawText() called from lua.\n";

		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();

		if (context && context->activeFont) {
			sf::Text sfText;
			sfText.setFont(context->activeFont->font);
			sfText.setString(text);
			sfText.setCharacterSize(context->activeFont->size); // Set text size
			sfText.setFillColor(sf::Color::White); // Set text color
			sfText.setPosition(x, y);
			context->window.draw(sfText);
		}
		else {

			if (!context) std::cerr << "Error: globalContext not found in lua state.\n";
			else if( !context->activeFont)  std::cerr << "Error: activeFont not found in lua state.\n";

		}
	}

	void drawRectangle(f32 x, f32 y, f32 width, f32 height, sol::state &lua, sol::optional<sol::table> colorTable = sol::nullopt) {
		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
		if (!context || !context->window.isOpen()) return;
		sf::Color color = colorTable ? luaToColor(*colorTable) : sf::Color::White;
		sf::RectangleShape rectangle(sf::Vector2f(width, height));
		// Set origin to top-left corner
		rectangle.setOrigin(0, 0);

		rectangle.setPosition(x, y);

		rectangle.setFillColor(color);
		std::cout << "Drawing rectangle at (" << x << ", " << y << ") with size (" << width << ", " << height << ")\n";
		//context->window.draw(rectangle);
		context->canvas->draw(rectangle);
	}

	void drawCircle(f32 x, f32 y, f32 radius, sol::state &lua, sol::optional<sol::table> colorTable = sol::nullopt) {
		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
		if (!context || !context->window.isOpen()) return;
		sf::Color color = colorTable ? luaToColor(*colorTable) : sf::Color::White;
		sf::CircleShape circle(radius);
		circle.setPosition(x, y);
		circle.setFillColor(color);
		context->window.draw(circle);
	}

	void draw(sf::Texture *texture, f32 x, f32 y, sol::state &lua,
		sol::optional<float> rotation = sol::nullopt,
		sol::optional<float> scaleX = sol::nullopt,
		sol::optional<float> scaleY = sol::nullopt,
		sol::optional<float> originX = sol::nullopt,
		sol::optional<float> originY = sol::nullopt) {
		
		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
		if (!context || !context->window.isOpen() || texture == nullptr) return;
		sf::Sprite sprite;
		sprite.setTexture(*texture);
		sprite.setPosition(x, y);
		if (rotation) sprite.setRotation(*rotation);
		if (scaleX && scaleY) sprite.setScale(*scaleX, *scaleY);
		if (originX && originY) sprite.setOrigin(*originX, *originY);
		context->window.draw(sprite);
	}

	
	void setBackgroundColor(sol::table colorTable, sol::state &lua) {
		// Retrieve the globalContext from Lua
		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
		if (context) {
			context->backgroundColor = luaToColor(colorTable);
		}
	}
}

void bindGraphicsModule(sol::state &lua) {
	sol::table graphics = lua.create_table();
	graphics.set_function("rectangle", [&](f32 x, f32 y, f32 width, f32 height, sol::optional<sol::table> colorTable = sol::nullopt) {
		graphics::drawRectangle(x, y, width, height, lua, colorTable);
	});
	graphics.set_function("circle", [&](f32 x, f32 y, f32 radius, sol::optional<sol::table> colorTable = sol::nullopt) {
		graphics::drawCircle(x, y, radius, lua, colorTable);
	});
	graphics.set_function("setBackgroundColor", [&](sol::table colorTable) {
		graphics::setBackgroundColor(colorTable, lua);
	});

	// Expose the draw function
	graphics.set_function("draw", [&](sf::Texture *texture, f32 x, f32 y,
		sol::optional<float> rotation = sol::nullopt,
		sol::optional<float> scaleX = sol::nullopt,
		sol::optional<float> scaleY = sol::nullopt,
		sol::optional<float> originX = sol::nullopt,
		sol::optional<float> originY = sol::nullopt) {
			graphics::draw(texture, x, y, lua, rotation, scaleX, scaleY, originX, originY);
		});

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

	// Expose functions
	graphics.set_function("newFont", graphics::newFont);

	graphics.set_function("setFont", [&](Font *fontData) {
		graphics::setFont(fontData, lua);
	});
	graphics.set_function("drawText", [&](const std::string &text, float x, float y) {
		graphics::drawText(text, x, y, lua);
	});

	lua["mo"]["graphics"] = graphics;
}

#pragma endregion

#pragma region Window Module


// Example ResolutionManager class (adjust as needed)
//class ResolutionManager {
//public:
//	ResolutionManager(sf::RenderWindow &window)
//		: window(window), virtualWidth(800), virtualHeight(600), fullscreen(false) {}
//
//	void setupScreen(f32 virtualWidth, f32 virtualHeight, u32 windowWidth, u32 windowHeight, sol::table settings) {
//
//		std::cout << "SetupScreen (..) called from lua.\n";
//
//		this->virtualWidth = virtualWidth;
//		this->virtualHeight = virtualHeight;
//		this->fullscreen = settings["fullscreen"].get_or(false);
//
//		sf::VideoMode mode(windowWidth, windowHeight);
//		sf::Uint32 style = this->fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
//
//		try {
//			window.create(mode, "title", style);
//			window.setVerticalSyncEnabled(settings["vsync"].get_or(true));
//		}
//		catch (const std::exception &e) {
//			std::cerr << "Exception during window creation: " << e.what() << std::endl;
//		}
//		catch (...) {
//			std::cerr << "Unknown error during window creation." << std::endl;
//		}
//	
//
//		updateView();
//	}
//
//	void resize(unsigned int width, unsigned int height) {
//		window.setSize(sf::Vector2u(width, height));
//		updateView();
//	}
//
//	void start() {
//		std::cout << "Start from push module.\n";
//		window.clear(); // TODO need to get the background color from the context here?
//		window.setView(view);
//	}
//
//	void finish() {
//
//		std::cout << "Display from push module.\n";
//		window.display();
//	}
//
//private:
//	void updateView() {
//		float windowWidth = window.getSize().x;
//		float windowHeight = window.getSize().y;
//
//		float scaleX = windowWidth / virtualWidth;
//		float scaleY = windowHeight / virtualHeight;
//		float scale = std::min(scaleX, scaleY);
//
//		view.setSize(virtualWidth * scale, virtualHeight * scale);
//		view.setCenter(virtualWidth / 2.0f, virtualHeight / 2.0f);
//		window.setView(view); // Make sure the view is set to the window
//
//		// Debug info
//		std::cout << "View Size: (" << view.getSize().x << ", " << view.getSize().y << ")\n";
//		std::cout << "View Center: (" << view.getCenter().x << ", " << view.getCenter().y << ")\n";
//	}
//
//	sf::RenderWindow &window;
//	sf::View view;
//	float virtualWidth;
//	float virtualHeight;
//	bool fullscreen;
//};


class ResolutionManager {
public:
	ResolutionManager(sf::RenderWindow &window)
		: window(window), virtualWidth(800), virtualHeight(600), fullscreen(false) {
		// Initial setup
		setupRenderTexture();
	}

	void setupScreen(f32 virtualWidth, f32 virtualHeight,f32 windowWidth, f32 windowHeight, sol::table settings) {


		std::cout << "SetupScreen (..) called from lua.\n";

		this->virtualWidth = virtualWidth;
		this->virtualHeight = virtualHeight;
		this->fullscreen = settings["fullscreen"].get_or(false);

		sf::VideoMode mode(windowWidth, windowHeight);
		sf::Uint32 style = this->fullscreen ? sf::Style::Fullscreen : sf::Style::Close;

		try {
			window.create(mode, "title", style);
			window.setVerticalSyncEnabled(settings["vsync"].get_or(true));

			setupRenderTexture();
		}
		catch (const std::exception &e) {
			std::cerr << "Exception during window creation: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unknown error during window creation." << std::endl;
		}
	}

	void resize(u32 width, u32 height) {
		window.setSize(sf::Vector2u(width, height));
		updateSprite();
	}

	// NOTE(mo): Something below got commented out and fixed the issue or origin being top-left
	// later figure it out why!

	void start() {
		//window.clear(); // Clear with background color if needed
		//window.draw(sprite);

		renderTexture.clear(sf::Color::Black);
	}

	void finish() {
		//window.display();

		renderTexture.display();

		window.clear(sf::Color::Black);
		window.draw(sprite);
		window.display();
	}

	
	sf::RenderTexture renderTexture;
private:
	void setupRenderTexture() {
		// Create a render texture with the virtual resolution
		if (!renderTexture.create(virtualWidth, virtualHeight)) {
			std::cerr << "Error creating render texture." << std::endl;
			return;
		}

		// Create a sprite to display the render texture
		sprite.setTexture(renderTexture.getTexture());

		// Calculate scaling factors
		float scaleX = static_cast<float>(window.getSize().x) / virtualWidth;
		float scaleY = static_cast<float>(window.getSize().y) / virtualHeight;
		sprite.setScale(scaleX, scaleY);
	}

	void updateSprite() {
		// Update the sprite scaling to match the new window size
		float scaleX = static_cast<float>(window.getSize().x) / virtualWidth;
		float scaleY = static_cast<float>(window.getSize().y) / virtualHeight;
		sprite.setScale(scaleX, scaleY);
	}

	sf::RenderWindow &window;
	
	sf::Sprite sprite;
	float virtualWidth;
	float virtualHeight;
	bool fullscreen;
};


// Function to bind ResolutionManager to Lua
void bindResolutionManagerModulePush(sol::state &lua, sf::RenderWindow &window) {
	// Create a usertype for ResolutionManager and its methods
	lua.new_usertype<ResolutionManager>(
		"ResolutionManager",
		"setupScreen", &ResolutionManager::setupScreen,
		"resize", &ResolutionManager::resize,
		"start", &ResolutionManager::start,
		"finish", &ResolutionManager::finish
	);

	// Create an instance of ResolutionManager
	auto resolutionManager = std::make_shared<ResolutionManager>(window);

	// Bind the instance to the global `push` table
	lua["push"] = resolutionManager;


	GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
	if (!context) return;
	context->canvas = &resolutionManager->renderTexture;
}


bool checkModuleExist(const std::string &moduleName, sol::state& lua) {
	// Check if the module exists in the global namespace
	sol::optional<sol::table> module = lua[moduleName.c_str()];
	if (module) return true;
	return false;
}


// Function to create the window initially
void createWindow(GlobalContext &context) {
	auto &settings = context.settings;

	sf::Uint32 style = settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
	if (settings.resizable) {
		style |= sf::Style::Resize;
	}
	settings.sfmlSettings.antialiasingLevel = 8; // Example for anti-aliasing
	context.window.create(sf::VideoMode(settings.width, settings.height), settings.title.c_str(), style, settings.sfmlSettings);
	context.window.setVerticalSyncEnabled(settings.vsync);
}

inline void setTitle(const std::string &title, sol::state& lua) { 
	GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
	if (!context) return;
	context->settings.title = title;
}

// Function to update window settings based on Lua table
void setWindowMode(int width, int height, sol::table settings, sol::state &lua) {
	bool fullscreen = settings.get_or("fullscreen", false);
	bool resizable = settings.get_or("resizable", false);
	bool vsync = settings.get_or("vsync", true);

	GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
	if (!context) return;

	context->settings.fullscreen = fullscreen;
	context->settings.resizable = resizable;
	context->settings.vsync = vsync;
	context->settings.width = width;
	context->settings.height = height;
}

void bindWindowModule(sol::state &lua) {
	sol::table window = lua.create_table();
	// Expose the set_window_mode function
	window.set_function("setMode", [&](int width, int height, sol::table settings) {
		setWindowMode(width, height, settings, lua);
	});
	window.set_function("setTitle", [&](const std::string &title) {
		setTitle(title, lua);
	});
	lua["mo"]["window"] = window;
}

#pragma endregion

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
void eventQuit(sol::state &lua) {
	GlobalContext *context = lua["globalContext"].get<GlobalContext *>();
	if (!context || !context->window.isOpen()) return;
	context->window.close();
}

void bindEventModule(sol::state &lua) {
	sol::table event = lua.create_table();
	// Expose the set_window_mode function
	event.set_function("quit", [&]() {
		eventQuit(lua);
	});
	lua["mo"]["event"] = event;
}
#pragma endregion


int main() {
	sf::Clock clock;
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package); // sol::lib::package i think its used to give us the ability to `require` modules, maybe ?
	// Create the global `mo` table
	lua["mo"] = lua.create_table();

	// This needs to happen before mo.load()
	GlobalContext context;
	// Load font if necessary
	//context.activeFont = graphics::newFont("./data/arial.ttf", 12);;
	
	

	bindGraphicsModule(lua);
	bindWindowModule(lua);		
	bindEventModule(lua);
	
	
	
	// TODO(MO): Need to catch silent errors e.g. calling non-existing module or function
	// Might remove exceptions since that does not seem to catch it anyways
	try {
		//Load and execute Lua script from file
		sol::protected_function_result result = lua.script_file("script.lua");

		if (!result.valid()) {
			sol::error err = result;
			std::cerr << "Lua error: " << err.what() << std::endl;
			return -1;
		}
	}
	catch (const std::exception &e) {
		std::cerr << "Exception caught while executing script: " << e.what() << std::endl;
	}

	if (!checkModuleExist("push", lua)) {
		context.pushModuleUsed = true;
		std::cout << "Push Module is used\n";
		//exit(-1);
	}
	else {
		context.pushModuleUsed = false;
	}
	lua["globalContext"] = &context;
	bindResolutionManagerModulePush(lua, context.window);

	// Call Lua functions defined in the script
	sol::function load_function = lua["mo"]["load"];
	if (load_function.valid()) {
		load_function();  // Call the Lua-defined function
	}
	else {
		std::cerr << "Error: 'mo.load' is not defined in Lua" << std::endl;
	}

	//if(!context.pushModuleUsed) createWindow(context);
	
	

	

	// Main loop
	while (context.window.isOpen()) {
		sf::Time dt = clock.restart();
		float deltaTime = dt.asSeconds();

		// Event handling
		sf::Event event;
		while (context.window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				handleKeyPressed(event, lua);
			}
			if (event.type == sf::Event::Closed) {
				context.window.close();
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
		if(!context.pushModuleUsed)
			context.window.clear(context.backgroundColor);

		// Call 'mo.draw'
		sol::function draw_function = lua["mo"]["draw"];
		if (draw_function.valid()) {
			draw_function();
		}
		else {
			std::cerr << "Error: 'mo.draw' is not defined in Lua" << std::endl;
		}
		// Display the contents

		if(!context.pushModuleUsed)
			context.window.display();
	}
	return 0;
}
