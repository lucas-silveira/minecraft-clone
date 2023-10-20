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
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} chunkMesh;

bool*** makeChunk(void);
void deleteChunk(bool*** chunk);
blockMesh makeBlockMesh(float x, float y, float z);
chunkMesh makeChunkMesh(bool*** chunk);

#endif