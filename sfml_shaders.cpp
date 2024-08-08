//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//#include <random>
//#include <iostream>
//
//float getRandomFloat(float min, float max) {
//	static std::random_device rd; // Obtain a random number from hardware
//	static std::mt19937 gen(rd()); // Seed the generator
//	std::uniform_real_distribution<> dis(min, max); // Define the range
//	return dis(gen); // Generate the random number
//}
//
//int main() {
//	sf::RenderWindow window(sf::VideoMode(800, 600), "Shader Examples");
//	window.setFramerateLimit(60); // Set the frame rate limit to 60 FPS
//	// Load textures
//	sf::Texture texture;
//	if (!texture.loadFromFile("./data/bunny.jpg")) {
//		return -1; // Error loading texture
//	}
//
//	sf::Sprite sprite(texture);
//
//	sprite.setPosition(sf::Vector2f(100.0f, 100.0f));
//
//	// Set the size of the sprite (scaling the sprite)
//	sf::Vector2u textureSize = texture.getSize();
//	float desiredWidth = 50.f;
//	float desiredHeight = 50.f;
//	
//	// Calculate scale factors
//	float scaleX = desiredWidth / textureSize.x;
//	float scaleY = desiredHeight / textureSize.y;
//	
//	// Set the scale of the sprite
//	sprite.setScale(scaleX, scaleY);
//	
//	// Set the origin of the sprite (pivot point for rotation)
//	sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f); // Center of the sprite
//
//	// Load and compile shaders
//	//sf::Shader grayscaleShader;
//	//if (!grayscaleShader.loadFromFile("./data/grayscale.vert", "./data/grayscale.frag")) {
//	//	return -1; // Error loading grayscale shader
//	//}
//
//	sf::Shader pulseShader;
//	if (!pulseShader.loadFromFile("./data/pulse.vert", "./data/pulse.frag")) {
//		return -1; // Error loading pulse shader
//	}
//
//
//	// Set pulse shader uniform
//	
//	pulseShader.setUniform("time", 0.f); // Initialize time to 0
//
//	sf::Clock clock; // Clock to keep track of time
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//		}
//
//		float deltaTime = clock.restart().asSeconds();
//		float elapsedTime = clock.getElapsedTime().asSeconds();
//
//		// Update pulse shader time parameter
//
//		std::cout << elapsedTime << std::endl;
//
//		// Accumulate time and update the shader uniform
//		static float accumulatedTime = 0.f;
//		accumulatedTime += deltaTime;
//		
//		pulseShader.setUniform("time", accumulatedTime);
//
//		// Clear window
//		window.clear();
//
//		// Apply grayscale shader
//		//window.draw(sprite, &grayscaleShader);
//
//		// Apply pulse shader
//		window.draw(sprite, &pulseShader);
//
//		// Display window content
//		window.display();
//	}
//
//	return 0;
//}
