#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform float cos;
uniform float sin;

void main() {
    gl_Position = vec4(cos * inPos.x - sin * inPos.y, sin * inPos.x + cos * inPos.y, inPos.z, 1.0f);
    texCoord = inTexCoord;
    ourColor = inColor;
}
