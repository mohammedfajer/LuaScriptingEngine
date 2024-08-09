// 
// 
// int main() {
// 	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader Example");
// 
// 	// Create a render texture (canvas)
// 	sf::RenderTexture renderTexture;
// 	renderTexture.create(800, 600);
// 
// 	// Define shaders as raw strings
// 	const std::string vertexShaderCode = R"(
//         void main()
//         {
//             // Transform the vertex position
//             gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
// 
//             // Transform the texture coordinates
//             gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
// 
//             // Forward the vertex color
//             gl_FrontColor = gl_Color;
//         }
//     )";
// 
// 	const std::string fragmentShaderCode = R"(
//         uniform sampler2D texture;
//         uniform vec2 resolution;
//         uniform float vignetteIntensity;
//         uniform float waveStrength;
//         uniform float waveFrequency;
//         uniform float time;
// 
//         void main()
//         {
//             vec2 uv = gl_TexCoord[0].xy;
// 
//             // Apply wave distortion
//             float wave = sin(uv.y * waveFrequency + time * 0.5) * waveStrength;
//             vec2 distortedUV = uv + vec2(wave, 0.0);
// 
//             // Lookup the pixel in the texture with distorted coordinates
//             vec4 color = texture2D(texture, distortedUV);
// 
//             // Convert the color to grayscale using the luminance formula
//             float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
//             vec4 grayscaleColor = vec4(gray, gray, gray, color.a);
// 
//             // Calculate vignette effect
//             vec2 center = vec2(0.5, 0.5);
//             float distance = length(uv - center);
//             float vignette = smoothstep(0.7, 1.0, 1.0 - distance * vignetteIntensity);
// 
//             // Apply vignette effect
//             gl_FragColor = grayscaleColor * vignette;
//         }
//     )";
// 
// 	// Load shaders from raw strings
// 	sf::Shader shader;
// 	if (!shader.loadFromMemory(vertexShaderCode, fragmentShaderCode)) {
// 		std::cerr << "Failed to load shaders from memory" << std::endl;
// 		return -1;
// 	}
// 
// 	// Load texture and create sprite
// 	sf::Texture texture;
// 	if (!texture.loadFromFile("./data/bunny.jpg")) {
// 		return -1;
// 	}
// 	sf::Sprite sprite(texture);
// 
// 	// Set the shader's texture uniform to the current texture
// 	//shader.setUniform("texture", sf::Shader::CurrentTexture);
// 	  // Set shader uniforms
// 	shader.setUniform("resolution", sf::Glsl::Vec2(800, 600)); // Set the resolution of the image
// 	shader.setUniform("vignetteIntensity", 0.5f); // Set the intensity of the vignette effect
// 	shader.setUniform("waveStrength", 0.05f); // Set the strength of the wave effect
// 	shader.setUniform("waveFrequency", 10.0f); // Set the frequency of the wave effect
// 	sf::Clock clock; // To track time for animation
// 	// Main loop
// 	while (window.isOpen()) {
// 		sf::Event event;
// 		while (window.pollEvent(event)) {
// 			if (event.type == sf::Event::Closed)
// 				window.close();
// 		}
// 
// 		// Update time uniform
// 		float time = clock.getElapsedTime().asSeconds();
// 		shader.setUniform("time", time);
// 
// 		// Render the scene to the canvas
// 		renderTexture.clear(sf::Color::Transparent);
// 		renderTexture.draw(sprite);
// 		renderTexture.display();
// 
// 		// Clear the window
// 		window.clear();
// 
// 		// Draw the canvas with shader
// 		sf::Sprite canvasSprite(renderTexture.getTexture());
// 		window.draw(canvasSprite, &shader);
// 
// 		// Display the result
// 		window.display();
// 	}
// 
// 	return 0;
// 
// 
// }