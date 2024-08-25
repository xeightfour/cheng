#pragma once

#include <GL/glew.h>

#include <acs.h>

#include <print>

struct Texture {
	GLuint ID;

	Texture(const char *texturePath) {
		glGenTextures(1, &ID);

		glBindTexture(GL_TEXTURE_2D, ID);

		// Texture wrapping options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		Image *image = acsLoadImageFile(texturePath);
		if (image != nullptr) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::print("[ERROR] TEXTURE FAILED TO LOAD IMAGE\n");
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		acsFreeImage(image);
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
};
