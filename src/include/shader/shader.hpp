#pragma once

#include <GL/glew.h>

#include <print>
#include <string>
#include <string_view>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>

class Shader {
public:
	Shader(std::string_view vertexPath, std::string_view fragmentPath) {
		std::ifstream vertexFile {
			vertexPath.data(), std::ios::in
		};
		std::ifstream fragmentFile {
			fragmentPath.data(), std::ios::in
		};

		if (!vertexFile.is_open()) {
			throw std::runtime_error("Failed to open vertex shader file: " + std::string(vertexPath));
		}
		if (!fragmentFile.is_open()) {
			throw std::runtime_error("Failed to open fragment shader file: " + std::string(fragmentPath));
		}

		// Read files into strings
		std::stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		std::string vertexCode = vertexStream.str();
		std::string fragmentCode = fragmentStream.str();

		// Convert to null-terminated C strings for OpenGL
		const char* vertexShaderSource = vertexCode.c_str();
		const char* fragmentShaderSource = fragmentCode.c_str();

		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource, "vertex");
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource, "fragment");

		// Link program
		ID = glCreateProgram();
		if (ID == 0) {
			throw std::runtime_error("Failed to create shader program");
		}

		// RAII-like cleanup with scope guard
		struct ShaderCleanup {
			GLuint& vs;
			GLuint& fs;
			~ShaderCleanup() {
				glDeleteShader(vs);
				glDeleteShader(fs);
			}
		} cleanup { vertexShader, fragmentShader };

		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		GLint success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			std::string infoLog = getProgramInfoLog();
			throw std::runtime_error("Shader program linking failed:\n" + infoLog);
		}
	}

	// Deleted copy constructor/assignment to prevent OpenGL resource duplication
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	// Move constructor/assignment for resource ownership transfer
	Shader(Shader&& other) noexcept : ID(other.ID) {
		other.ID = 0;  // Mark moved-from object as invalid
	}
	Shader& operator=(Shader&& other) noexcept {
		if (this != &other) {
			glDeleteProgram(ID);  // Clean up old resource
			ID = other.ID;
			other.ID = 0;
		}
		return *this;
	}

	~Shader() {
		glDeleteProgram(ID);
	}

	void activate() const {
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (GLint)value);
	}

	void setInt(const std::string& name, GLint value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, GLfloat value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	GLuint getLoc(const std::string& name) const {
		GLint loc = glGetUniformLocation(ID, name.c_str());
		if (loc == -1) {
			std::print("[WARNING] Uniform '{}' not found in shader\n", name);
		}
		return static_cast<GLuint>(loc);
	}

private:
	GLuint ID;

	GLuint compileShader(GLenum type, const char* source, const std::string& shaderType) {
		GLuint shader = glCreateShader(type);
		if (shader == 0) {
			throw std::runtime_error("Failed to create " + shaderType + " shader");
		}

		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			std::string infoLog = getShaderInfoLog(shader);
			glDeleteShader(shader);  // Clean up on failure
			throw std::runtime_error("[ERROR] Shader " + shaderType + " compilation failed:\n" + infoLog);
		}
		return shader;
	}

	std::string getShaderInfoLog(GLuint shader) const {
		std::vector<char> infoLog(512);
		glGetShaderInfoLog(shader, infoLog.size(), nullptr, infoLog.data());
		return std::string(infoLog.begin(), infoLog.end());
	}

	std::string getProgramInfoLog() const {
		std::vector<char> infoLog(512);
		glGetProgramInfoLog(ID, infoLog.size(), nullptr, infoLog.data());
		return std::string(infoLog.begin(), infoLog.end());
	}
};
