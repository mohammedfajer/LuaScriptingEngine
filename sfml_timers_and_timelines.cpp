#include <SFML/Graphics.hpp>
#include <iostream>

int CustomEntryPointTimersTimelines() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Timer Example");

	// Create a clock to track time
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time updateInterval = sf::seconds(1.0f / 60.0f); // Target 60 FPS

	// Time line
	sf::Time timeSinceLastEvent = sf::Time::Zero;
	sf::Time eventInterval = sf::seconds(2.0f); // Event occurs every 2 seconds

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Calculate the time elapsed since the last frame
		sf::Time deltaTime = clock.restart();
		timeSinceLastUpdate += deltaTime;

		// Update game logic based on the target update interval
		while (timeSinceLastUpdate >= updateInterval) {
			timeSinceLastUpdate -= updateInterval;

			// Update game logic here (e.g., move objects, check collisions)
			//std::cout << "Updating game logic" << std::endl;
		}

		timeSinceLastEvent += deltaTime;
		// Trigger events based on the time line
		if (timeSinceLastEvent >= eventInterval) {
			timeSinceLastEvent -= eventInterval;
			// Trigger the event (e.g., play a sound, spawn an enemy)
			std::cout << "Event triggered!" << std::endl;
		}

		window.clear(sf::Color::Black);
		// Draw game objects here
		window.display();
	}

	return 0;
}
