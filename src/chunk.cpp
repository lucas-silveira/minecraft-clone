#include "chunk.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include <vector>

const float kBlockSize = 0.5f;
const unsigned kChunkSize = 32;
const unsigned kChunkArea = kChunkSize*kChunkSize;
const unsigned kChunkVolume = kChunkSize*kChunkSize*kChunkSize;
const unsigned kTerrainSize = 6;

Chunk* MakeChunk(void)
{
    Chunk* chunk = new Chunk();
    chunk->blocks = new bool**[kChunkSize];
    for (int x = 0; x < kChunkSize; x++)
    {
        chunk->blocks[x] = new bool*[kChunkSize];
        for (int y = 0; y < kChunkSize; y++)
        {
            chunk->blocks[x][y] = new bool[kChunkSize];
            for (int z = 0; z < kChunkSize; z++)
                chunk->blocks[x][y][z] = false;
        }
    }
    return chunk;
}

void DeleteChunk(Chunk* chunk)
{
    for (int i = 0; i < kChunkSize; ++i) {
        for (int j = 0; j < kChunkSize; ++j) {
            delete[] chunk->blocks[i][j];
        }
        delete[] chunk->blocks[i];
    }
    delete[] chunk->blocks;

    glDeleteVertexArrays(1, &(chunk->mesh.ID));
    glDeleteBuffers(2, chunk->mesh.buffers);

    delete chunk;
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

ChunkMesh MakeChunkMesh(Chunk* chunk)
{
    ChunkMesh chunk_mesh;
    for (int x = 0; x < kChunkSize; x++)
        for (int y = 0; y < kChunkSize; y++)
            for (int z = 0; z < kChunkSize; z++)
            {
                if (chunk->blocks[x][y][z] == false) continue;

                bool left_neighbor = false, right_neighbor = false;
                bool bottom_neighbor = false, top_neighbor = false;
                bool back_neighbor = false, front_neighbor = false;

                if (x > 0) left_neighbor = chunk->blocks[x - 1][y][z];
                if (x < kChunkSize - 1) right_neighbor = chunk->blocks[x + 1][y][z];

                if (y > 0) bottom_neighbor = chunk->blocks[x][y - 1][z];
                if (y < kChunkSize - 1) top_neighbor = chunk->blocks[x][y + 1][z];

                if (z > 0) back_neighbor = chunk->blocks[x][y][z - 1];
                if (z < kChunkSize - 1) front_neighbor = chunk->blocks[x][y][z + 1];

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

    if (chunk_mesh.vertices.size() > 0)
    {
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
    }

    return chunk_mesh;
}

void RenderChunk(Chunk* chunk, unsigned texture)
{
    if (chunk->mesh.vertices.size() == 0) return;

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(chunk->mesh.ID);

    glDrawElements(GL_TRIANGLES, 36 * kChunkSize * kChunkSize * kChunkSize, GL_UNSIGNED_INT, 0);
}

Terrain MakeTerrain()
{
    Terrain terrain;
    for (int x = 0; x < kTerrainSize; x++)
        for (int y = 0; y < 2; y++)
            for (int z = 0; z < kTerrainSize; z++)
            {
                Chunk* chunk = MakeChunk();
                chunk->position = glm::vec3(x * kChunkSize, y * kChunkSize, z * kChunkSize);
                ApplyNoise(chunk);
                chunk->mesh = MakeChunkMesh(chunk);

                terrain.chunks.push_back(chunk);
            }

    return terrain;
}

void DeleteTerrain(Terrain terrain)
{
    for (int i = 0; i < terrain.chunks.size(); i++)
        DeleteChunk(terrain.chunks[i]);
}

void ApplyNoise(Chunk* chunk)
{
    for (int x = 0; x < kChunkSize; x++)
    {
        for (int z = 0; z < kChunkSize; z++)
        {
            int block_x = x + chunk->position.x;
            int block_z = z + chunk->position.z;

            int octaves = 6;
            float seed = 2048.f;
            float smoothness = 205.f;
            float roughness = 0.58f;
            float amplitude = 128.f;

            float noise1 = calc_noise(block_x, block_z, seed, octaves, smoothness, roughness);

            octaves = 4;
            smoothness = 200.f;
            roughness = 0.45f;

            float noise2 = calc_noise(block_x, block_z, seed, octaves, smoothness, roughness);

            float height = noise1 * noise2 * amplitude;
            for (int y = 0; y < kChunkSize; y++)
            {
                int block_y = y + chunk->position.y;
                if (block_y < height) chunk->blocks[x][y][z] = true;
            }
        }
    }
}

float calc_noise(int bx, int bz, float seed, int octaves, float smoothness, float roughness)
{
    float acc_noise = 0;
    float acc_amplitude = 0;

    for (int i = 0; i < octaves; i++)
    {
        float frequency = glm::pow(2.f, i);
        float amplitude = glm::pow(roughness, i);

        float x = bx * frequency / smoothness;
        float y = bz * frequency / smoothness;

        float noise = glm::simplex(glm::vec3(x + seed, y + seed, seed));
        noise = (noise + 1.f) / 2.f;
        acc_noise += noise * amplitude;
        acc_amplitude += amplitude;
    }

    return acc_noise / acc_amplitude;
}