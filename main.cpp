
#pragma region Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

	// mo.graphics.print(text, x, y, [r], [sx], [sy], [ox], [oy], [kx], [ky])
	void printText(const std::string &text, f32 x, f32 y, sol::state &lua,
		sol::optional<f32> Pangle = sol::nullopt,
		sol::optional<f32> Psx = sol::nullopt, sol::optional<f32> Psy = sol::nullopt,
		sol::optional<f32> Pox = sol::nullopt, sol::optional<f32> Poy = sol::nullopt,
		sol::optional<f32> Pkx = sol::nullopt, sol::optional<f32> Pky = sol::nullopt)
	{
		std::cout << "mo.graphics.print(...) called.\n";

		f32 angle = Pangle.value_or(0.0f);
		f32 sx = Psx.value_or(1.0f);
		f32 sy = Psy.value_or(1.0f);
		f32 ox = Pox.value_or(0.0f);
		f32 oy = Poy.value_or(0.0f);
		f32 kx = Pkx.value_or(0.0f);
		f32 ky = Pky.value_or(0.0f);


		std::cout << "[r] = " << angle << ", [sx] = " << sx << ", [sy] = " << sy << std::endl;


		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();

		if (!context || !context->window.isOpen()) {
			std::cout << "Error: globalContext not found in lua state.\n";  
			return;
		}


		
		sf::Text sfText;
		if (!context->activeFont) {
			std::cout << "Font not set we need to set one later\n";
			return;
		}
		// TODO(mo): if fount not setup we need to setup one!
		sfText.setFont(context->activeFont->font);
		sfText.setString(text);
		sfText.setCharacterSize(context->activeFont->size); // Set text size
		sfText.setFillColor(sf::Color::White); // Set text color



		// origin x, y
		
	

		sf::FloatRect textRect = sfText.getLocalBounds();
		sfText.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);

		// x, y
		sfText.setPosition(x, y);
		
		// scale x, y
		sfText.setScale(sx, sy);



		// applying shear transform shear-x, shear-y
		sf::Transform shearTransform = sf::Transform(	1, kx, 0,
										ky, 1, 0,
										0, 0, 1);

		// angle
		sfText.setRotation(angle);

		// Draw text on canvas
		//context->window.draw(sfText, t); Since window in old system, but now we use canvas
		context->canvas->draw(sfText, shearTransform);

		
		
	}

	// Dummy draw text Remove Later
	void drawText(const std::string &text, float x, float y, sol::state &lua) {

		std::cout << "drawText() called from lua.\n";

		GlobalContext *context = lua["globalContext"].get<GlobalContext *>();

		if (!context || !context->window.isOpen()) return;

		if (context && context->activeFont) {
			sf::Text sfText;
			sfText.setFont(context->activeFont->font);
			sfText.setString(text);
			sfText.setCharacterSize(context->activeFont->size); // Set text size
			sfText.setFillColor(sf::Color::White); // Set text color
			sfText.setPosition(x, y);
		//	context->window.draw(sfText);
			context->canvas->draw(sfText);
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
		//std::cout << "Drawing rectangle at (" << x << ", " << y << ") with size (" << width << ", " << height << ")\n";
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
	graphics.set_function("circle", [&](f32 x, f32 y, f32 radius, sol::optional<sol::table> colorTable = sol::nullopt) { graphics::drawCircle(x, y, radius, lua, colorTable); });
	graphics.set_function("setBackgroundColor", [&](sol::table colorTable) { graphics::setBackgroundColor(colorTable, lua); });
	graphics.set_function("clear", [&](sol::table colorTable) { graphics::setBackgroundColor(colorTable, lua); });


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

	graphics.set_function("print", [&](const std::string &text, f32 x, f32 y,
		sol::optional<f32> Pangle = sol::nullopt,
		sol::optional<f32> Psx = sol::nullopt, sol::optional<f32> Psy = sol::nullopt,
		sol::optional<f32> Pox = sol::nullopt, sol::optional<f32> Poy = sol::nullopt,
		sol::optional<f32> Pkx = sol::nullopt, sol::optional<f32> Pky = sol::nullopt) {
		graphics::printText(text, x, y, lua, Pangle, Psx, Psy, Pox, Poy, Pkx, Pky);
		});

	lua["mo"]["graphics"] = graphics;
}

#pragma endregion

#pragma region Window Module


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
	lua["resolutionManager"] = resolutionManager;
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
	{"escape", sf::Keyboard::Escape},
	{"enter", sf::Keyboard::Enter},
	{"return", sf::Keyboard::Return}
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

#pragma region Audio Module

namespace audio {

	// TODO (mo) we might not be able to distinguish a sound effect from music based on isMusicFile() maybe!
	// This does not expose setters, getters, pause, etc...
	class AudioSource {
	public:
		// Load the audio file and determine whether to use Sound or Music

		bool loadFromFile(const std::string &filename, const std::string &type) {

			if (type == "static") {
				soundBuffer = std::make_unique<sf::SoundBuffer>();
				if (soundBuffer->loadFromFile(filename)) {
					sound = std::make_unique<sf::Sound>();
					sound->setBuffer(*soundBuffer);
					return true;
				}
			}
			else if (type == "stream") {
				music = std::make_unique<sf::Music>();
				return music->openFromFile(filename);
			}

			return false;
		}

		bool loadFromFile(const std::string &filename) {
			if (isMusicFile(filename)) {
				music = std::make_unique<sf::Music>();
				return music->openFromFile(filename);
			}
			else {
				soundBuffer = std::make_unique<sf::SoundBuffer>();
				if (soundBuffer->loadFromFile(filename)) {
					sound = std::make_unique<sf::Sound>();
					sound->setBuffer(*soundBuffer);
					return true;
				}
			}
			return false;
		}

		// Play the audio
		void play() {
			if (music) {
				music->play();
			}
			else if (sound) {
				sound->play();
			}
		}

		// Set the volume
		void setVolume(float volume) {
			if (music) {
				music->setVolume(volume);
			}
			else if (sound) {
				sound->setVolume(volume);
			}
		}

		// Set looping
		void setLoop(bool loop) {
			if (music) {
				music->setLoop(loop);
			}
			else if (sound) {
				sound->setLoop(loop);
			}
		}

		void pause() {
			if (music) music->pause();
			else sound->pause();
		}

		// Additional functions can be added as needed, such as stop, pause, etc.

	private:
		std::unique_ptr<sf::SoundBuffer> soundBuffer;
		std::unique_ptr<sf::Sound> sound;
		std::unique_ptr<sf::Music> music;

		// Determine if the file is music based on its extension
		bool isMusicFile(const std::string &filename) {
			const std::string musicExtensions[] = { ".ogg", ".wav", ".flac", ".mp3" };
			for (const auto &ext : musicExtensions) {
				if (filename.size() >= ext.size() &&
					filename.compare(filename.size() - ext.size(), ext.size(), ext) == 0) {
					return true;
				}
			}
			return false;
		}
	};

	void bindAudioModule(sol::state &lua) {
		// Create the `mo` table if it doesn't exist
		sol::table mo = lua["mo"].get_or_create<sol::table>();

		// Create the `audio` table under `mo`
		sol::table audio = mo["audio"].get_or_create<sol::table>();

		// Bind sf::SoundBuffer
		audio.new_usertype<sf::SoundBuffer>(
			"SoundBuffer",
			sol::constructors<sf::SoundBuffer()>(),
			"loadFromFile", &sf::SoundBuffer::loadFromFile
		);

		// Bind sf::Sound
		audio.new_usertype<sf::Sound>(
			"Sound",
			sol::constructors<sf::Sound()>(),
			"setBuffer", &sf::Sound::setBuffer,
			"play", &sf::Sound::play,
			"pause", &sf::Sound::pause,
			"stop", &sf::Sound::stop
		);

		audio["Sound"]["setVolume"] = [](sf::Sound &self, float volume) { self.setVolume(volume); };
		audio["Sound"]["setLoop"] = [](sf::Sound &self, bool loop) { self.setLoop(loop); };
		audio["Sound"]["setPitch"] = [](sf::Sound &self, float pitch) { self.setPitch(pitch); };
		audio["Sound"]["setPosition"] = [](sf::Sound &self, float x, float y, float z) { self.setPosition(x, y, z); };


		// Bind sf::Music
		audio.new_usertype<sf::Music>(
			"Music",
			sol::constructors<sf::Music()>(),
			"openFromFile", &sf::Music::openFromFile,
			"play", &sf::Music::play,
			"pause", &sf::Music::pause,
			"stop", &sf::Music::stop
		);

		audio["Music"]["setVolume"] = [](sf::Music &self, float volume) { self.setVolume(volume); };
		audio["Music"]["setLoop"] = [](sf::Music &self, bool loop) { self.setLoop(loop); };
		audio["Music"]["setPitch"] = [](sf::Music &self, float pitch) { self.setPitch(pitch); };
		audio["Music"]["setPosition"] = [](sf::Music &self, float x, float y, float z) { self.setPosition(x, y, z); };

	}

	void bindSimplifiedAudioModule(sol::state &lua) {
		// Create the `mo` table if it doesn't exist
		sol::table mo = lua["mo"].get_or_create<sol::table>();

		// Create the `audio` table under `mo`
		sol::table audio = mo["audio"].get_or_create<sol::table>();

		// Bind the AudioSource class
		audio.new_usertype<AudioSource>(
			"AudioSource",
			sol::constructors<AudioSource()>(),
			//"loadFromFile", &AudioSource::loadFromFile,
			"play", &AudioSource::play,
			"setVolume", &AudioSource::setVolume,
			"setLoop", &AudioSource::setLoop,
			"pause", &AudioSource::pause
		);

		// Add the newSource function to the audio table
		audio.set_function("newSource", [](const std::string &filename) {
			auto source = std::make_shared<AudioSource>();
			if (source->loadFromFile(filename)) {
				return source;
			}
			return std::shared_ptr<AudioSource>(nullptr); // Return null if the file fails to load
		});

		audio.set_function("newSource", [](const std::string &filename, const std::string &type) {
			std::cout << "Typed newSource() called.\n";
			auto source = std::make_shared<AudioSource>();
			if (source->loadFromFile(filename, type)) {
				return source;
			}
			return std::shared_ptr<AudioSource>(nullptr); // Return null if the file fails to load
			});
	}

}

#pragma endregion


#pragma region Timer Module
#include <numeric>

namespace timer {

	// TODO(mo) we might not want to use a class and lua["mo"]["timer"] = timer obj we might want to prepere procedural func calls
	class Timer {
	public:
		Timer()
			: frameTimes(), fps(0), lastFrameTime(sf::Time::Zero) {
			clock.restart();
			lastTime = clock.getElapsedTime();
			lastFrameStart = clock.getElapsedTime().asSeconds();
		}

		float getAverageDelta() const {
			if (frameTimes.empty()) return 0.0f;
			float total = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
			return total / static_cast<float>(frameTimes.size());
		}

		float getDelta() const {
			return lastFrameTime.asSeconds();
		}

		int getFPS() const {
			return fps;
		}

		float getTime() const {
			return clock.getElapsedTime().asSeconds();
		}

		void sleep(float seconds) {
			sf::sleep(sf::seconds(seconds));
		}

		void step() {
			sf::Time currentTime = clock.getElapsedTime();
			lastFrameTime = currentTime - lastTime;
			lastTime = currentTime;

			if (!frameTimes.empty() && getTime() - lastFrameStart >= 1.0f) {
				fps = static_cast<int>(frameTimes.size());
				frameTimes.clear();
				lastFrameStart = getTime();
			}

			frameTimes.push_back(lastFrameTime.asSeconds());
		}

	private:
		sf::Clock clock;
		sf::Time lastTime;
		sf::Time lastFrameTime;
		float lastFrameStart;
		std::vector<float> frameTimes;
		int fps;
	};

	void bindTimerModule(sol::state &lua) {
		// Retrieve the existing `mo` table or create it if it doesn't exist
		sol::table mo = lua["mo"].get_or_create<sol::table>();

		// Retrieve the existing `timer` table or create it if it doesn't exist
		sol::table timerM = mo["timer"].get_or_create<sol::table>();

		// Expose the Timer class to Lua
		timerM.new_usertype<Timer>("Timer",
			"getAverageDelta", &Timer::getAverageDelta,
			"getDelta", &Timer::getDelta,
			"getFPS", &Timer::getFPS,
			"getTime", &Timer::getTime,
			"sleep", &Timer::sleep,
			"step", &Timer::step
		);

		// Create a global instance of the Timer class and bind it to `mo.timer`.
		auto timer = std::make_shared<Timer>();
		mo["timer"] = timer;
	}
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
	//audio::bindAudioModule(lua);
	audio::bindSimplifiedAudioModule(lua);
	timer::bindTimerModule(lua);
	
	
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
			
			if (event.type == sf::Event::Resized) {
				// NOTE(MO): not sure if that works, we need to test it later :)
				ResolutionManager *rm = lua["resolutionManager"].get<ResolutionManager *>();
				if (rm) {
					rm->resize(event.size.width, event.size.height);
				}
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
