#include "chunk.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include <vector>

const float kBlockSize = 0.5f;
const unsigned kChunkSize = 16;
const unsigned kTerrainSize = 5;

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

BlockMesh MakeBlockMesh(
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
)
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;

    if (!back_neighbor)
    {
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        });
        indices.insert(indices.end(), {
            0, 3, 2,
            2, 1, 0
        });
    }
    if (!front_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        });
        indices.insert(indices.end(), {
            0+offset, 1+offset, 2+offset,
            2+offset, 3+offset, 0+offset
         });
    }

    if (!left_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
        });
        indices.insert(indices.end(), {
            3+offset, 0+offset, 1+offset,
            1+offset, 2+offset, 3+offset
        });
    }
    if (!right_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
        });
        indices.insert(indices.end(), {
            0+offset, 1+offset, 2+offset,
            2+offset, 3+offset, 0+offset
        });
    }

    if (!bottom_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  1.0f, -1.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  1.0f, -1.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,  1.0f, -1.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,  1.0f, -1.0f,  0.0f,
        });
        indices.insert(indices.end(), {
            0+offset, 1+offset, 2+offset,
            2+offset, 3+offset, 0+offset
        });
    }
    if (!top_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
        });
        indices.insert(indices.end(), {
            0+offset, 1+offset, 2+offset,
            2+offset, 3+offset, 0+offset
        });
    }
    
    BlockMesh b;
    b.vertices = vertices;
    b.indices = indices;

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

                bool left_neighbor = false, right_neighbor = false;
                bool bottom_neighbor = false, top_neighbor = false;
                bool back_neighbor = false, front_neighbor = false;

                if (x > 0) left_neighbor = chunk[x - 1][y][z];
                if (x < kChunkSize - 1) right_neighbor = chunk[x + 1][y][z];

                if (y > 0) bottom_neighbor = chunk[x][y - 1][z];
                if (y < kChunkSize - 1) top_neighbor = chunk[x][y + 1][z];

                if (z > 0) back_neighbor = chunk[x][y][z - 1];
                if (z < kChunkSize - 1) front_neighbor = chunk[x][y][z + 1];

                BlockMesh blockMesh = MakeBlockMesh(
                    x, y, z,
                    left_neighbor, right_neighbor,
                    bottom_neighbor, top_neighbor,
                    back_neighbor, front_neighbor
                );

                for (int i = 0; i < blockMesh.indices.size(); i++)
                {
                    unsigned offset = chunk_mesh.vertices.size() / 8;
                    chunk_mesh.indices.push_back(blockMesh.indices[i] + offset);
                }
                
                for (int i = 0; i < blockMesh.vertices.size(); i++)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

Terrain MakeTerrain()
{
    Terrain terrain;
    for (int i = 0; i < kTerrainSize*kTerrainSize; i++)
    {
        bool*** chunk = MakeChunk();
        ApplyNoise(chunk);
        ChunkMesh chunk_mesh = MakeChunkMesh(chunk);
        DeleteChunk(chunk);

        terrain.chunks.push_back(chunk_mesh);
    }

    return terrain;
}

void DeleteTerrain(Terrain terrain)
{
    for (int i = 0; i < terrain.chunks.size(); i++)
        DeleteChunkMesh(terrain.chunks[i]);
}

void ApplyNoise(bool*** chunk)
{
    for (int x = 0; x < kChunkSize; x++)
    {
        for (int z = 0; z < kChunkSize; z++)
        {
            int cube_x = x + kChunkSize;
            int cube_z = z + kChunkSize;
            /*
                Generate simplex noise, clamp to [0,1] and multiply by chunk size.
            */
            float noise = glm::simplex(glm::vec2(x / 16.f, z / 16.f));
            float height = ((noise + 1) / 2) * 16.f;
            for (int y = 0; y < height; y++)
                chunk[x][y][z] = true;
        }
    }
}