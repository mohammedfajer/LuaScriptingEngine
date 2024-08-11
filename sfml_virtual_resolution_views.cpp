#include <SFML/Graphics.hpp>
// CustomVirtualResolutionMainViews
int CustomVirtualResolutionMainViews() {
	// Actual window size
	sf::RenderWindow window(sf::VideoMode(700, 700), "Virtual Resolution Example");

	// Virtual resolution size
	const unsigned int virtualWidth = 10;
	const unsigned int virtualHeight = 10;

	// Create and set up the view for the virtual resolution
	sf::View view;
	float windowWidth = static_cast<float>(window.getSize().x);
	float windowHeight = static_cast<float>(window.getSize().y);

	// Calculate scaling factors
	float scaleX = windowWidth / virtualWidth;
	float scaleY = windowHeight / virtualHeight;
	float scale = std::min(scaleX, scaleY);

	// Set the view size and center based on the virtual resolution
	view.setSize(virtualWidth * scale, virtualHeight * scale);
	view.setCenter(virtualWidth / 2.0f, virtualHeight / 2.0f);
	window.setView(view);

	// Create a rectangle shape
	sf::RectangleShape rect(sf::Vector2f(1.5, 1.5)); // Small rectangle
	rect.setFillColor(sf::Color::Red);

	// Calculate the center of the rectangle
	float centerX = rect.getSize().x / 2.0f;
	float centerY = rect.getSize().y / 2.0f;
	rect.setOrigin(centerX, centerY);

	// Position shapes relative to virtual resolution
	sf::Vector2f pos(0.0, 0.0); // Center of the virtual resolution
	rect.setPosition(pos);

	// Main loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// Clear the window
		window.clear(sf::Color::Black);

		// Draw the rectangle to the window
		window.draw(rect);

		// Display the content of the window
		window.display();
	}

	return 0;
}
