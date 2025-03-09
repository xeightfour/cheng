#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_tex_coord;

out vec3 our_color;
out vec2 tex_coord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main() {
	gl_Position = projection * view * transform * vec4(in_pos, 1.0);
	tex_coord = in_tex_coord;
	our_color = in_color;
}
