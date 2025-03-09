#version 460 core

// Input variables from the vertex shader
in vec3 our_color;
in vec2 tex_coord;

// Output variable for the fragment color
out vec4 frag_color;

// Uniform variables for brightness, texture usage, and texture sampler
uniform float brightness;
uniform bool use_texture;
uniform sampler2D our_texture;

void main() {
	// Apply brightness to the input color
	vec3 color = brightness * our_color;

	// Determine the final fragment color based on texture usage
	if (use_texture) {
		frag_color = texture(our_texture, tex_coord) * vec4(color, 1.0);
	} else {
		frag_color = vec4(color, 1.0);
	}
}
