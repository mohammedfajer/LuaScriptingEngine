#version 130

in vec2 texCoord; // Input texture coordinate
out vec4 fragColor; // Output color

uniform sampler2D texture; // Texture to sample from

void main() {
    vec4 color = texture(texture, texCoord); // Sample color from texture
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // Convert to grayscale
    fragColor = vec4(vec3(gray), color.a); // Output grayscale color
}
