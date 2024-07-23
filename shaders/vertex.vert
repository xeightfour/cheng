#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform float cos;
uniform float sin;

// Rotation axis
uniform float Rx, Ry, Rz;

void main() {
    // Define rotation parameters
    vec4 outPos;
    mat4 rotate = mat4(
        cos + Rx * Rx * (1.0F - cos), Rx * Ry * (1.0F - cos) - Rz * sin, Rx * Rz * (1.0F - cos) + Ry * sin, 0.0F,
        Rx * Ry * (1.0F - cos) + Rz * sin, cos + Ry * Ry * (1.0F - cos), Ry * Rz * (1.0F - cos) - Rx * sin, 0.0F,
        Rx * Rz * (1.0F - cos) - Ry * sin, Ry * Rz * (1.0F - cos) + Rx * sin, cos + Rz * Rz * (1.0F - cos), 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F
    );
    vec4 inPosw = vec4(inPos, 1.0F);

    outPos = rotate * inPosw;

    // Return outputs
    gl_Position = outPos;
    texCoord = inTexCoord;
    ourColor = inColor;
}
