#version 460 core

in vec3 ourColor;

out vec4 fragColor;

uniform float redValue;

void main() {
    fragColor = vec4(redValue, ourColor.y, ourColor.z, 1.0f);
}
