#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main() {
	gl_Position = projection * view * transform * vec4(inPos, 1.0F);
	texCoord = inTexCoord;
	ourColor = inColor;
}
