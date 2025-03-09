#version 460 core

in vec3 our_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform float brightness;
uniform bool check_texture;

uniform sampler2D our_texture;

void main() {
	vec3 color = brightness * our_color;
	if (check_texture) {
		frag_color = texture(our_texture, tex_coord) * vec4(color, 1.0);
	} else {
		frag_color = vec4(color, 1.0);
	}
}
