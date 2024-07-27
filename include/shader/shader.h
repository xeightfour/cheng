#include <GL/glew.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

struct Shader {
    GLuint ID;

    Shader(const char *vertexPath, const char *fragmentPath) {
        // Strings containing shader source code
        std::string vertexCode, fragmentCode;

        // Streams to open shader files
        std::ifstream vertexFile, fragmentFile;

        // Ensure ifstream objects can throw exceptions
        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            std::stringstream vertexStream, fragmentStream;

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexFile.close();
            fragmentFile.close();

            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        } catch (const std::ifstream::failure &ERR) {
            std::cout << "[ERROR] SHADER FILE NOT SUCCESSFULLY LOAD" << std::endl;
        }

        // Convert source code to c-string
        const char *vertexShaderSource = vertexCode.data();
        const char *fragmentShaderSource = fragmentCode.data();

        GLint success;
        char infoLog[512];

        // Define shaders
        GLuint vertexShader, fragmentShader;

        // Create vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Check if successful
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "[ERROR] SHADER VERTEX COMPILATION FAILED\n" << infoLog << std::endl;
        }

        // Create fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Check if successful
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "[ERROR] SHADER FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
        }

        // Link all to a shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        // Check if linking was successful
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "[ERROR] SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
        }

        // Delete shaders(:
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void activate() {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.data()), (GLint)value);
    }

    void setInt(const std::string &name, GLint value) const {
        glUniform1i(glGetUniformLocation(ID, name.data()), value);
    }

    void setFloat(const std::string &name, GLfloat value) const {
        glUniform1f(glGetUniformLocation(ID, name.data()), value);
    }

    GLuint getLoc(const std::string &name) const {
        return glGetUniformLocation(ID, name.data());
    }
};
