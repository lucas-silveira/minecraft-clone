#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include <vector>

extern const float kBlockSize;
extern const unsigned kChunkSize;
extern const unsigned kTerrainSize;

typedef struct
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} BlockMesh;

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
BlockMesh MakeBlockMesh(
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
);
ChunkMesh MakeChunkMesh(Chunk* chunk);
void RenderChunk(Chunk* chunk, unsigned texture);
Terrain MakeTerrain();
void DeleteTerrain(Terrain terrain);
void ApplyNoise(Chunk* chunk);

#endif