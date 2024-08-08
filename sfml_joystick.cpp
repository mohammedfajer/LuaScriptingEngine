//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <iostream>
//
//int main()
//{
//	// Create a window
//	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Joystick Example");
//
//	// Check if joystick is connected
//	if (!sf::Joystick::isConnected(0))
//	{
//		std::cerr << "Joystick not connected!" << std::endl;
//		return -1;
//	}
//
//	// Main loop
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		// Check joystick button status
//		if (sf::Joystick::isButtonPressed(0, 0)) // Joystick 0, Button 0
//		{
//			std::cout << "Button 0 is pressed!" << std::endl;
//		}
//
//		// Get joystick axis values
//		float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
//		float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
//
//		std::cout << "Joystick X: " << x << " Y: " << y << std::endl;
//
//		// Clear the window
//		window.clear();
//
//		// Display the window
//		window.display();
//	}
//
//	return 0;
//}
