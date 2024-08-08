//#include <SFML/Graphics.hpp>
//
//int main() {
//	sf::RenderWindow window(sf::VideoMode(800, 600), "Text Rendering");
//
//	// Load a font
//	sf::Font font;
//	if (!font.loadFromFile("./data/arial.ttf")) {
//		return -1; // Error loading font
//	}
//
//	// Create a text object
//	sf::Text text("Hello, SFML!", font, 50);
//	text.setFillColor(sf::Color::Red);
//	text.setPosition(200.f, 300.f);
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//		}
//
//		window.clear();
//		window.draw(text);
//		window.display();
//	}
//
//	return 0;
//}
