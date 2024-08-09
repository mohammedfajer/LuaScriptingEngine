
#include "audio.h"


void playSound(const std::string &filepath) {

	std::cout << "Called from lua playSound(..)\n";

	// Create a sound buffer and load the sound file
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filepath)) {
		std::cerr << "Failed to load sound from file: " << filepath << std::endl;
		return;
	}

	// Create a sound object and set its buffer
	sf::Sound sound;
	sound.setBuffer(buffer);

	// Play the sound
	sound.play();

	// Keep the program running to allow the sound to play completely
	// This is a simple example; in a real application, you'd use an event loop
	// and manage the sound duration properly
	sf::sleep(sf::seconds(3)); // Adjust this duration as needed
}

void bind_audio_module(sol::state &lua)
{
	sol::table audio = lua.create_table();
	// Expose the playSound function to Lua
	audio.set_function("playSound", &playSound);


	// Bind the audio table to Lua
	lua["mo"]["audio"] = audio;


}
