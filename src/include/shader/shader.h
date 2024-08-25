#pragma once

#include <GL/glew.h>

#include <print>
#include <string>
#include <sstream>
#include <fstream>

struct Shader {
	GLuint ID;

	Shader(const char *vertexPath, const char *fragmentPath) {
		std::ifstream vertexFile, fragmentFile;
		std::string vertexCode, fragmentCode;

		// Ensure streams can throw exceptions
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);

			std::stringstream vertexStream, fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();
			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();

			vertexFile.close();
			fragmentFile.close();
		} catch (const std::ifstream::failure &ERR) {
			std::print("[ERROR] SHADER FILE NOT SUCCESSFULLY LOAD\n");
		}

		const char *vertexShaderSource = vertexCode.data();
		const char *fragmentShaderSource = fragmentCode.data();

		GLint success;
		char infoLog[512];

		GLuint vertexShader, fragmentShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::print("[ERROR] SHADER VERTEX COMPILATION FAILED\n{}\n", infoLog);
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::print("[ERROR] SHADER FRAGMENT COMPILATION FAILED\n{}\n", infoLog);
		}

		// Link all to a program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::print("[ERROR] SHADER PROGRAM LINKING FAILED\n{}\n", infoLog);
		}

		// Delete shaders <:
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
