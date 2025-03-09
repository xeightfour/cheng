#pragma once

#include <GL/glew.h>

#include <acs.hpp>

#include <string>
#include <stdexcept>
#include <iostream>

class Texture {
public:
	Texture() {
		glGenTextures(1, &ID);
	}

	~Texture() {
		glDeleteTextures(1, &ID);
	}

	void loadFromData(const unsigned char* data, int width, int height, int channels) {
		glBindTexture(GL_TEXTURE_2D, ID);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // For RGB data

		// Texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void loadFromFile(const std::string& filename) {
		try {
			// Load the JPEG
			JpegLoader loader(filename);

			// Create and load the texture
			Texture texture;
			loadFromData(loader.getData().data(), loader.getWidth(), loader.getHeight(), loader.getChannels());
		} catch (const std::exception& e) {
			std::cerr << "[ERROR] " << e.what() << std::endl;
			return;
		}
	}

	void bindAsUnit(GLuint textureUnit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	static void unBindUnits(GLuint first = 0, GLuint last = 16) {
		for (GLuint i = first; i < last; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
	}

private:
	GLuint ID;
};
