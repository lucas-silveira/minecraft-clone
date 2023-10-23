#include "texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#include "common.h"

unsigned texture;

void LoadTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("assets/dirt.jpg", &width, &height, &nr_channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

unsigned GetTexture()
{
    return texture;
}

float* GetTextureCoord(BlockType type)
{
    float uv_coordinates[2];
    switch (type)
    {
        case BlockType_Dirt:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
        case BlockType_Grass:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
        case BlockType_Water:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
        case BlockType_Stone:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
        case BlockType_Wood:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
        case BlockType_Sand:
            uv_coordinates[0] = 1.f;
            uv_coordinates[1] = 1.f;
            break;
    }
    return uv_coordinates;
}