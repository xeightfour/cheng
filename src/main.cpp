#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader.hpp>
#include <texture/texture.hpp>
#include <vertices.hpp>

#include <stdexcept>
#include <string_view>
#include <iostream>
#include <memory>
#include <utility>

constexpr GLint WINDOW_WIDTH = 800;
constexpr GLint WINDOW_HEIGHT = 600;
constexpr GLfloat CAMERA_SPEED = 2.5f;
constexpr GLfloat SENSITIVITY = 0.05f;
constexpr GLfloat PI = std::acos(-1);
constexpr GLfloat FRAME_TIME = 1.0f / 90.0f;

struct WindowDim {
	GLint width = WINDOW_WIDTH;
	GLint height = WINDOW_HEIGHT;
} winDim;

class GLFWWindow {
public:
	GLFWWindow(GLint width, GLint height, std::string_view title) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 16); // Anti-aliasing

		window.reset(glfwCreateWindow(width, height, title.data(), nullptr, nullptr));
		if (!window) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(window.get());

		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW");
		}
	}

	~GLFWWindow() {
		glfwTerminate();
	}

	GLFWwindow* get() const {
		return window.get();
	}

private:
	struct GLFWDeleter {
		void operator()(GLFWwindow* ptr) const { glfwDestroyWindow(ptr); }
	};
	std::unique_ptr<GLFWwindow, GLFWDeleter> window;
};

struct Camera {
	glm::vec3 pos = { -10.0f, 5.0f, 0.0f };
	glm::vec3 dir = { 1.0f, 0.0f, 0.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	GLfloat fov = 45.0f;
} camera;

struct Mouse {
	GLfloat lastX = WINDOW_WIDTH / 2.0f;
	GLfloat lastY = WINDOW_HEIGHT / 2.0f;
	GLfloat yaw = 0.0f;
	GLfloat pitch = 0.0f;
	bool firstMove = true;
} mouse;

struct Rotation {
	glm::vec4 axis = { -0.2f, 1.0f, -0.2f, 1.0f };
	GLfloat speed = 0.5f;
	GLfloat speedMul = 0.0f;
	GLfloat angle = 0.0f;
	GLfloat angleExcess = 0.0f;
} rotation;

bool paused = false;
GLfloat deltaTime = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, GLint width, GLint height) {
	winDim.width = width;
	winDim.height = height;
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, GLint key, GLint, GLint action, GLint) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(window, true);
		}
		if (key == GLFW_KEY_P) {
			paused = !paused;
		}
		if (key == GLFW_KEY_RIGHT_BRACKET && rotation.speed < 8.0f) {
			rotation.speedMul = 2.0f;
			std::cout << "Rotation speed: " << rotation.speed * rotation.speedMul << std::endl;
		}
		if (key == GLFW_KEY_LEFT_BRACKET && rotation.speed > 0.05f) {
			rotation.speedMul = 0.5f;
			std::cout << "Rotation speed: " << rotation.speed * rotation.speedMul << std::endl;
		}
	}
}

void mouseCallback(GLFWwindow*, GLdouble posX, GLdouble posY) {
	if (mouse.firstMove) {
		mouse.lastX = static_cast<GLfloat>(posX);
		mouse.lastY = static_cast<GLfloat>(posY);
		mouse.firstMove = false;
		return;
	}

	GLfloat offX = static_cast<GLfloat>(posX) - mouse.lastX;
	GLfloat offY = mouse.lastY - static_cast<GLfloat>(posY);
	mouse.lastX = static_cast<GLfloat>(posX);
	mouse.lastY = static_cast<GLfloat>(posY);

	mouse.yaw += offX * SENSITIVITY;
	mouse.pitch += offY * SENSITIVITY;

	mouse.pitch = glm::clamp(mouse.pitch, -89.0f, 89.0f);

	camera.dir = glm::normalize(glm::vec3 {
		cos(glm::radians(mouse.yaw)) * cos(glm::radians(mouse.pitch)),
		sin(glm::radians(mouse.pitch)),
		sin(glm::radians(mouse.yaw)) * cos(glm::radians(mouse.pitch))
		});
}

void scrollCallback(GLFWwindow*, GLdouble, GLdouble offY) {
	camera.fov -= static_cast<GLfloat>(offY) * 3.0f;
	camera.fov = glm::clamp(camera.fov, 1.0f, 80.0f);
}

void processInput(GLFWwindow* window) {
	GLfloat moveSpeed = CAMERA_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.pos += moveSpeed * camera.dir;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.pos -= moveSpeed * camera.dir;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.pos -= glm::normalize(glm::cross(camera.dir, camera.up)) * moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.pos += glm::normalize(glm::cross(camera.dir, camera.up)) * moveSpeed;
	}
}

