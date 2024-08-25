#include <print>

GLfloat vertices[] = {
	-0.5F, -0.5F, -0.5F,
	+0.5F, -0.5F, -0.5F,
	+0.5F, +0.5F, -0.5F,
	+0.5F, +0.5F, -0.5F,
	-0.5F, +0.5F, -0.5F,
	-0.5F, -0.5F, -0.5F,

	-0.5F, -0.5F, +0.5F,
	+0.5F, -0.5F, +0.5F,
	+0.5F, +0.5F, +0.5F,
	+0.5F, +0.5F, +0.5F,
	-0.5F, +0.5F, +0.5F,
	-0.5F, -0.5F, +0.5F,

	-0.5F, +0.5F, +0.5F,
	-0.5F, +0.5F, -0.5F,
	-0.5F, -0.5F, -0.5F,
	-0.5F, -0.5F, -0.5F,
	-0.5F, -0.5F, +0.5F,
	-0.5F, +0.5F, +0.5F,

	+0.5F, +0.5F, +0.5F,
	+0.5F, +0.5F, -0.5F,
	+0.5F, -0.5F, -0.5F,
	+0.5F, -0.5F, -0.5F,
	+0.5F, -0.5F, +0.5F,
	+0.5F, +0.5F, +0.5F,

	-0.5F, -0.5F, -0.5F,
	+0.5F, -0.5F, -0.5F,
	+0.5F, -0.5F, +0.5F,
	+0.5F, -0.5F, +0.5F,
	-0.5F, -0.5F, +0.5F,
	-0.5F, -0.5F, -0.5F,

	-0.5F, +0.5F, -0.5F,
	+0.5F, +0.5F, -0.5F,
	+0.5F, +0.5F, +0.5F,
	+0.5F, +0.5F, +0.5F,
	-0.5F, +0.5F, +0.5F,
	-0.5F, +0.5F, -0.5F,

	-9.9F, +0.0F, -9.9F,
	-9.9F, +0.0F, +9.9F,
	+9.9F, +0.0F, -9.9F,
	+9.9F, +0.0F, +9.9F,

	+0.0F, -99.0F, +0.0F,
	+0.0F, +99.0F, +0.0F
};

// Move the cube upwards
for (int i = 0; i < 3 * 36; i += 3) {
	vertices[i] += 3.0F;
	vertices[i + 1] += 6.0F;
}

// Fuck EBO for now <;

GLfloat colors[] = {
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,
	1.0F, 1.0F, 1.0F,

	1.0F, 0.925F, 0.82F,
	1.0F, 0.925F, 0.82F,
	1.0F, 0.925F, 0.82F,
	1.0F, 0.925F, 0.82F,

	1.0F, 0.925F, 0.82F,
	1.0F, 0.925F, 0.82F
};

GLfloat coords[] = {
	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F,
	+0.5F, +0.5F,
	-0.5F, +0.5F,
	-0.5F, -0.5F,

	-0.5F, -0.5F,
	-0.5F, +0.5F,
	+0.5F, -0.5F,
	+0.5F, +0.5F
};
