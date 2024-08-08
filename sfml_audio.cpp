//#include <SFML/Audio.hpp>
//#include <iostream>
//
//int main()
//{
//	// Load a sound buffer from file and create a sound
//	sf::SoundBuffer soundBuffer;
//	if (!soundBuffer.loadFromFile("./data/soundEffect.mp3"))
//	{
//		std::cerr << "Error loading sound effect" << std::endl;
//		return -1;
//	}
//
//	sf::Sound sound;
//	sound.setBuffer(soundBuffer);
//
//	// Load music from file and play it
//	sf::Music music;
//	if (!music.openFromFile("./data/music.mp3"))
//	{
//		std::cerr << "Error loading background music" << std::endl;
//		return -1;
//	}
//
//	music.setLoop(true); // Loop the music
//	music.play();
//
//	// Play the sound effect
//	sound.play();
//
//	// Main loop to keep the program running
//	while (music.getStatus() == sf::SoundSource::Playing)
//	{
//		// Do nothing, just wait for the music to finish
//		// You could handle other events here if needed
//	}
//
//	return 0;
//}
