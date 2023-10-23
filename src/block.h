#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "common.h"

extern const float kBlockSize;

typedef struct
{
    bool is_active;
    BlockType type;
} Block;

typedef struct
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;
} BlockMesh;

Block MakeBlock(BlockType type);
BlockMesh MakeBlockMesh(
    BlockType type,
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
);

#endif