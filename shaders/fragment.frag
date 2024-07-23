#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 fragColor;

uniform float redValue;

uniform sampler2D ourTextureWall;
uniform sampler2D ourTextureWood;

void main() {
    fragColor = mix(texture(ourTextureWall, texCoord), texture(ourTextureWood, texCoord), 0.70) * vec4(redValue, ourColor.y, ourColor.z, 1.0);
}
