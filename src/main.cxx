#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader.h>
#include <texture/texture.h>

#include <iostream>

using std::endl;
using std::cout;

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);

void handleInput(GLFWwindow *window);

void keyCallback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);
void mouseCallback(GLFWwindow *window, GLdouble posX, GLdouble posY);
void scrollCallback(GLFWwindow *window, GLdouble offX, GLdouble offY);

constexpr GLfloat PI = std::acos(-1);

constexpr GLfloat frameTime = 1.0F / 90.0F;

GLint Width = 800;
GLint Height = 800;

bool paused;
GLfloat pausedTime;
GLfloat lastFrame;
GLfloat deltaTime;

GLfloat speed = 0.5F;
GLfloat speedMul;

GLfloat angle;
GLfloat angleExcess;

// Rotation
glm::vec4 R(-0.2F, 1.0F, -0.2F, 1.0F);
glm::mat4 transform;

// Camera
glm::vec3 cameraPos = glm::vec3(-10.0F, 5.0F, 0.0F);
glm::vec3 cameraDir = glm::vec3(1.0F, 0.0F, 0.0F);
glm::vec3 cameraUp = glm::vec3(0.0F, 1.0F, 0.0F);
GLfloat FOV = 45.0F;

// Mouse
bool initMouse = true;
GLfloat yaw;
GLfloat pitch;
GLfloat lastX = 400.0F;
GLfloat lastY = 400.0F;

int main() {
	cout << "Hello shader!" << endl;
	cout << "Press <ESC> or Q to kill the program <:" << endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Anti Aliasing
	glfwWindowHint(GLFW_SAMPLES, 8);
	glEnable(GL_MULTISAMPLE);

	GLFWwindow *window = glfwCreateWindow(Width, Height, "BlahBlahBlah", nullptr, nullptr);
	if (window == nullptr) {
		cout << "[ERROR] Failed to create GLFW window" << endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// Callback functions
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// Capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit()) {
		cout << "[ERROR] Failed to initialize GLEW" << endl;
		glfwTerminate();
		return 1;
	}

	Shader shader("lib/shaders/vertex.vert", "lib/shaders/fragment.frag");

	Texture wallText("assets/wall.jpg");
	Texture woodText("assets/wood.jpg");

	// What the fuck seriously
#include <data.dat>

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint VBO, COL, TEX;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &COL);
	glGenBuffers(1, &TEX);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, COL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, TEX);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Normalize rotation vector
	R /= glm::length(glm::vec3(R.x, R.y, R.z));

	lastFrame = glfwGetTime();
	GLfloat phoneyTime = lastFrame;

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {
		GLfloat timeValue = glfwGetTime();
		if (timeValue - lastFrame < frameTime) {
			continue;
		}
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		glClearColor(0.0F, 0.082F, 0.141F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Width, Height);

		handleInput(window);

		enum Text {
			kWall = 0,
			kWood
		};
		wallText.bindAsUnit(kWall);
		woodText.bindAsUnit(kWood);

		glBindVertexArray(VAO);
		shader.activate();

		if (paused) {
			pausedTime = timeValue - phoneyTime;
		}
		phoneyTime = timeValue - pausedTime;

		// Rotation speed
		if (speedMul != 0.0F) {
			speed *= speedMul;
			angleExcess += (angle - angleExcess) * (1.0F - speedMul); // I just don't get it /:
			speedMul = 0.0F;
		}

		// Rotation matrix
		angle = phoneyTime * 2.0F * PI * speed + angleExcess;
		GLfloat cos = std::cos(angle);
		GLfloat sin = std::sin(angle);
		transform = glm::mat4(
			cos + R.x * R.x * (1.0F - cos), R.x * R.y * (1.0F - cos) - R.z * sin, R.x * R.z * (1.0F - cos) + R.y * sin, 0.0F,
			R.x * R.y * (1.0F - cos) + R.z * sin, cos + R.y * R.y * (1.0F - cos), R.y * R.z * (1.0F - cos) - R.x * sin, 0.0F,
			R.x * R.z * (1.0F - cos) - R.y * sin, R.y * R.z * (1.0F - cos) + R.x * sin, cos + R.z * R.z * (1.0F - cos), 0.0F,
			0.0F, 0.0F, 0.0F, 1.0F
		);

		// Load rotation matrix to shader
		glUniformMatrix4fv(shader.getLoc("transform"), 1, GL_FALSE, glm::value_ptr(transform));

		// Camera stuff
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, 0.05F, 64.0F);

		// Load 3D to shader
		glUniformMatrix4fv(shader.getLoc("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader.getLoc("projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Change brightness
		GLfloat brightness = std::sin(phoneyTime) / 3.0F + 0.66F;
		shader.setFloat("brightness", brightness);

		// Draw to screen
		shader.setBool("checkTexture", true);
		shader.setInt("ourTexture", kWall);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		shader.setInt("ourTexture", kWood);
		glDrawArrays(GL_TRIANGLES, 18, 18);

		transform = glm::mat4(1.0F);
		glUniformMatrix4fv(shader.getLoc("transform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0F)));

		shader.setInt("ourTexture", kWall);
		glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);

		shader.setBool("checkTexture", false);
		glDrawArrays(GL_LINES, 40, 2);

		Texture::unBindUnits();
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Texture::unBindUnits();

	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &COL);
	glDeleteBuffers(1, &TEX);

	glfwTerminate();

	return 0;
}

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height) {
	Width = width;
	Height = height;
}

void handleInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Camera movement
	const GLfloat cameraSpeed = 2.5F * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraDir;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraDir;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
	}
}

void keyCallback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods) {
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		paused = ~paused;
	}
	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
		if (speed < 8.0F) {
			speedMul = 2.0F;
			cout << "Rotation speed: " << speed * speedMul << endl;
		}
	}
	if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
		if (speed > 0.05F) {
			speedMul = 0.5F;
			cout << "Rotation speed: " << speed * speedMul << endl;
		}
	}
}

void mouseCallback(GLFWwindow *window, GLdouble posX, GLdouble posY) {
	if (initMouse) {
		lastX = posX;
		lastY = posY;
		initMouse = false;
	}

	GLfloat offX = posX - lastX;
	GLfloat offY = lastY - posY;
	lastX = posX;
	lastY = posY;

	GLfloat sensitivity = 0.05F;
	offX *= sensitivity;
	offY *= sensitivity;

	yaw += offX;
	pitch += offY;

	if (pitch > 89.0F) {
		pitch = 89.0F;
	} else if (pitch < -89.0F) {
		pitch = -89.0F;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDir = glm::normalize(direction);
}

void scrollCallback(GLFWwindow *window, GLdouble offX, GLdouble offY) {
	FOV -= (GLfloat)offY * 3.0F;
	if (FOV < 1.0F) {
		FOV = 1.0F;
	} else if (FOV > 80.0F) {
		FOV = 80.0F;
	}
}
