
#include <SFML/Graphics.hpp>

int CustomWindowMain() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}


		window.clear();
		window.display();
	}

	return 0;
}