#include <SFML/Graphics.hpp>
#include <stack>

class TransformManager {
public:
	void push(const sf::Transform &transform) {
		if (transformStack.empty()) {
			transformStack.push(transform);
		}
		else {
			// Multiply the current top transformation with the new one
			transformStack.push(transformStack.top() * transform);
		}
	}

	void pop() {
		if (!transformStack.empty()) {
			transformStack.pop();
		}
	}

	const sf::Transform &getCurrentTransform() const {
		return transformStack.top();
	}

	void pushIdentity() {
		if (transformStack.empty()) {
			transformStack.push(sf::Transform::Identity);
		}
	}

private:
	std::stack<sf::Transform> transformStack;
};


// To Be Continued....
int CustomEntryPointPushPop() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Push/Pop Transformation Example");

	sf::Font font;
	if (!font.loadFromFile("./data/arial.ttf")) {
		return -1;
	}

	sf::Text normalText("Normal text", font, 50);
	normalText.setPosition(50, 50);

	sf::Text scaledText("Scaled text", font, 50);
	scaledText.setPosition(50, 50);

	TransformManager transformManager;
	transformManager.pushIdentity();  // Start with the identity transform

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear the window
		window.clear();

		// Draw with normal transformation
		window.draw(normalText, sf::RenderStates(transformManager.getCurrentTransform()));

		// Push a scaling transformation
		sf::Transform scaleTransform;
		scaleTransform.scale(0.5f, 0.5f);
		transformManager.push(scaleTransform);

		// Draw the scaled text with the new transformation
		window.draw(scaledText, sf::RenderStates(transformManager.getCurrentTransform()));
		

		// Pop the scaling transformation
		transformManager.pop();

		// Draw the normal text again to show it is unaffected
		window.draw(normalText, sf::RenderStates(transformManager.getCurrentTransform()));

		// Display the window
		window.display();
	}

	return 0;
}
