#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include <glm/glm.hpp>

#include "block.h"

extern const int kChunkSize;
extern const int kChunkArea;
extern const int kChunkVolume;
extern const int kTerrainSize;

typedef struct
{
    unsigned ID;
    unsigned buffers[2];
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} ChunkMesh;

typedef struct {
    bool is_empty;
    Block*** blocks;
    glm::vec3 position;
    ChunkMesh mesh;
} Chunk;

typedef struct {
    std::vector<Chunk*> chunks;
} Terrain;

Chunk* MakeChunk(glm::vec3 pos);
void DeleteChunk(Chunk* chunk);
glm::vec3 ChunkCenter(Chunk* chunk);
ChunkMesh MakeChunkMesh(Chunk* chunk);
void RenderChunk(Chunk* chunk);
void PrepareChunkToRender(Chunk* chunk);
void ApplyNoise(Chunk* chunk);
float CalculateGlobalNoise(int bx, int bz, float seed, int octaves, float smoothness, float roughness);
float CalculateStoneNoise(int bx, int bz);

#endif