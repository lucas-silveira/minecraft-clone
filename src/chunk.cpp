#include "chunk.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

const float kBlockSize = 0.5f;
const unsigned int kChunkSize = 16;

bool*** MakeChunk(void)
{
    bool*** chunk = new bool**[kChunkSize];
    for (int x = 0; x < kChunkSize; x++)
    {
        chunk[x] = new bool*[kChunkSize];
        for (int y = 0; y < kChunkSize; y++)
        {
            chunk[x][y] = new bool[kChunkSize];
            for (int z = 0; z < kChunkSize; z++)
                chunk[x][y][z] = false;
        }
    }
    return chunk;
}

void DeleteChunk(bool*** chunk)
{
    for (int i = 0; i < kChunkSize; ++i) {
        for (int j = 0; j < kChunkSize; ++j) {
            delete[] chunk[i][j];
        }
        delete[] chunk[i];
    }
    delete[] chunk;
}

BlockMesh MakeBlockMesh(float x, float y, float z)
{
    BlockMesh b = {
        {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 1.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 1.0f,

            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,

            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,

            x + kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,

            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,

            x + kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,
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

ChunkMesh MakeChunkMesh(bool*** chunk)
{
    ChunkMesh chunk_mesh;
    for (int x = 0; x < kChunkSize; x++)
        for (int y = 0; y < kChunkSize; y++)
            for (int z = 0; z < kChunkSize; z++)
            {
                if (chunk[x][y][z] == false) continue;
                BlockMesh blockMesh = MakeBlockMesh(x, y, z);

                for (int i = 0; i < sizeof(blockMesh.indices) / sizeof(unsigned); i++)
                {
                    chunk_mesh.indices.push_back(blockMesh.indices[i] + chunk_mesh.vertices.size() / 5);
                }
                
                for (int i = 0; i < sizeof(blockMesh.vertices)/sizeof(float); i++)
                {
                    chunk_mesh.vertices.push_back(blockMesh.vertices[i]);
                }
            }

    glGenBuffers(2, chunk_mesh.buffers);
    glGenVertexArrays(1, &chunk_mesh.ID);

    glBindVertexArray(chunk_mesh.ID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk_mesh.buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, chunk_mesh.vertices.size() * sizeof(float), &chunk_mesh.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk_mesh.buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk_mesh.indices.size() * sizeof(unsigned), &chunk_mesh.indices[0], GL_STATIC_DRAW);
    /* Position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return chunk_mesh;
}

void DeleteChunkMesh(ChunkMesh chunk)
{
    glDeleteVertexArrays(1, &chunk.ID);
    glDeleteBuffers(2, chunk.buffers);
}

void RenderChunk(ChunkMesh chunk, unsigned texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(chunk.ID);

    glDrawElements(GL_TRIANGLES, 36 * kChunkSize * kChunkSize * kChunkSize, GL_UNSIGNED_INT, 0);
}
