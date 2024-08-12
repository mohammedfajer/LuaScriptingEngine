#include <SFML/Graphics.hpp>
#include <string>

void drawLeftAlignedText(sf::RenderWindow &window, const std::string &textStr, const sf::Font &font, float x, float y, unsigned int characterSize) {
	sf::Text text;
	text.setFont(font);
	text.setString(textStr);
	text.setCharacterSize(characterSize);
	text.setFillColor(sf::Color::White);

	// Get the local bounds of the text
	sf::FloatRect textBounds = text.getLocalBounds();

	// Set the text position to align it to the left
	// By default, the origin is the top-left corner of the text
	text.setOrigin(0, 0); // Align text to top-left
	text.setPosition(x, y);

	// Draw the text
	window.draw(text);
}

void drawRightAlignedText(sf::RenderWindow &window, const std::string &textStr, const sf::Font &font, float x, float y, unsigned int characterSize) {
	sf::Text text;
	text.setFont(font);
	text.setString(textStr);
	text.setCharacterSize(characterSize);
	text.setFillColor(sf::Color::White);

	// Get the local bounds of the text
	sf::FloatRect textBounds = text.getLocalBounds();

	// Set the origin to the right side of the text
	text.setOrigin(textBounds.width, 0); // Align text to top-right
	text.setPosition(x, y);

	// Draw the text
	window.draw(text);
}

// Function to draw text with specified alignment within a given width
// Function to draw text aligned within a bounding width
// Function to draw text aligned within a bounding width and draw the bounding box
void drawAlignedTextWithBox(sf::RenderWindow &window, const std::string &textStr, const sf::Font &font, float x, float y, float boxWidth, unsigned int characterSize, const std::string &alignment) {
	sf::Text text;
	text.setFont(font);
	text.setString(textStr);
	text.setCharacterSize(characterSize);
	text.setFillColor(sf::Color::White);

	// Ensure text is updated to compute bounds correctly
	text.setString(textStr);

	// Get the global bounds of the text
	sf::FloatRect textBounds = text.getGlobalBounds();

	// Calculate text width
	float textWidth = textBounds.width;

	// Calculate the offset based on the alignment
	float offsetX = 0.0f;
	if (alignment == "left") {
		offsetX = 0; // Left alignment
	}
	else if (alignment == "right") {
		offsetX = boxWidth - textWidth; // Right alignment
	}
	else if (alignment == "center") {
		offsetX = (boxWidth - textWidth) / 2; // Center alignment
	}
	else {
		// Default to left alignment if an unknown alignment is specified
		offsetX = 0;
	}

	// Draw the bounding box
	sf::RectangleShape boundingBox(sf::Vector2f(boxWidth, textBounds.height + 5));
	boundingBox.setFillColor(sf::Color(0, 0, 0, 50)); // Semi-transparent fill
	boundingBox.setOutlineColor(sf::Color::Red); // Outline color
	boundingBox.setOutlineThickness(1.0f); // Outline thickness
	boundingBox.setPosition(x, y); // Position the bounding box

	// Set the origin to the top-left corner
	text.setOrigin(0, 0);

	// Set the position of the text
	text.setPosition(x + offsetX, y);

	// Draw the bounding box and the text
	window.draw(boundingBox);
	window.draw(text);
}

void drawTextWithNearestFilter(sf::RenderWindow &window, const std::string &textStr, const sf::Font &font, float x, float y, unsigned int characterSize) {
	// Create a RenderTexture
	sf::RenderTexture renderTexture;
	renderTexture.create(800, 600); // Create a texture of sufficient size

	// Create text and set its properties
	sf::Text text;
	text.setFont(font);
	text.setString(textStr);
	text.setCharacterSize(characterSize);
	text.setFillColor(sf::Color::White);

	// Draw the text to the RenderTexture
	renderTexture.clear(sf::Color::Transparent);
	renderTexture.draw(text);
	renderTexture.display();

	// Create a sprite from the RenderTexture
	sf::Sprite sprite(renderTexture.getTexture());

	// Set the filtering to nearest-neighbor
	//sprite.getTexture();
	renderTexture.setSmooth(false);
	

	// Set the position of the sprite
	sprite.setPosition(x, y);

	// Draw the sprite with the applied nearest-neighbor filter
	window.draw(sprite);
}

int TEXT_ALIGNMENT() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Alignment");

	sf::Font font;
	if (!font.loadFromFile("./data/arial.ttf")) {
		return -1; // Error loading font
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Draw left-aligned text
		/*drawLeftAlignedText(window, "Hello, SFML!", font, 50, 100, 30);
		drawRightAlignedText(window, "Hello, SFML!", font, 200, 140, 30);*/


		// Define the width of the box where the text will be aligned
		float boxWidth = 200.0f;

		// Draw text with different alignments
	   // Draw text with different alignments
		// Draw text with different alignments and their bounding boxes
		drawAlignedTextWithBox(window, "Hello, World", font, 50, 100, boxWidth, 30, "left");   // Left aligned
		drawAlignedTextWithBox(window, "Hello, World", font, 50, 200, boxWidth, 30, "center"); // Center aligned
		drawAlignedTextWithBox(window, "Hello, World", font, 50, 240, boxWidth, 30, "right");  // Right aligned

		drawAlignedTextWithBox(window, "LETS GO!", font, 0, 600 / 2.0f, 800, 30, "center");

		drawTextWithNearestFilter(window, "Hello, World", font, 20, 50, 50);


		window.display();
	}

	return 0;
}
