#include "chunk.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include <vector>

#include "block.h"

const unsigned kChunkSize = 32;
const unsigned kChunkArea = kChunkSize*kChunkSize;
const unsigned kChunkVolume = kChunkSize*kChunkSize*kChunkSize;
const unsigned kTerrainSize = 6;
const unsigned kTerrainHeight = 2;

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
        for (int y = 0; y < kTerrainHeight; y++)
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

void DeleteTerrain(Terrain &terrain)
{
    for (int i = 0; i < terrain.chunks.size(); i++)
        DeleteChunk(terrain.chunks[i]);
}

void PrepareToRender(Terrain &terrain)
{
    for (Chunk* chunk : terrain.chunks)
    {
        if (chunk->mesh.vertices.size() > 0)
        {
            glGenBuffers(2, chunk->mesh.buffers);
            glGenVertexArrays(1, &chunk->mesh.ID);

            glBindVertexArray(chunk->mesh.ID);

            glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.buffers[0]);
            glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertices.size() * sizeof(float), &chunk->mesh.vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.buffers[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.indices.size() * sizeof(unsigned), &chunk->mesh.indices[0], GL_STATIC_DRAW);
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
    }
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

            float noise1 = CalculateNoise(block_x, block_z, seed, octaves, smoothness, roughness);

            octaves = 4;
            smoothness = 200.f;
            roughness = 0.45f;

            float noise2 = CalculateNoise(block_x, block_z, seed, octaves, smoothness, roughness);

            float height = noise1 * noise2 * amplitude;
            for (int y = 0; y < kChunkSize; y++)
            {
                int block_y = y + chunk->position.y;
                if (block_y < height) chunk->blocks[x][y][z] = true;
            }
        }
    }
}

float CalculateNoise(int bx, int bz, float seed, int octaves, float smoothness, float roughness)
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