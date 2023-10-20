#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

extern const float BLOCK_SIZE;
extern const unsigned int CHUNK_SIZE;

typedef struct {
    float vertices[120];
    unsigned indices[36];
} blockMesh;
typedef struct {
    unsigned ID;
    unsigned buffers[2];
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} chunkMesh;

bool*** makeChunk(void);
void deleteChunk(bool*** chunk);
blockMesh makeBlockMesh(float x, float y, float z);
chunkMesh makeChunkMesh(bool*** chunk);
void renderChunk(chunkMesh chunk, unsigned texture);
void deleteChunkMesh(chunkMesh chunk);

#endif