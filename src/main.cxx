#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader.h>
#include <texture/texture.h>

void framebufferSizeCallback(GLFWwindow *window, GLint width, GLint height);

void handleInput(GLFWwindow *window);

void keyCallback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);

constexpr int GLFW_FAIL = -3;
constexpr int GLEW_FAIL = -4;

constexpr GLfloat ZERO = 0.0F;

constexpr GLuint SCREEN_WIDTH = 800;
constexpr GLuint SCREEN_HEIGHT = 800;

constexpr GLfloat PI = std::acos(-1);

// Duration of each frame (90 FPS)
constexpr GLfloat frameTime = 1.0F / 90.0F;

// Timing
bool paused = false;
GLfloat pausedTime = ZERO;

// Rotation speed
GLfloat speed = 0.8F;
GLfloat speedMul = ZERO;

// Rotation angle
GLfloat angleExcess = ZERO;
GLfloat angle = ZERO;

// Rotation axis & matrix
glm::vec4 R(1.0F, 1.0F, 1.0F, 1.0F);
glm::mat4 trans;

int main() {
    std::cout << "Hello shader!" << std::endl;
    std::cout << "Press <ESC> or Q to kill the program(:" << std::endl;

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create & activate GLFW window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "[ERROR] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return GLFW_FAIL;
    }
    glfwMakeContextCurrent(window);

    // Set GLFW Callback functions
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Initialize glew
    if (glewInit()) {
        std::cout << "[ERROR] Failed to initialize glew" << std::endl;
        glfwTerminate();
        return GLEW_FAIL;
    }

    // Create shader
    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    // Create textures
    Texture textureWall("assets/wall.jpg");
    Texture textureWood("assets/wood.jpg");

    // Define vertices and their attributes
    GLfloat vertices[] = {
        +0.5F, +0.5F, 0.0F,
        +0.5F, -0.5F, 0.0F,
        -0.5F, -0.5F, 0.0F,
        -0.5F, +0.5F, 0.0F,
        +0.0F, +0.0F, 0.0F,
        +1.0F, +0.0F, 0.0F,
        +0.0F, +1.0F, 0.0F,
        -1.0F, +0.0F, 0.0F,
        +0.0F, -1.0F, 0.0F
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 6,
        4, 7, 8
    };

    GLfloat colors[] = {
        0.031F, 0.851F, 0.839F,
        1.000F, 0.180F, 0.388F,
        0.031F, 0.851F, 0.839F,
        1.000F, 0.180F, 0.388F,
        0.031F, 0.851F, 0.839F,
        0.031F, 0.851F, 0.839F,
        1.000F, 0.180F, 0.388F,
        1.000F, 0.180F, 0.388F,
        1.000F, 0.180F, 0.388F
    };

    GLfloat coords[] = {
        +0.5F, +0.5F,
        +0.5F, -0.5F,
        -0.5F, -0.5F,
        -0.5F, +0.5F,
        +0.0F, +0.0F,
        +1.0F, +0.0F,
        +0.0F, +1.0F,
        -1.0F, +0.0F,
        +0.0F, -1.0F
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    // Bind COL
    glBindBuffer(GL_ARRAY_BUFFER, COL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Attach COL to VAO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);

    // Bind TEX
    glBindBuffer(GL_ARRAY_BUFFER, TEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

    // Attach TEX to VAO
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(2);

    // Disconnect GL_ARRAY_BUFFER and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Last frame rendered
    GLfloat lastFrame = glfwGetTime();

    // Observed time
    GLfloat phoneyTime = lastFrame;

    // Normalize rotation axis
    GLfloat Rlen = glm::length(glm::vec3(R.x, R.y, R.z));
    R /= Rlen;

    while(!glfwWindowShouldClose(window)) {
        GLfloat timeValue = glfwGetTime();

        // Set a limit of 90 FPS
        if (timeValue - lastFrame < frameTime) {
            continue;
        }
        lastFrame = timeValue;

        // Set background color
        glClearColor(0.145F, 0.165F, 0.204F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        handleInput(window);

        // Bind textures
        textureWall.bindAsUnit(0);
        textureWood.bindAsUnit(1);

        glBindVertexArray(VAO);

        shader.activate();

        // Add textures to shader
        shader.setInt("ourTextureWall", 0);
        shader.setInt("ourTextureWood", 1);

        // Update observed time
        if (paused) {
            pausedTime = timeValue - phoneyTime;
        }
        phoneyTime = timeValue - pausedTime;

        // Change of color
        GLfloat redValue = sin(phoneyTime) / 2.0F + 0.5F;
        shader.setFloat("redValue", redValue);

        // Rotation speed
        if (speedMul != ZERO) {
            speed *= speedMul;
            angleExcess += (angle - angleExcess) * (1.0F - speedMul);
            speedMul = ZERO;
        }

        // Rotation angle
        angle = phoneyTime * 2.0F * PI * speed + angleExcess;
        GLfloat cos = std::cos(angle);
        GLfloat sin = std::sin(angle);
        trans = glm::mat4(
            cos + R.x * R.x * (1.0F - cos), R.x * R.y * (1.0F - cos) - R.z * sin, R.x * R.z * (1.0F - cos) + R.y * sin, 0.0F,
            R.x * R.y * (1.0F - cos) + R.z * sin, cos + R.y * R.y * (1.0F - cos), R.y * R.z * (1.0F - cos) - R.x * sin, 0.0F,
            R.x * R.z * (1.0F - cos) - R.y * sin, R.y * R.z * (1.0F - cos) + R.x * sin, cos + R.z * R.z * (1.0F - cos), 0.0F,
            0.0F, 0.0F, 0.0F, 1.0F
        );

        // Load transformation to shader
        glUniformMatrix4fv(shader.getLoc("transform"), 1, GL_FALSE, glm::value_ptr(trans));

        // Just draw everything to screen
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // Free texture units
        Texture::unBindUnits();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate VAO
    glDeleteVertexArrays(1, &VAO);

    // Deallocate vertex buffers
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
    // Switch pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        paused ^= true;
    }

    // Increase speed
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        speedMul = 1.6F;
        std::cout << "Rotation speed: " << speed * speedMul << std::endl;
    }

    // Decrease speed
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        speedMul = 1.0F / 1.6F;
        std::cout << "Rotation speed: " << speed * speedMul << std::endl;
    }
}
