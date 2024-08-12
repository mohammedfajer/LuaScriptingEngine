#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// Function to calculate the intersection of a ray and a line segment
sf::Vector2f raycast(sf::Vector2f rayOrigin, sf::Vector2f rayDirection, sf::Vector2f segmentStart, sf::Vector2f segmentEnd) {
	sf::Vector2f segmentDir = segmentEnd - segmentStart;
	sf::Vector2f toSegment = segmentStart - rayOrigin;

	float det = rayDirection.x * segmentDir.y - rayDirection.y * segmentDir.x;

	if (std::abs(det) < 0.00001f) return sf::Vector2f(-1, -1); // No intersection

	float t = (toSegment.x * segmentDir.y - toSegment.y * segmentDir.x) / det;
	float u = (toSegment.x * rayDirection.y - toSegment.y * rayDirection.x) / det;

	if (t >= 0 && u >= 0 && u <= 1) {
		return rayOrigin + t * rayDirection;
	}

	return sf::Vector2f(-1, -1); // No intersection
}

int CustomEntryPointRayCast() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Raycasting Demo");

	std::vector<sf::RectangleShape> obstacles;

	// Create some obstacles
	sf::RectangleShape rect1(sf::Vector2f(200, 50));
	rect1.setPosition(150, 200);
	rect1.setFillColor(sf::Color::White);
	obstacles.push_back(rect1);

	sf::RectangleShape rect2(sf::Vector2f(100, 100));
	rect2.setPosition(400, 400);
	rect2.setFillColor(sf::Color::White);
	obstacles.push_back(rect2);

	sf::RectangleShape rect3(sf::Vector2f(150, 75));
	rect3.setPosition(600, 150);
	rect3.setFillColor(sf::Color::White);
	obstacles.push_back(rect3);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

		window.clear();

		// Draw obstacles
		for (const auto &obstacle : obstacles) {
			window.draw(obstacle);
		}

		// Cast rays
		for (float angle = 0; angle < 360; angle += 1.0f) {
			float rad = angle * 3.14159265f / 180.0f;
			sf::Vector2f rayDir(std::cos(rad), std::sin(rad));

			sf::Vector2f closestPoint(-1, -1);
			float closestDist = std::numeric_limits<float>::max();

			for (const auto &obstacle : obstacles) {
				sf::Vector2f obstaclePoints[4] = {
					obstacle.getPosition(),
					obstacle.getPosition() + sf::Vector2f(obstacle.getSize().x, 0),
					obstacle.getPosition() + sf::Vector2f(0, obstacle.getSize().y),
					obstacle.getPosition() + obstacle.getSize()
				};

				for (int i = 0; i < 4; ++i) {
					sf::Vector2f intersection = raycast(mousePos, rayDir, obstaclePoints[i], obstaclePoints[(i + 1) % 4]);

					if (intersection.x != -1 && intersection.y != -1) {
						float dist = std::hypot(intersection.x - mousePos.x, intersection.y - mousePos.y);
						if (dist < closestDist) {
							closestDist = dist;
							closestPoint = intersection;
						}
					}
				}
			}

			if (closestPoint.x != -1 && closestPoint.y != -1) {
				sf::Vertex line[] = {
					sf::Vertex(mousePos, sf::Color::Red),
					sf::Vertex(closestPoint, sf::Color::Red)
				};
				window.draw(line, 2, sf::Lines);
			}
		}

		window.display();
	}

	return 0;
}
