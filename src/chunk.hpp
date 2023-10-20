#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

extern const float kBlockSize;
extern const unsigned int kChunkSize;

typedef struct {
    float vertices[120];
    unsigned indices[36];
} BlockMesh;
typedef struct {
    unsigned ID;
    unsigned buffers[2];
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} ChunkMesh;

bool*** MakeChunk(void);
void DeleteChunk(bool*** chunk);
BlockMesh MakeBlockMesh(float x, float y, float z);
ChunkMesh MakeChunkMesh(bool*** chunk);
void RenderChunk(ChunkMesh chunk, unsigned texture);
void DeleteChunkMesh(ChunkMesh chunk);

#endif