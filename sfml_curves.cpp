#include <SFML/Graphics.hpp>
#include <cmath>

// Quadratic Bezier function
sf::Vector2f quadraticBezier(sf::Vector2f P0, sf::Vector2f P1, sf::Vector2f P2, float t) {
    float u = 1 - t;
    return u * u * P0 + 2 * u * t * P1 + t * t * P2;
}

int CustomEntryPointCurves() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Enemy Following Bezier Curve");

    sf::CircleShape enemy(10); // Enemy shape (a circle)
    enemy.setFillColor(sf::Color::Red);

    // Define control points for the Bezier curve
    sf::Vector2f P0(100, 500);
    sf::Vector2f P1(400, 100);
    sf::Vector2f P2(700, 500);

    float t = 0.0f; // Parameter for interpolation
    float speed = 0.001f; // Speed of the enemy along the path

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the parameter 't'
        t += speed;
        if (t > 1.0f) {
            t = 0.0f; // Loop the movement when reaching the end of the curve
        }

        // Calculate the enemy's position along the curve
        sf::Vector2f position = quadraticBezier(P0, P1, P2, t);
        enemy.setPosition(position);

        window.clear();
        window.draw(enemy);
        window.display();
    }

    return 0;
}
