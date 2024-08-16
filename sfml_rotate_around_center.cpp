#include <SFML/Graphics.hpp>

int CustomEntryPointRotateAroundCenter()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Rotate Square");

	// Create a square
	sf::RectangleShape square(sf::Vector2f(100.f, 100.f));
	square.setFillColor(sf::Color::Green);

	// Set the origin to the center of the square
	square.setOrigin(square.getSize().x / 2, square.getSize().y / 2);

	// Set the initial position of the square
	square.setPosition(400.f, 300.f); // Center of the window

	// text
	  // Load a font
	sf::Font font;
	if (!font.loadFromFile("./data/arial.ttf")) {
		return -1; // Make sure to use a valid font file path
	}

	// Create a text object
	sf::Text text("Hello, SFML!", font, 50);
	text.setFillColor(sf::Color::White);

	// Set the origin to the center of the text
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	// Set the initial position of the text
	text.setPosition(400.f, 300.f); // Center of the window
	text.setScale(1, 1);

	float rotationAngle = 0.0f; // Initial rotation angle

	// Start the game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear the window
		window.clear();

		// Rotate the square
		rotationAngle += 0.1f; // Adjust the rotation speed as needed
		square.setRotation(rotationAngle);

		text.setRotation(rotationAngle);

		// Draw the text
		window.draw(text);

		// Draw the square
		window.draw(square);

		// Display what was drawn
		window.display();
	}

	return 0;
}
