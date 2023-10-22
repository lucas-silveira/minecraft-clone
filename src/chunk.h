#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include <vector>

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
    bool*** blocks;
    glm::vec3 position;
    ChunkMesh mesh;
} Chunk;

typedef struct {
    std::vector<Chunk*> chunks;
} Terrain;

Chunk* MakeChunk(void);
void DeleteChunk(Chunk* chunk);
ChunkMesh MakeChunkMesh(Chunk* chunk);
void RenderChunk(Chunk* chunk, unsigned texture);
Terrain MakeTerrain();
void DeleteTerrain(Terrain terrain);
void ApplyNoise(Chunk* chunk);
float CalculateNoise(int bx, int bz, float seed, int octaves, float smoothness, float roughness);

#endif