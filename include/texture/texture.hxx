#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Texture {
    GLuint ID;

    Texture(const char *texturePath) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Load texture
        GLint width, height, nChannels;
        GLubyte *content = stbi_load(texturePath, &width, &height, &nChannels, 0);
        if (content) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, content);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "[ERROR] Failed to load texture" << std::endl;
        }

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Just free the image memory
        stbi_image_free(content);
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
    }
};
