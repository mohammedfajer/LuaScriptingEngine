#version 130

in vec2 vertexPosition; // Vertex position
in vec2 vertexTexCoord; // Vertex texture coordinate

out vec2 texCoord; // Output texture coordinate

void main() {
    gl_Position = vec4(vertexPosition, 0.0, 1.0); // Transform the vertex position
    texCoord = vertexTexCoord; // Pass texture coordinate to fragment shader
}
