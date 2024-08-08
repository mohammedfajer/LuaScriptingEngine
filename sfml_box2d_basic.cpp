//#include <SFML/Graphics.hpp>
//#include <box2d/box2d.h>
//#include <vector>
//#include <memory>
//#include <cstdlib>
//#include <cmath>
//
//class Shape
//{
//public:
//	Shape(b2Body *body, sf::Shape *shape) : body(body), shape(shape) {}
//	b2Body *body;
//	std::unique_ptr<sf::Shape> shape;
//};
//
//void createCircle(b2World &world, std::vector<Shape> &shapes, float x, float y, float radius)
//{
//	b2BodyDef bodyDef;
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.position.Set(x, y);
//	b2Body *body = world.CreateBody(&bodyDef);
//
//	b2CircleShape circle;
//	circle.m_radius = radius;
//
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &circle;
//	fixtureDef.density = 1.0f;
//	fixtureDef.friction = 0.3f;
//	body->CreateFixture(&fixtureDef);
//
//	auto circleShape = std::make_unique<sf::CircleShape>(radius * 30.0f); // Scale for SFML
//	circleShape->setFillColor(sf::Color::Green);
//	shapes.emplace_back(body, circleShape.release());
//}
//
//void createBox(b2World &world, std::vector<Shape> &shapes, float x, float y, float width, float height)
//{
//	b2BodyDef bodyDef;
//	bodyDef.type = b2_dynamicBody;
//	bodyDef.position.Set(x, y);
//	b2Body *body = world.CreateBody(&bodyDef);
//
//	b2PolygonShape box;
//	box.SetAsBox(width / 2.0f, height / 2.0f);
//
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &box;
//	fixtureDef.density = 1.0f;
//	fixtureDef.friction = 0.3f;
//	body->CreateFixture(&fixtureDef);
//
//	auto boxShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(width * 30.0f, height * 30.0f)); // Scale for SFML
//	boxShape->setFillColor(sf::Color::Red);
//	shapes.emplace_back(body, boxShape.release());
//}
//
//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + Box2D Falling Shapes Example");
//
//	// Create Box2D world with gravity
//	b2Vec2 gravity(0.0f, -10.0f);
//	b2World world(gravity);
//
//	// Create ground body
//	b2BodyDef groundBodyDef;
//	groundBodyDef.position.Set(0.0f, -10.0f); // Center the ground on the bottom
//	b2Body *groundBody = world.CreateBody(&groundBodyDef);
//
//	b2PolygonShape groundBox;
//	groundBox.SetAsBox(40.0f, 1.0f); // Make the ground box wider and thinner
//	groundBody->CreateFixture(&groundBox, 0.0f);
//
//	// Create ground rectangle shape for SFML
//	sf::RectangleShape groundShape(sf::Vector2f(800.0f, 60.0f)); // Full width of the window and height
//	groundShape.setFillColor(sf::Color::Blue);
//	groundShape.setPosition(0, 540); // Position at the bottom of the window
//
//	std::vector<Shape> shapes;
//
//	// Spawn shapes
//	for (int i = 0; i < 10; ++i)
//	{
//		float x = static_cast<float>(rand() % 600 + 100) / 30.0f; // Random x position
//		float y = static_cast<float>(rand() % 300 + 50) / 30.0f; // Random y position (start higher)
//		if (rand() % 2)
//			createCircle(world, shapes, x, y, 0.5f + static_cast<float>(rand() % 100) / 300.0f);
//		else
//			createBox(world, shapes, x, y, 1.0f + static_cast<float>(rand() % 100) / 300.0f, 1.0f + static_cast<float>(rand() % 100) / 300.0f);
//	}
//
//	sf::Clock clock;
//	while (window.isOpen())
//	{
//		sf::Time dt = clock.restart();
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		// Step the Box2D world
//		world.Step(dt.asSeconds(), 8, 3);
//
//		// Update SFML shapes positions
//		for (auto &shape : shapes)
//		{
//			b2Vec2 position = shape.body->GetPosition();
//			shape.shape->setPosition(position.x * 30.0f, 600 - position.y * 30.0f); // Convert Box2D coordinates to SFML
//			shape.shape->setRotation(-shape.body->GetAngle() * 180.0f / b2_pi); // Convert radians to degrees
//		}
//
//		window.clear();
//		window.draw(groundShape); // Draw the ground
//		for (const auto &shape : shapes)
//		{
//			window.draw(*shape.shape); // Draw falling shapes
//		}
//		window.display();
//	}
//
//	return 0;
//}
