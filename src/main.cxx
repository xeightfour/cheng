#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader.hxx>
#include <texture/texture.hxx>

#define ZERO (float)0.0

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);
void handleInput(GLFWwindow *window);
void keyCallback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 800;

const GLfloat pi = acos(-1);

bool paused = false;
GLfloat pausedTime = ZERO;

GLfloat speed = 0.8;
GLfloat speedMul = ZERO;
GLfloat overload = ZERO;
GLfloat angle = ZERO;

int main() {
    std::cout << "Hello shader!" << std::endl;
    std::cout << "Press <ESC> or Q to kill the program(:" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "[ERROR] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Initialize glew
    if (glewInit()) {
        std::cout << "[ERROR] Failed to initialize glew" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create shader
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    // Create textures
    Texture textureWall("assets/wall.jpg");
    Texture textureWood("assets/wood.jpg");

    // Define vertices and their attributes
    GLfloat vertices[] = {
        +0.5f, +0.5f, 0.0f,
        +0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, +0.5f, 0.0f,
        +0.0f, +0.0f, 0.0f,
        +1.0f, +0.0f, 0.0f,
        +0.0f, +1.0f, 0.0f,
        -1.0f, +0.0f, 0.0f,
        +0.0f, -1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 6,
        4, 7, 8
    };

    GLfloat colors[] = {
        0.031f, 0.851f, 0.839f,
        1.000f, 0.180f, 0.388f,
        0.031f, 0.851f, 0.839f,
        1.000f, 0.180f, 0.388f,
        0.031f, 0.851f, 0.839f,
        1.000f, 0.180f, 0.388f,
        1.000f, 0.180f, 0.388f,
        1.000f, 0.180f, 0.388f,
        1.000f, 0.180f, 0.388f
    };

    GLfloat coords[] = {
        +0.5f, +0.5f,
        +0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f, +0.5f,
        +0.0f, +0.0f,
        +1.0f, +0.0f,
        +0.0f, +1.0f,
        -1.0f, +0.0f,
        +0.0f, -1.0f
    };

    // Create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Create vertex buffers
    GLuint VBO, EBO, COL, TEX;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &COL);
    glGenBuffers(1, &TEX);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attach VBO to VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind COL
    glBindBuffer(GL_ARRAY_BUFFER, COL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Attach COL to VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    // Bind TEX
    glBindBuffer(GL_ARRAY_BUFFER, TEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), coords, GL_STATIC_DRAW);

    // Attach TEX to VAO
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    // Just in case . . .
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat frameTime = 1.0f / 90.0f;
    GLfloat lastFrame = glfwGetTime();
    GLfloat objectTime = lastFrame;

    while(!glfwWindowShouldClose(window)) {
        GLfloat timeValue = glfwGetTime();

        // Set a limit of 90 frames per second
        if (timeValue - lastFrame < frameTime) {
            continue;
        }
        lastFrame = timeValue;

        glClearColor(0.145f, 0.165f, 0.204f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        handleInput(window);

        // Bind textures
        textureWall.bindAsUnit(0);
        textureWood.bindAsUnit(1);

        glBindVertexArray(VAO);
        shader.activate();

        // Set up shader textures
        shader.setInt("ourTextureWall", 0);
        shader.setInt("ourTextureWood", 1);

        // Update object time considering paused moments
        if (paused) {
            pausedTime = timeValue - objectTime;
        }
        objectTime = timeValue - pausedTime;

        // Change of color
        GLfloat redValue = sin(objectTime) / 2.0f + 0.5f;
        shader.setFloat("redValue", redValue);

        // Rotation
        if (speedMul != ZERO) {
            speed *= speedMul;
            overload += (angle - overload) * (1.0f - speedMul);
            speedMul = ZERO;
        }
        angle = objectTime * 2.0f * pi * speed + overload;
        shader.setFloat("cos", cos(angle));
        shader.setFloat("sin", sin(angle));

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);

        glBindVertexArray(0);
        Texture::unBindUnits();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &COL);
    glDeleteBuffers(1, &TEX);

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void keyCallback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods) {
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        paused ^= true;
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        speedMul = 1.6;
        std::cout << "Rotation speed: " << speed * speedMul << std::endl;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        speedMul = 1.0f / 1.6f;
        std::cout << "Rotation speed: " << speed * speedMul << std::endl;
    }
}
