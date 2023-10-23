#include "texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#include "common.h"
#include "shader.h"

Texture textures[6] = {
        {
            0, "dirt", "assets/dirt.png",
        },
        {
            0, "grass", "assets/grass.png",
        },
        {
            0, "water", "assets/water.png",
        },
        {
            0, "stone", "assets/stone.png",
        },
        {
            0, "wood", "assets/wood.png",
        },
        {
            0, "sand", "assets/sand.png",
        }
};

void LoadTextures(Shader &shader)
{
    shader.use();
    for (int i = 0; i < 6; i++)
    {
        glGenTextures(1, &textures[i].ID);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nr_channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(textures[i].path, &width, &height, &nr_channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        shader.setInt(textures[i].name, i);
    }
}

Texture* GetTextures()
{
    return textures;
}