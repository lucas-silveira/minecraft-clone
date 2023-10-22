#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

extern const float kBlockSize;

typedef struct
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} BlockMesh;

BlockMesh MakeBlockMesh(
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
);

#endif