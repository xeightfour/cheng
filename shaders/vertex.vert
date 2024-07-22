#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform float cos;
uniform float sin;

void main() {
    gl_Position = vec4(cos * aPos.x - sin * aPos.y, sin * aPos.x + cos * aPos.y, aPos.z, 1.0f);
    texCoord = aTexCoord;
    ourColor = aColor;
}
