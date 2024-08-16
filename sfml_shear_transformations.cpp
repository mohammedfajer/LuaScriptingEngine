#include <SFML/Graphics.hpp>

int CustomEntryPointShear() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shear Transformation");

	// Create a shape (e.g., rectangle)
	sf::RectangleShape rectangle(sf::Vector2f(100, 50));
	rectangle.setFillColor(sf::Color::Green);
	rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);
	rectangle.setPosition(400, 300);

	// Create a shear transform (shear along the X-axis with a factor of 0.5)
	sf::Transform shearTransform;
	shearTransform = sf::Transform(1, 0.5, 0,  // shear factor in the X direction
		0, 1, 0,   // no shear in Y direction
		0, 0, 1);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Apply the shear transform and draw the shape
		window.draw(rectangle, shearTransform);

		window.display();
	}

	return 0;
}
