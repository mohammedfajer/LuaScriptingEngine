//#include <SFML/Graphics.hpp>
//
//int main() {
//	// Actual window size
//	sf::RenderWindow window(sf::VideoMode(800, 600), "Virtual Resolution Example");
//
//	// Virtual resolution size
//	const unsigned int virtualWidth = 400;
//	const unsigned int virtualHeight = 300;
//
//	// Create a render texture with the virtual resolution
//	sf::RenderTexture renderTexture;
//	if (!renderTexture.create(virtualWidth, virtualHeight)) {
//		return -1; // Error creating render texture
//	}
//
//	// Create a sprite to display the render texture
//	sf::Sprite sprite(renderTexture.getTexture());
//
//	// Calculate scaling factors
//	float scaleX = static_cast<float>(window.getSize().x) / virtualWidth;
//	float scaleY = static_cast<float>(window.getSize().y) / virtualHeight;
//
//
//	// Set the scale of the sprite
//	sprite.setScale(scaleX, scaleY);
//
//
//	// Create some shapes to draw
//	sf::CircleShape circle(50.f);
//	// Center the pivot of the circle
//
//	circle.setOrigin(circle.getRadius(), circle.getRadius());
//
//	
//	circle.setFillColor(sf::Color::Green);
//
//	// Position relative to virtual resolution
//	sf::Vector2f virtualPosition(200.f, 150.f);
//
//	// Scale position to actual window size
//	sf::Vector2f actualPosition = {
//		virtualPosition.x ,
//		virtualPosition.y 
//	};
//	circle.setPosition(actualPosition);
//
//	// Main loop
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//		}
//
//		//circle.move(sf::Vector2f(0.04f, 0.0f));
//
//		// Clear the render texture and draw shapes at the virtual resolution
//		renderTexture.clear(sf::Color::Black);
//
//		// Draw circle to render texture
//		renderTexture.draw(circle);
//
//		// Display the content of render texture
//		renderTexture.display();
//
//		// Clear the window
//		window.clear();
//
//		// Draw the render texture's sprite (scaled to fit the window)
//		window.draw(sprite);
//
//		// Display the content of the window
//		window.display();
//	}
//
//	return 0;
//}
