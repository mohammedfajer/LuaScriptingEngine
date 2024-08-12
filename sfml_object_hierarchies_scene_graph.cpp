#include <SFML/Graphics.hpp>
#include <vector>

class Node {
public:
	void draw(sf::RenderTarget &target, const sf::Transform &parentTransform) const {
		sf::Transform combinedTransform = parentTransform * m_transform;

		onDraw(target, combinedTransform);

		for (std::size_t i = 0; i < m_children.size(); ++i) {
			m_children[i]->draw(target, combinedTransform);
		}
	}

	void addChild(Node *child) {
		m_children.push_back(child);
	}

	void setPosition(float x, float y) {
		m_transform.translate(x, y);
	}

	void setRotation(float angle) {
		m_transform.rotate(angle);
	}

protected:

	virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const = 0;

	sf::Transform m_transform;
	std::vector<Node *> m_children;
};

//// a simple derived class: a node that draws a sprite
//class SpriteNode : public Node
//{
//public:
//
//	// .. functions to define the sprite
//
//private:
//
//	virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const
//	{
//		target.draw(m_sprite, transform);
//	}
//
//	sf::Sprite m_sprite;
//};

// CircleNode derived class to represent the gun's base
class CircleNode : public Node {
public:
	CircleNode(float radius, const sf::Color &color) {
		m_circle.setRadius(radius);
		m_circle.setFillColor(color);
		m_circle.setOrigin(radius, radius); // Set origin to center for rotation
	}

	void move(float dx, float dy) {
		m_transform.translate(dx, dy);
	}

protected:
	virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const override {
		target.draw(m_circle, transform);
	}

private:
	sf::CircleShape m_circle;
};

// RectangleNode derived class to represent the gun's barrel
class RectangleNode : public Node {
public:
	RectangleNode(float width, float height, const sf::Color &color) {
		m_rectangle.setSize(sf::Vector2f(width, height));
		m_rectangle.setFillColor(color);
		m_rectangle.setOrigin(width / 2, height); // Set origin to the bottom center for correct rotation
	}

protected:
	virtual void onDraw(sf::RenderTarget &target, const sf::Transform &transform) const override {
		target.draw(m_rectangle, transform);
	}

private:
	sf::RectangleShape m_rectangle;
};

int CustomEntryPointSceneGraph() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Scene Graph");

	// Create the gun (circle + rectangle)
	CircleNode *gunBase = new CircleNode(50.f, sf::Color::Blue);
	RectangleNode *gunBarrel = new RectangleNode(10.f, 100.f, sf::Color::Red);

	// Set up the scene graph
	gunBase->addChild(gunBarrel);

	sf::Clock clock;
	float moveSpeed = 200.0f; // pixels per second
	float rotationSpeed = 100.0f; // degrees per second

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Handle movement and rotation
		float deltaTime = clock.restart().asSeconds();
		float dx = 0.f, dy = 0.f, dRotation = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			dy -= moveSpeed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			dy += moveSpeed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			dx -= moveSpeed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			dx += moveSpeed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			dRotation -= rotationSpeed * deltaTime;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			dRotation += rotationSpeed * deltaTime;
		}

		gunBase->move(dx, dy);
		gunBase->setRotation(dRotation);

		// Clear the window
		window.clear();

		// Draw the scene graph
		gunBase->draw(window, sf::Transform::Identity);

		// Display the window
		window.display();
	}

	// Clean up dynamic memory
	delete gunBase;
	delete gunBarrel;

	return 0;
}