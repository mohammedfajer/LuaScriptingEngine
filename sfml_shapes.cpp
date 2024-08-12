#include <SFML/Graphics.hpp>

// Function to create a star shape
sf::ConvexShape createStar(float x, float y, float radius1, float radius2, int points) {
	sf::ConvexShape star;
	star.setPointCount(points * 2);

	for (int i = 0; i < points * 2; ++i) {
		float angle = i * 3.14159f / points;
		float radius = (i % 2 == 0) ? radius1 : radius2;
		star.setPoint(i, sf::Vector2f(x + std::cos(angle) * radius, y + std::sin(angle) * radius));
	}

	return star;
}

int CustomEntryPointShapes()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shapes Example");
	
	sf::Texture texture;
	if (!texture.loadFromFile("./data/bunny.jpg")) {
		// Handle error
	}

	// Create a filled star
	sf::ConvexShape filledStar = createStar(200, 300, 50, 100, 5);
	filledStar.setFillColor(sf::Color::Yellow);
	filledStar.setOutlineThickness(3);
	filledStar.setOutlineColor(sf::Color::Black);

	// Create a non-filled star (just the outline)
	sf::ConvexShape nonFilledStar = createStar(600, 300, 50, 100, 5);
	nonFilledStar.setFillColor(sf::Color::Transparent);
	nonFilledStar.setOutlineThickness(3);
	nonFilledStar.setOutlineColor(sf::Color::Red);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Basic Line
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(10, 10), sf::Color::Red),
			sf::Vertex(sf::Vector2f(150, 150), sf::Color::Blue)
		};
		window.draw(line, 2, sf::Lines);

		// Thick Line
		sf::RectangleShape thickLine(sf::Vector2f(100, 5));
		thickLine.setPosition(50, 50);
		thickLine.setFillColor(sf::Color::Green);
		thickLine.setRotation(45);
		window.draw(thickLine);

		// Triangle
		sf::ConvexShape triangle;
		triangle.setPointCount(3);
		triangle.setPoint(0, sf::Vector2f(100, 100));
		triangle.setPoint(1, sf::Vector2f(150, 50));
		triangle.setPoint(2, sf::Vector2f(200, 100));
		triangle.setFillColor(sf::Color::Yellow);
		window.draw(triangle);

		// Rectangle
		sf::RectangleShape rectangle(sf::Vector2f(120, 60));
		rectangle.setPosition(100, 200);
		rectangle.setFillColor(sf::Color::Cyan);
		rectangle.setOutlineThickness(5);
		rectangle.setOutlineColor(sf::Color::Black);
		window.draw(rectangle);

		// Arc
		sf::VertexArray arc(sf::TrianglesFan, 100);
		sf::Vector2f center(400, 300);
		float radius = 100.f;
		float startAngle = 0.f;
		float endAngle = 90.f;
		arc[0].position = center;
		arc[0].color = sf::Color::Magenta;
		for (int i = 1; i < 100; i++)
		{
			float angle = startAngle + (endAngle - startAngle) * i / (99);
			float rad = angle * 3.14159265f / 180.f;
			arc[i].position = center + sf::Vector2f(std::cos(rad) * radius, std::sin(rad) * radius);
			arc[i].color = sf::Color::Magenta;
		}
		window.draw(arc);

		// Convex Shape
		sf::ConvexShape convex;
		convex.setPointCount(6);
		convex.setPoint(0, sf::Vector2f(400, 200));
		convex.setPoint(1, sf::Vector2f(450, 100));
		convex.setPoint(2, sf::Vector2f(500, 200));
		convex.setPoint(3, sf::Vector2f(550, 150));
		convex.setPoint(4, sf::Vector2f(600, 200));
		convex.setPoint(5, sf::Vector2f(500, 250));
		convex.setFillColor(sf::Color::Blue);
		window.draw(convex);


		sf::CircleShape circle(50);
		circle.setPosition(300, 300);
		circle.setFillColor(sf::Color::Green);            // Fill color (background color)
		circle.setOutlineThickness(5);                    // Border thickness
		circle.setOutlineColor(sf::Color::Red);           // Border color (outline color)


		sf::RectangleShape backgroundRect(sf::Vector2f(120, 60));
		backgroundRect.setPosition(100, 100);
		backgroundRect.setFillColor(sf::Color::Red);

		sf::RectangleShape foregroundRect(sf::Vector2f(100, 40));
		foregroundRect.setPosition(110, 110);
		foregroundRect.setFillColor(sf::Color::White);

		window.draw(backgroundRect);
		window.draw(foregroundRect);

		window.draw(circle);


		sf::VertexArray gradientRect(sf::Quads, 4);
		gradientRect[0].position = sf::Vector2f(100, 100);
		gradientRect[1].position = sf::Vector2f(200, 100);
		gradientRect[2].position = sf::Vector2f(200, 200);
		gradientRect[3].position = sf::Vector2f(100, 200);

		gradientRect[0].color = sf::Color::Red;      // Top-left
		gradientRect[1].color = sf::Color::Green;    // Top-right
		gradientRect[2].color = sf::Color::Blue;     // Bottom-right
		gradientRect[3].color = sf::Color::Yellow;   // Bottom-left

		window.draw(gradientRect);


		

		sf::ConvexShape Rconvex;
		Rconvex.setPointCount(6);
		Rconvex.setPoint(0, sf::Vector2f(400, 200));
		Rconvex.setPoint(1, sf::Vector2f(450, 100));
		Rconvex.setPoint(2, sf::Vector2f(500, 200));
		Rconvex.setPoint(3, sf::Vector2f(550, 150));
		Rconvex.setPoint(4, sf::Vector2f(600, 200));
		Rconvex.setPoint(5, sf::Vector2f(500, 250));
		Rconvex.setTexture(&texture);                // Apply texture

		window.draw(Rconvex);


		// Textured Rectangle with Border
		sf::RectangleShape texturedRect(sf::Vector2f(120, 60));
		texturedRect.setPosition(300, 100);
		texturedRect.setTexture(&texture);
		texturedRect.setOutlineThickness(5);
		texturedRect.setOutlineColor(sf::Color::Black);
		window.draw(texturedRect);


		// Draw the filled star
		window.draw(filledStar);

		// Draw the non-filled star (outline only)
		window.draw(nonFilledStar);


		window.display();
	}

	return 0;
}
