//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <cstdlib>
//#include <ctime>
//
//class Particle {
//public:
//	sf::CircleShape shape;
//	sf::Vector2f velocity;
//
//	Particle(float x, float y) : shape(2.f), velocity((rand() % 100 - 50) / 50.f, (rand() % 100 - 50) / 50.f) {
//		shape.setPosition(x, y);
//		shape.setFillColor(sf::Color::White);
//	}
//
//	void update() {
//		shape.move(velocity);
//	}
//};
//
//int main() {
//	sf::RenderWindow window(sf::VideoMode(800, 600), "Particle System");
//	srand(static_cast<unsigned>(time(0)));
//
//	std::vector<Particle> particles;
//	for (int i = 0; i < 100; ++i) {
//		particles.emplace_back(400.f, 300.f);
//	}
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
//
//		for (auto &particle : particles) {
//			particle.update();
//			window.draw(particle.shape);
//		}
//
//		window.display();
//	}
//
//	return 0;
//}
