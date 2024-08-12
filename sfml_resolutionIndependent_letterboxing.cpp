#include <SFML/Graphics.hpp>

// Function to adjust the view to maintain the aspect ratio and implement letterboxing
void setLetterboxView(sf::RenderWindow &window, sf::View &view, float targetAspectRatio) {
	// Get the current window size
	sf::Vector2u windowSize = window.getSize();
	float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);

	// Calculate new viewport dimensions to maintain aspect ratio
	float scaleFactor;
	float viewportWidth, viewportHeight;

	if (windowAspectRatio > targetAspectRatio) {
		// Window is wider than the target aspect ratio
		scaleFactor = static_cast<float>(windowSize.y) * targetAspectRatio;
		viewportWidth = scaleFactor;
		viewportHeight = windowSize.y;
	}
	else {
		// Window is taller than the target aspect ratio
		scaleFactor = static_cast<float>(windowSize.x) / targetAspectRatio;
		viewportWidth = windowSize.x;
		viewportHeight = scaleFactor;
	}

	// Center the viewport
	float viewportLeft = (windowSize.x - viewportWidth) / 2.0f;
	float viewportTop = (windowSize.y - viewportHeight) / 2.0f;

	// Update the view
	view.setViewport(sf::FloatRect(
		viewportLeft / windowSize.x,
		viewportTop / windowSize.y,
		viewportWidth / windowSize.x,
		viewportHeight / windowSize.y
	));

	// Set the view size and center
	view.setSize(viewportWidth, viewportHeight);
	view.setCenter(viewportWidth / 2.0f, viewportHeight / 2.0f);
}

int customEntryPointResolutionIndependent() {
	// Create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Letterboxing Example");

	// Define the target aspect ratio (e.g., 16:9)
	float targetAspectRatio = 16.0f / 9.0f;

	// Create the view
	sf::View view(sf::FloatRect(0, 0, 800, 600));

	// Set the initial view
	setLetterboxView(window, view, targetAspectRatio);

	// Create a red rectangle to visualize the content
	sf::RectangleShape contentRectangle(sf::Vector2f(200.f, 100.f));
	contentRectangle.setFillColor(sf::Color::Red);
	contentRectangle.setOrigin(contentRectangle.getSize() / 2.0f); // Center origin
	contentRectangle.setPosition(400.f, 300.f); // Will be updated later

	// Create a white rectangle for the content background
	sf::RectangleShape contentBackground;
	contentBackground.setFillColor(sf::Color::White);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Handle resizing
		if (event.type == sf::Event::Resized) {
			// Adjust view and viewport when the window is resized
			setLetterboxView(window, view, targetAspectRatio);
		}

		// Update rectangle position to be centered in the viewport
		contentRectangle.setPosition(view.getSize() / 2.0f);

		// Set content background size and position
		contentBackground.setSize(view.getSize());
		contentBackground.setPosition(0, 0);

		window.clear(sf::Color::Black); // Set the main background color to black

		// Set the view and draw the content background
		window.setView(view);
		window.draw(contentBackground); // Draw the white background of the viewport
		window.draw(contentRectangle);  // Draw the red rectangle in the viewport

		window.display();
	}

	return 0;
}
