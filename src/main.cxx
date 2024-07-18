// Triangles, colors, uniforms, shader as a class((:
// The code below uses a separate VBO for color codes

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include <shader/shader.h>

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);
void handleInput(GLFWwindow *window);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 800;

int main() {
    std::cout << "Hello shader!" << std::endl;
    std::cout << "Press <ESC> to kill the program(:" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

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
        1.000f, 0.180f, 0.388f,
        1.000f, 0.180f, 0.388f,
        0.031f, 0.851f, 0.839f,
        1.000f, 0.180f, 0.388f,
        0.031f, 0.851f, 0.839f
    };

    // Set up vertex buffers
    GLuint VBO, VAO, EBO, COLORS;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &COLORS);

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

    // Bind COLORS
    glBindBuffer(GL_ARRAY_BUFFER, COLORS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Attach COLORS to VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    // Just in case . . .
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        handleInput(window);

        glClearColor(0.145f, 0.165f, 0.204f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        shader.activate();

        GLfloat timeValue = glfwGetTime();
        GLfloat redValue = sin(timeValue) / 2.0f + 0.5f;
        shader.setFloat("redValue", redValue);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);
        // glDrawArrays(GL_TRIANGLES, 3, 3); oops!

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &COLORS);

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
