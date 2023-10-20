#include "chunk.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

const float BLOCK_SIZE = 0.5f;
const unsigned int CHUNK_SIZE = 16;

bool*** makeChunk(void)
{
    bool*** chunk = new bool**[CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        chunk[x] = new bool*[CHUNK_SIZE];
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            chunk[x][y] = new bool[CHUNK_SIZE];
            for (int z = 0; z < CHUNK_SIZE; z++)
                chunk[x][y][z] = false;
        }
    }
    return chunk;
}

void deleteChunk(bool*** chunk)
{
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            delete[] chunk[i][j];
        }
        delete[] chunk[i];
    }
    delete[] chunk;
}

blockMesh makeBlockMesh(float x, float y, float z)
{
    blockMesh b = {
        {
            x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 0.0f,
            x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 0.0f,
            x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 1.0f,
            x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 1.0f,

            x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 0.0f,
            x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 0.0f,
            x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 1.0f,
            x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 1.0f,

            x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 0.0f,
            x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 0.0f,
            x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 1.0f,
            x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 1.0f,

            x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 0.0f,
            x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 0.0f,
            x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 1.0f,
            x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 1.0f,

            x - BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 0.0f,
            x + BLOCK_SIZE, y - BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 0.0f,
            x + BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 1.0f,
            x - BLOCK_SIZE, y - BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 1.0f,

            x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  0.0f, 0.0f,
            x - BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE,  1.0f, 0.0f,
            x - BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  1.0f, 1.0f,
            x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE,  0.0f, 1.0f,
        },
        {
            // Back
            0, 3, 2,
            2, 1, 0,
            // Front
            4, 5, 6,
            6, 7 ,4,
            // Left
            11, 8, 9,
            9, 10, 11,
            // Right
            12, 13, 14,
            14, 15, 12,
            // Bottom
            16, 17, 18,
            18, 19, 16,
            // Top
            20, 21, 22,
            22, 23, 20
        }
    };
    return b;
}

chunkMesh makeChunkMesh(bool*** chunk)
{
    chunkMesh chunkMesh;
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (chunk[x][y][z] == false) continue;
                blockMesh blockMesh = makeBlockMesh(x, y, z);

                for (int i = 0; i < sizeof(blockMesh.indices) / sizeof(unsigned); i++)
                {
                    chunkMesh.indices.push_back(blockMesh.indices[i] + chunkMesh.vertices.size() / 5);
                }
                
                for (int i = 0; i < sizeof(blockMesh.vertices)/sizeof(float); i++)
                {
                    chunkMesh.vertices.push_back(blockMesh.vertices[i]);
                }
            }

    glGenBuffers(2, chunkMesh.buffers);
    glGenVertexArrays(1, &chunkMesh.ID);

    glBindVertexArray(chunkMesh.ID);

    glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, chunkMesh.vertices.size() * sizeof(float), &chunkMesh.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkMesh.indices.size() * sizeof(unsigned), &chunkMesh.indices[0], GL_STATIC_DRAW);
    /* Position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return chunkMesh;
}

void deleteChunkMesh(chunkMesh chunk)
{
    glDeleteVertexArrays(1, &chunk.ID);
    glDeleteBuffers(2, chunk.buffers);
}

void renderChunk(chunkMesh chunk, unsigned texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(chunk.ID);

    glDrawElements(GL_TRIANGLES, 36 * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, GL_UNSIGNED_INT, 0);
}
