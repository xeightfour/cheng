#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 fragColor;

uniform float brightness;
uniform bool checkTexture;

uniform sampler2D ourTexture;

void main() {
	vec3 color = brightness * ourColor;
	if (checkTexture) {
		fragColor = texture(ourTexture, texCoord) * vec4(color, 1.0F);
	} else {
		fragColor = vec4(color, 1.0F);
	}
}
