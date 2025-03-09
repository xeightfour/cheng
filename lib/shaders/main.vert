#version 460 core

// Input attributes
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_tex_coord;

// Output variables to the fragment shader
out vec3 our_color;
out vec2 tex_coord;

// Uniform matrices for transformations
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main() {
	// Calculate the vertex position in clip space
	gl_Position = projection * view * transform * vec4(in_pos, 1.0);

	// Pass the texture coordinates and color to the fragment shader
	tex_coord = in_tex_coord;
	our_color = in_color;
}
