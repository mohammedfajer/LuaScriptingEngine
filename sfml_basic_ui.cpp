//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//class Button {
//public:
//	Button(float x, float y, float width, float height, const sf::String &text) {
//		// Initialize the button's rectangle
//		buttonRect.setSize(sf::Vector2f(width, height));
//		buttonRect.setPosition(x, y);
//		buttonRect.setFillColor(sf::Color::Blue);
//
//		// Initialize the button's text
//		buttonText.setFont(font);
//		buttonText.setString(text);
//		buttonText.setCharacterSize(24);
//		buttonText.setFillColor(sf::Color::White);
//		buttonText.setPosition(x + 10, y + 10);  // Adjust positioning for padding
//
//		// Load font
//		if (!font.loadFromFile("./data/arial.ttf")) {
//			// Handle error
//		}
//	}
//
//	void draw(sf::RenderWindow &window) {
//		window.draw(buttonRect);
//		window.draw(buttonText);
//	}
//
//	void update(sf::Vector2i mousePos) {
//		if (buttonRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
//			buttonRect.setFillColor(sf::Color::Cyan); // Hover color
//			buttonText.setFillColor(sf::Color::Red);
//			buttonText.setScale(sf::Vector2f(1.02f, 1.02f));
//		}
//		else {
//			buttonRect.setFillColor(sf::Color::Blue); // Default color
//			buttonText.setScale(sf::Vector2f(1, 1));
//			buttonText.setFillColor(sf::Color::White);
//		}
//	}
//
//
//	bool isClicked(sf::Vector2i mousePos) {
//		return buttonRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
//	}
//
//	bool isHovered(sf::Vector2i mousePos) const {
//		return buttonRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
//	}
//
//private:
//	sf::RectangleShape buttonRect;
//	sf::Text buttonText;
//	sf::Font font;
//};
//
//int main() {
//	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML UI Example");
//
//	Button myButton(100, 100, 200, 50, "Click Me");
//
//
//	// Load the custom cursor
//	sf::Cursor handCursor;
//	if (!handCursor.loadFromSystem(sf::Cursor::Hand)) {
//		// Handle error
//	}
//
//
//	sf::Cursor defaultCursor;
//	if (!defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
//
//	}
//	
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed)
//				window.close();
//
//			if (event.type == sf::Event::MouseButtonPressed) {
//				if (event.mouseButton.button == sf::Mouse::Left) {
//					if (myButton.isClicked(sf::Mouse::getPosition(window))) {
//						// Handle button click
//						std::cout << "Button clicked!" << std::endl;
//
//						window.close();
//					}
//				}
//			}
//		}
//
//		// Update cursor
//		if (myButton.isHovered(sf::Mouse::getPosition(window))) {
//			window.setMouseCursor(handCursor);
//		}
//		else {
//			window.setMouseCursor(defaultCursor);  // Set to default cursor
//		}
//
//		// Update hover effect
//		myButton.update(sf::Mouse::getPosition(window));
//
//		window.clear();
//		myButton.draw(window);
//		window.display();
//	}
//
//	return 0;
//}
