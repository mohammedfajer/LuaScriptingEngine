#include <SFML/Graphics.hpp>
#include <iostream>

int CustomTextureFiltering()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1200, 768), "SFML Texture Filtering");

	// Load a texture from a file
	sf::Texture texture;
	if (!texture.loadFromFile("./data/cat.png")) {
		std::cerr << "Error loading texture" << std::endl;
		return -1;
	}

	// Create two sprites to demonstrate both filtering modes
	sf::Sprite spritePixelated;
	spritePixelated.setTexture(texture);
	spritePixelated.setPosition(100, 100);
	spritePixelated.setScale(2.0f, 2.0f);  // Scale up to see the effect clearly

	sf::Sprite spriteSmooth;
	spriteSmooth.setTexture(texture);
	spriteSmooth.setPosition(500, 100);
	spriteSmooth.setScale(2.0f, 2.0f);  // Scale up to see the effect clearly

	// Set the filtering mode for each texture
	texture.setSmooth(false);  // Pixelated effect
	spritePixelated.setTexture(texture);

	texture.setSmooth(true);   // Smooth effect
	spriteSmooth.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		// Draw the sprites with different filtering modes
		window.draw(spritePixelated);
		window.draw(spriteSmooth);

		window.display();
	}

	return 0;
}
