#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

extern const float kBlockSize;

enum BlockType
{
    BlockType_Dirt = 0,
    BlockType_Grass,
    BlockType_Water,
    BlockType_Stone,
    BlockType_Wood,
    BlockType_Sand
};

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
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
);

#endif