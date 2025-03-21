#include <SFML/Graphics.hpp>

int CustomVirtualResoltuionMain() {
	// Actual window size
	sf::RenderWindow window(sf::VideoMode(700, 700), "Virtual Resolution Example");

	// Virtual resolution size
	const unsigned int virtualWidth = 10;
	const unsigned int virtualHeight = 10;

	// Create a render texture with the virtual resolution
	sf::RenderTexture renderTexture;
	if (!renderTexture.create(virtualWidth, virtualHeight)) {
		return -1; // Error creating render texture
	}

	// Create a sprite to display the render texture
	sf::Sprite sprite(renderTexture.getTexture());

	// Calculate scaling factors
	float scaleX = static_cast<float>(window.getSize().x) / virtualWidth;
	float scaleY = static_cast<float>(window.getSize().y) / virtualHeight;

	// Set the scale of the sprite
	sprite.setScale(scaleX, scaleY);

	

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

		// Clear the render texture and draw shapes at the virtual resolution
		renderTexture.clear(sf::Color::Black);

		// Draw shapes to render texture
		renderTexture.draw(rect);
		
		

		// Display the content of render texture
		renderTexture.display();

		// Clear the window
		window.clear();

		// Draw the render texture's sprite (scaled to fit the window)
		window.draw(sprite);

		// Display the content of the window
		window.display();
	}

	return 0;
}
