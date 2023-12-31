#include "chunk.h"

#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include "block.h"

const int kChunkSize = 32;
const int kChunkArea = kChunkSize*kChunkSize;
const int kChunkVolume = kChunkSize*kChunkSize*kChunkSize;

Chunk* MakeChunk(glm::vec3 pos)
{
    Chunk* chunk = new Chunk();
    chunk->is_empty = true;
    chunk->position = pos;
    chunk->blocks = new Block**[kChunkSize];
    for (int x = 0; x < kChunkSize; x++)
    {
        chunk->blocks[x] = new Block*[kChunkSize];
        for (int y = 0; y < kChunkSize; y++)
        {
            chunk->blocks[x][y] = new Block[kChunkSize];
            for (int z = 0; z < kChunkSize; z++)
                chunk->blocks[x][y][z] = MakeBlock(BlockType_Dirt);
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

    if (!chunk->is_empty)
    {
        glDeleteVertexArrays(1, &(chunk->mesh.ID));
        glDeleteBuffers(2, chunk->mesh.buffers);
    }

    delete chunk;
}

glm::vec3 ChunkCenter(Chunk* chunk)
{
    return glm::vec3(
        chunk->position.x + kChunkSize / 2,
        chunk->position.y + kChunkSize / 2,
        chunk->position.z + kChunkSize / 2
    );
}

ChunkMesh MakeChunkMesh(Chunk* chunk)
{
    ChunkMesh chunk_mesh;
    for (int x = 0; x < kChunkSize; x++)
        for (int y = 0; y < kChunkSize; y++)
            for (int z = 0; z < kChunkSize; z++)
            {
                Block block = chunk->blocks[x][y][z];
                if (block.is_active == false) continue;

                bool left_neighbor = false, right_neighbor = false;
                bool bottom_neighbor = false, top_neighbor = false;
                bool back_neighbor = false, front_neighbor = false;

                if (x > 0) left_neighbor = chunk->blocks[x - 1][y][z].is_active;
                if (x < kChunkSize - 1) right_neighbor = chunk->blocks[x + 1][y][z].is_active;

                if (y > 0) bottom_neighbor = chunk->blocks[x][y - 1][z].is_active;
                if (y < kChunkSize - 1) top_neighbor = chunk->blocks[x][y + 1][z].is_active;

                if (z > 0) back_neighbor = chunk->blocks[x][y][z - 1].is_active;
                if (z < kChunkSize - 1) front_neighbor = chunk->blocks[x][y][z + 1].is_active;

                BlockMesh blockMesh = MakeBlockMesh(
                    block.type,
                    x, y, z,
                    left_neighbor, right_neighbor,
                    bottom_neighbor, top_neighbor,
                    back_neighbor, front_neighbor
                );

                for (int i = 0; i < blockMesh.indices.size(); i++)
                {
                    unsigned offset = chunk_mesh.vertices.size() / 9;
                    chunk_mesh.indices.push_back(blockMesh.indices[i] + offset);
                }
                chunk_mesh.vertices.insert(chunk_mesh.vertices.end(), blockMesh.vertices.begin(), blockMesh.vertices.end());
            }
    return chunk_mesh;
}

void RenderChunk(Chunk* chunk)
{
    glBindVertexArray(chunk->mesh.ID);
    glDrawElements(GL_TRIANGLES, 36 * kChunkVolume, GL_UNSIGNED_INT, 0);
}

void PrepareChunkToRender(Chunk* chunk)
{
    glGenBuffers(2, chunk->mesh.buffers);
    glGenVertexArrays(1, &chunk->mesh.ID);

    glBindVertexArray(chunk->mesh.ID);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertices.size() * sizeof(float), &chunk->mesh.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.indices.size() * sizeof(unsigned), &chunk->mesh.indices[0], GL_STATIC_DRAW);
    /* Position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Texture Type
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

            float noise1 = CalculateGlobalNoise(block_x, block_z, seed, octaves, smoothness, roughness);

            octaves = 4;
            smoothness = 200.f;
            roughness = 0.45f;

            float noise2 = CalculateGlobalNoise(block_x, block_z, seed, octaves, smoothness, roughness);
            float noise3 = CalculateStoneNoise(block_x, block_z);

            float terrain_level = noise1 * noise2 * amplitude;
            float dirt_level = kChunkSize/2.f + 2.f;
            float stone_level = kChunkSize * noise1;
            float stone_noise = noise3 * noise3;
            for (int y = 0; y < kChunkSize; y++)
            {
                int block_y = y + chunk->position.y;
                if (block_y < terrain_level)
                {
                    chunk->is_empty = false;
                    chunk->blocks[x][y][z].is_active = true;

                    if (block_y+1 >= terrain_level && block_y > dirt_level) chunk->blocks[x][y][z].type = BlockType_Grass;
                    if (block_y < stone_level) chunk->blocks[x][y][z].type = BlockType_Stone;
                    if (y > stone_noise && block_y > kChunkSize) chunk->blocks[x][y][z].type = BlockType_Stone;
                }
            }
        }
    }
}

float CalculateGlobalNoise(int bx, int bz, float seed, int octaves, float smoothness, float roughness)
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

float CalculateStoneNoise(int bx, int bz)
{
    return (glm::simplex(glm::vec2(bx / 16.f, bz / 16.f)) + 1.f) / 2.f * 32.f;
}