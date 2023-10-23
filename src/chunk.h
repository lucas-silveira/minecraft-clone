#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include <vector>

#include "block.h"

extern const unsigned kChunkSize;
extern const unsigned kChunkArea;
extern const unsigned kChunkVolume;
extern const unsigned kTerrainSize;
extern const unsigned kTerrainHeight;

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

Chunk* MakeChunk(void);
void DeleteChunk(Chunk* chunk);
ChunkMesh MakeChunkMesh(Chunk* chunk);
void RenderChunk(Chunk* chunk);
Terrain MakeTerrain();
void DeleteTerrain(Terrain &terrain);
void PrepareToRender(Terrain &terrain);
void ApplyNoise(Chunk* chunk);
float CalculateGlobalNoise(int bx, int bz, float seed, int octaves, float smoothness, float roughness);
float CalculateStoneNoise(int bx, int bz);

#endif