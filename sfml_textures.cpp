#include <SFML/Graphics.hpp>

int CustomTexturesMain() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Texture Example");

	// Load a texture
	sf::Texture texture;
	if (!texture.loadFromFile("./data/bunny.jpg")) {
		return -1; // Error loading texture
	}

	// Create a sprite with the texture
	sf::Sprite sprite(texture);

	
	// Set the position of the sprite
	sprite.setPosition(100.f, 100.f);

	// Set the size of the sprite (scaling the sprite)
	sf::Vector2u textureSize = texture.getSize();
	float desiredWidth = 50.f;
	float desiredHeight = 50.f;

	// Calculate scale factors
	float scaleX = desiredWidth / textureSize.x;
	float scaleY = desiredHeight / textureSize.y;

	// Set the scale of the sprite
	sprite.setScale(scaleX, scaleY);

	// Set the origin of the sprite (pivot point for rotation)
	sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f); // Center of the sprite

	

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		static float angle = 0.0f;
		angle++;
		// Set the rotation of the sprite
		sprite.setRotation(angle); // Rotate 45 degrees
		window.draw(sprite);
		window.display();
	}

	return 0;
}
