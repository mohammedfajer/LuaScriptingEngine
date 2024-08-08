//#include <SFML/Graphics.hpp>
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Shape");
//
//    // Create a rectangle shape
//    sf::RectangleShape rectangle(sf::Vector2f(120.f, 50.f));
//    rectangle.setFillColor(sf::Color::Blue);
//    rectangle.setPosition(200.f, 150.f);
//
//	// Create a circle shape
//	sf::CircleShape circle(50.f);
//	circle.setFillColor(sf::Color::Green);
//	circle.setPosition(300.f, 200.f);
//
//
//	// Create a convex shape with 5 points
//	sf::ConvexShape convex;
//	convex.setPointCount(5);
//	convex.setPoint(0, sf::Vector2f(500.f, 200.f));
//	convex.setPoint(1, sf::Vector2f(600.f, 300.f));
//	convex.setPoint(2, sf::Vector2f(750.f, 400.f));
//	convex.setPoint(3, sf::Vector2f(450.f, 400.f));
//	convex.setPoint(4, sf::Vector2f(400.f, 300.f));
//	convex.setFillColor(sf::Color::Yellow);
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//
//        window.clear();
//        window.draw(rectangle);
//        window.draw(circle);
//        window.draw(convex);
//        window.display();
//    }
//
//    return 0;
//}