int main() {
	std::cout << "Hello Shader!\n";
	std::cout << "Press <ESC> or Q to exit, P to pause, [ and ] to adjust rotation speed :>\n";

	try {
		GLFWWindow window(WINDOW_WIDTH, WINDOW_HEIGHT, "BlahBlahBlah");
		GLFWwindow* glfwWin = window.get();

		glfwSetFramebufferSizeCallback(glfwWin, framebufferSizeCallback);
		glfwSetKeyCallback(glfwWin, keyCallback);
		glfwSetCursorPosCallback(glfwWin, mouseCallback);
		glfwSetScrollCallback(glfwWin, scrollCallback);
		glfwSetInputMode(glfwWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glfwSwapInterval(1); // VSync

		Shader shader("lib/shaders/main.vert", "lib/shaders/main.frag");
		Texture wallText;
		wallText.loadFromFile("assets/wall.jpeg");
		Texture woodText;
		woodText.loadFromFile("assets/wood.jpeg");

		updateVertData();

		struct VertexBuffers {
			GLuint vao, vbo, col, tex;
			VertexBuffers() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
				glGenBuffers(1, &col);
				glGenBuffers(1, &tex);

				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, col);
				glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
				glEnableVertexAttribArray(1);

				glBindBuffer(GL_ARRAY_BUFFER, tex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
				glEnableVertexAttribArray(2);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			~VertexBuffers() {
				glDeleteVertexArrays(1, &vao);
				glDeleteBuffers(1, &vbo);
				glDeleteBuffers(1, &col);
				glDeleteBuffers(1, &tex);
			}
		} buffers;

		rotation.axis = glm::normalize(rotation.axis); // Normalize once at start

		GLfloat phoneyTime = 0.0f;
		GLfloat pausedTime = 0.0f;
		GLfloat lastFrame = static_cast<GLfloat>(glfwGetTime());

		while (!glfwWindowShouldClose(glfwWin)) {
			GLfloat timeValue = static_cast<GLfloat>(glfwGetTime());
			if (timeValue - lastFrame < FRAME_TIME) {
				continue;
			}
			deltaTime = timeValue - lastFrame;
			lastFrame = timeValue;

			processInput(glfwWin);

			glClearColor(0.0f, 0.082f, 0.141f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			enum class TextureUnit : GLint { Wall = 0, Wood };
			wallText.bindAsUnit(static_cast<GLint>(TextureUnit::Wall));
			woodText.bindAsUnit(static_cast<GLint>(TextureUnit::Wood));

			shader.activate();
			glBindVertexArray(buffers.vao);

			if (paused) {
				pausedTime = timeValue - phoneyTime;
			}
			phoneyTime = timeValue - pausedTime;

			if (rotation.speedMul != 0.0f) {
				rotation.speed *= rotation.speedMul;
				rotation.angleExcess += (rotation.angle - rotation.angleExcess) * (1.0f - rotation.speedMul);
				rotation.speedMul = 0.0f;
			}

			rotation.angle = phoneyTime * 2.0f * PI * rotation.speed + rotation.angleExcess;
			glm::mat4 transform = glm::rotate(glm::mat4(1.0f), rotation.angle, glm::vec3(rotation.axis));

			glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.dir, camera.up);
			glm::mat4 projection = glm::perspective(
				glm::radians(camera.fov),
				static_cast<float>(winDim.width) / static_cast<float>(winDim.height),
				0.05f, 64.0f
			);

			glUniformMatrix4fv(shader.getLoc("transform"), 1, GL_FALSE, glm::value_ptr(transform));
			glUniformMatrix4fv(shader.getLoc("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(shader.getLoc("projection"), 1, GL_FALSE, glm::value_ptr(projection));

			GLfloat brightness = std::sin(phoneyTime) / 3.0f + 0.66f;
			shader.setFloat("brightness", brightness);

			shader.setBool("use_texture", true);
			shader.setInt("our_texture", static_cast<GLint>(TextureUnit::Wall));
			glDrawArrays(GL_TRIANGLES, 0, 18);

			shader.setInt("our_texture", static_cast<GLint>(TextureUnit::Wood));
			glDrawArrays(GL_TRIANGLES, 18, 18);

			transform = glm::mat4(1.0f);
			glUniformMatrix4fv(shader.getLoc("transform"), 1, GL_FALSE, glm::value_ptr(transform));
			shader.setInt("our_texture", static_cast<GLint>(TextureUnit::Wall));
			glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);

			shader.setBool("use_texture", false);
			glDrawArrays(GL_LINES, 40, 2);

			Texture::unBindUnits();
			glBindVertexArray(0);

			glfwSwapBuffers(glfwWin);
			glfwPollEvents();
		}
	} catch (const std::exception& e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
