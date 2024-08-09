#include <SFML/Graphics.hpp>





int CustomInputMain() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Keyboard Input");

	sf::CircleShape circle(50.f);
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(300.f, 200.f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Check if the 'W' key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			circle.move(0.f, -1.f);
		}

		// Check if the 'S' key is pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			circle.move(0.f, 1.f);
		}

		// Uncomment this to use keyboard input
		// Get mouse position and set circle position
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		circle.setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));


		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}
