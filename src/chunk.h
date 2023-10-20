#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

extern const float kBlockSize;
extern const unsigned int kChunkSize;

typedef struct {
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} BlockMesh;

typedef struct {
    unsigned ID;
    unsigned buffers[2];
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} ChunkMesh;

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

#endif