#ifndef CHUNK_H
#define CHUNK_H

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
    std::vector<ChunkMesh> chunks;
} Terrain;

bool*** MakeChunk(void);
void DeleteChunk(bool*** chunk);
BlockMesh MakeBlockMesh(
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
);
ChunkMesh MakeChunkMesh(bool*** chunk);
void RenderChunk(ChunkMesh chunk, unsigned texture);
void DeleteChunkMesh(ChunkMesh chunk);
Terrain MakeTerrain();
void DeleteTerrain(Terrain terrain);
void ApplyNoise(bool*** chunk);

#endif