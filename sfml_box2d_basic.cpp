#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <memory>
#include <cstdlib>
#include <cmath>

#include "physics.h"
#include <vector>
#include <string>

using namespace std;
using namespace sf;



int CustomEntryPointBox2D()
{
	RenderWindow window(VideoMode(800, 600), "SFML + Box2D Falling Shapes Example");
	window.setFramerateLimit(60);
	b2World world(b2Vec2(0.0, 9.8));

	// walls
	physics::createBox(world, 0, 570, 800, 30, b2_staticBody); // Floor



	Clock clock;
	while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Escape))
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			physics::createBox(world, Mouse::getPosition(window).x, Mouse::getPosition(window).y, 10, 10);
		}

		window.clear();
		physics::displayWorld(world, window);
		window.display();
	}

	return 0;
}
