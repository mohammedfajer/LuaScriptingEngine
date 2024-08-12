uniform sampler2D texture;  // Texture to sample from
uniform float time;         // Time variable to control the pulse effect

void main() {
    // Lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Create a pulsating effect using a sine wave function
    float pulse = abs(sin(time)); // Create a pulsating effect

    // Modify the pixel color based on the pulse value
    gl_FragColor = pixel * pulse; // Multiply the color by the pulse value
}
