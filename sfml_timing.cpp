//#include <SFML/Graphics.hpp>
//#include <sstream> // For string stream
//
//int main() {
//	sf::RenderWindow window(sf::VideoMode(800, 600), "FPS Display Example");
//	sf::Clock clock; // Clock for FPS calculation
//	sf::Clock frameClock; // Clock for timing each frame
//
//	sf::CircleShape circle(50.f);
//	circle.setFillColor(sf::Color::Green);
//	circle.setPosition(300.f, 200.f);
//
//	// Load a font
//	sf::Font font;
//	if (!font.loadFromFile("./data/arial.ttf")) {
//		return -1; // Error loading font
//	}
//
//	// Create a text object for FPS display
//	sf::Text fpsText("", font, 20);
//	fpsText.setFillColor(sf::Color::Red);
//	fpsText.setPosition(10.f, 10.f);
//
//	int frameCount = 0;
//	float fps = 0.0f;
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//		}
//
//		// Calculate the elapsed time since the last frame
//		sf::Time elapsed = clock.restart();
//		float deltaTime = elapsed.asSeconds();
//
//		// Move the circle
//		circle.move(0.f, deltaTime * 100.f);
//
//		// Calculate FPS
//		frameCount++;
//		if (frameClock.getElapsedTime().asSeconds() >= 1.0f) {
//			fps = static_cast<float>(frameCount);
//			frameCount = 0;
//			frameClock.restart();
//		}
//
//		// Update FPS text
//		std::stringstream ss;
//		ss << "FPS: " << fps;
//		fpsText.setString(ss.str());
//
//		window.clear();
//		window.draw(circle);
//		window.draw(fpsText); // Draw FPS text
//		window.display();
//	}
//
//	return 0;
//}
