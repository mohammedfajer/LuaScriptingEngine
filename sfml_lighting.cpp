#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

// Shader sources as raw string literals
const std::string vertexShaderSource = R"(
#version 120


void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}
)";

const std::string pointLightFragmentShaderSource = R"(
#version 120
uniform sampler2D texture;
uniform vec2 lightPosition;
uniform vec3 lightColor;
uniform float radius;


void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    vec2 fragCoord = gl_TexCoord[0].xy * vec2(800.0, 600.0); // Adjust to your screen size
    float distance = length(lightPosition - fragCoord);
    float intensity = max(1.0 - (distance / radius), 0.0); // Light radius of 100
    gl_FragColor = vec4(color.rgb * lightColor * intensity, color.a);
}
)";

const std::string directionalLightFragmentShaderSource = R"(
#version 120
uniform sampler2D texture;
uniform vec2 lightDirection;
uniform vec3 lightColor;


void main() {
    vec4 color = texture2D(texture,  gl_TexCoord[0].xy);
    float intensity = max(dot(normalize(lightDirection), vec2(1.0, 0.0)), 0.0);
    gl_FragColor = vec4(color.rgb * lightColor * intensity, color.a);
}
)";

const std::string spotLightFragmentShaderSource = R"(
#version 120
uniform sampler2D texture;
uniform vec2 lightPosition;
uniform float lightRadius;
uniform float lightAngle;
uniform vec3 lightColor;


void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    vec2 fragCoord = gl_TexCoord[0].xy * vec2(800.0, 600.0); // Adjust to your screen size
    vec2 direction = normalize(fragCoord - lightPosition);
    float distance = length(fragCoord - lightPosition);
    float angle = max(dot(direction, vec2(1.0, 0.0)), 0.0);
    float intensity = max(1.0 - (distance / lightRadius), 0.0) * max(angle, 0.0);
    gl_FragColor = vec4(color.rgb * lightColor * intensity, color.a);
}
)";


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Lighting");

	// Create shaders from source

	sf::Shader pointLightShader;
	if (!pointLightShader.loadFromMemory(vertexShaderSource, pointLightFragmentShaderSource))
	{
		std::cerr << "Failed to load pointLight Shader.\n";
	}
	sf::Shader directionalLightShader;
	if (!directionalLightShader.loadFromMemory(vertexShaderSource, directionalLightFragmentShaderSource))
	{
		std::cerr << "Failed to load directionalLight Shader.\n";
	}

	sf::Shader spotLightShader;
	if (!spotLightShader.loadFromMemory(vertexShaderSource, spotLightFragmentShaderSource))
	{
		std::cerr << "Failed to load spotLight Shader.\n";
	}




	// Create textures
	sf::Texture texture;
	if (!texture.loadFromFile("./data/braid.png")) {
		return -1; // Handle texture loading error
	}

	// Create sprites
	sf::Sprite sprite1(texture);
	sprite1.setPosition(200, 200);
	sprite1.scale(0.5, 0.5);

	sf::Sprite sprite2(texture);
	sprite2.setPosition(400, 400);
	sprite2.scale(0.5, 0.5);

	// Create light positions
	sf::Vector2f pointLightPos(300, 300);
	sf::Vector2f directionalLightDir(1.0f, 1.0f);
	sf::Vector2f spotLightPos(500, 300);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		// Render with point light
		pointLightShader.setUniform("lightPosition", sf::Glsl::Vec2(pointLightPos));
		pointLightShader.setUniform("lightColor", sf::Glsl::Vec3(1.0f, 0.0f, 1.0f));
		pointLightShader.setUniform("radius", 400.0f);
		window.draw(sprite1, &pointLightShader);

		// Render with directional light
		directionalLightShader.setUniform("lightDirection", sf::Glsl::Vec2(directionalLightDir));
		directionalLightShader.setUniform("lightColor", sf::Glsl::Vec3(1.0f, 1.0f, 1.0f));
		window.draw(sprite2, &directionalLightShader);

		// Render with spot light
	/*	spotLightShader.setUniform("lightPosition", sf::Glsl::Vec2(spotLightPos));
		spotLightShader.setUniform("lightRadius", 600.0f);
		spotLightShader.setUniform("lightAngle", 0.2f);
		spotLightShader.setUniform("lightColor", sf::Glsl::Vec3(1.0f, 1.0f, 1.0f));
		window.draw(sprite1, &spotLightShader);*/

		window.display();
	}

	return 0;
}
