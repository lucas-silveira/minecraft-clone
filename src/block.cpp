#include "block.h"

#include <array>

#include "common.h"

const float kBlockSize = 0.5f;

Block MakeBlock(BlockType type)
{
    Block b;
    b.is_active = false;
    b.type = type;
    return b;
}

std::array<float, 48> GetTexCoord(BlockType type)
{
    std::array<float, 48> uv_coord;
    switch (type)
    {
    case BlockType_Grass:
        // Back
        uv_coord[0] = .5f;
        uv_coord[1] = .5f;
        uv_coord[2] = 1.f;
        uv_coord[3] = .5f;
        uv_coord[4] = 1.f;
        uv_coord[5] = 1.f;
        uv_coord[6] = .5f;
        uv_coord[7] = 1.f;
        // Front
        uv_coord[8] = .5f;
        uv_coord[9] = .5f;
        uv_coord[10] = 1.f;
        uv_coord[11] = .5f;
        uv_coord[12] = 1.f;
        uv_coord[13] = 1.f;
        uv_coord[14] = .5f;
        uv_coord[15] = 1.f;
        // Left
        uv_coord[16] = .5f;
        uv_coord[17] = 1.f;
        uv_coord[18] = .5f;
        uv_coord[19] = .5f;
        uv_coord[20] = 1.f;
        uv_coord[21] = .5f;
        uv_coord[22] = 1.f;
        uv_coord[23] = 1.f;
        // Right
        uv_coord[24] = 1.f;
        uv_coord[25] = .5f;
        uv_coord[26] = 1.f;
        uv_coord[27] = 1.f;
        uv_coord[28] = .5f;
        uv_coord[29] = 1.f;
        uv_coord[30] = .5f;
        uv_coord[31] = .5f;
        // Bottom
        uv_coord[32] = 0.f;
        uv_coord[33] = .5f;
        uv_coord[34] = .5f;
        uv_coord[35] = .5f;
        uv_coord[36] = .5f;
        uv_coord[37] = 1.f;
        uv_coord[38] = 0.f;
        uv_coord[39] = 1.f;
        // Top
        uv_coord[40] = 0.f;
        uv_coord[41] = 0.f;
        uv_coord[42] = .5f;
        uv_coord[43] = 0.f;
        uv_coord[44] = .5f;
        uv_coord[45] = .5f;
        uv_coord[46] = 0.f;
        uv_coord[47] = .5f;
        break;
    case BlockType_Wood:
        // Back
        uv_coord[0] = .5f;
        uv_coord[1] = 0.f;
        uv_coord[2] = 1.f;
        uv_coord[3] = 0.f;
        uv_coord[4] = 1.f;
        uv_coord[5] = 1.f;
        uv_coord[6] = .5f;
        uv_coord[7] = 1.f;
        // Front
        uv_coord[8] = .5f;
        uv_coord[9] = 0.f;
        uv_coord[10] = 1.f;
        uv_coord[11] = 0.f;
        uv_coord[12] = 1.f;
        uv_coord[13] = 1.f;
        uv_coord[14] = .5f;
        uv_coord[15] = 1.f;
        // Left
        uv_coord[16] = .5f;
        uv_coord[17] = 1.f;
        uv_coord[18] = .5f;
        uv_coord[19] = 0.f;
        uv_coord[20] = 1.f;
        uv_coord[21] = 0.f;
        uv_coord[22] = 1.f;
        uv_coord[23] = 1.f;
        // Right
        uv_coord[24] = 1.f;
        uv_coord[25] = 0.f;
        uv_coord[26] = 1.f;
        uv_coord[27] = 1.f;
        uv_coord[28] = .5f;
        uv_coord[29] = 1.f;
        uv_coord[30] = .5f;
        uv_coord[31] = 0.f;
        // Bottom
        uv_coord[32] = 0.f;
        uv_coord[33] = 0.f;
        uv_coord[34] = .5f;
        uv_coord[35] = 0.f;
        uv_coord[36] = .5f;
        uv_coord[37] = 1.f;
        uv_coord[38] = 0.f;
        uv_coord[39] = 1.f;
        // Top
        uv_coord[40] = 0.f;
        uv_coord[41] = 0.f;
        uv_coord[42] = .5f;
        uv_coord[43] = 0.f;
        uv_coord[44] = .5f;
        uv_coord[45] = 1.f;
        uv_coord[46] = 0.f;
        uv_coord[47] = 1.f;
        break;
    case BlockType_Dirt:
    case BlockType_Water:
    case BlockType_Stone:
    case BlockType_Sand:
    case BlockType_Leaf:
    default:
        // Back
        uv_coord[0] = 0.f;
        uv_coord[1] = 0.f;
        uv_coord[2] = 1.f;
        uv_coord[3] = 0.f;
        uv_coord[4] = 1.f;
        uv_coord[5] = 1.f;
        uv_coord[6] = 0.f;
        uv_coord[7] = 1.f;
        // Front
        uv_coord[8] = 0.f;
        uv_coord[9] = 0.f;
        uv_coord[10] = 1.f;
        uv_coord[11] = 0.f;
        uv_coord[12] = 1.f;
        uv_coord[13] = 1.f;
        uv_coord[14] = 0.f;
        uv_coord[15] = 1.f;
        // Left
        uv_coord[16] = 0.f;
        uv_coord[17] = 1.f;
        uv_coord[18] = 0.f;
        uv_coord[19] = 0.f;
        uv_coord[20] = 1.f;
        uv_coord[21] = 0.f;
        uv_coord[22] = 1.f;
        uv_coord[23] = 1.f;
        // Right
        uv_coord[24] = 1.f;
        uv_coord[25] = 0.f;
        uv_coord[26] = 1.f;
        uv_coord[27] = 1.f;
        uv_coord[28] = 0.f;
        uv_coord[29] = 1.f;
        uv_coord[30] = 0.f;
        uv_coord[31] = 0.f;
        // Bottom
        uv_coord[32] = 0.f;
        uv_coord[33] = 0.f;
        uv_coord[34] = 1.f;
        uv_coord[35] = 0.f;
        uv_coord[36] = 1.f;
        uv_coord[37] = 1.f;
        uv_coord[38] = 0.f;
        uv_coord[39] = 1.f;
        // Top
        uv_coord[40] = 0.f;
        uv_coord[41] = 0.f;
        uv_coord[42] = 1.f;
        uv_coord[43] = 0.f;
        uv_coord[44] = 1.f;
        uv_coord[45] = 1.f;
        uv_coord[46] = 0.f;
        uv_coord[47] = 1.f;
    }
    return uv_coord;
}

BlockMesh MakeBlockMesh(
    BlockType type,
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
)
{
    std::array<float, 48> uv_coord = GetTexCoord(type);
    std::vector<float> vertices;
    std::vector<unsigned> indices;

    if (!back_neighbor)
    {
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[0], uv_coord[1],  0.0f,  0.0f, -1.0f, (float)type,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[2], uv_coord[3],  0.0f,  0.0f, -1.0f, (float)type,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[4], uv_coord[5],  0.0f,  0.0f, -1.0f, (float)type,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[6], uv_coord[7],  0.0f,  0.0f, -1.0f, (float)type,
            });
        indices.insert(indices.end(), {
            0, 3, 2,
            2, 1, 0
            });
    }
    if (!front_neighbor)
    {
        unsigned offset = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[8],  uv_coord[9],    0.0f,  0.0f,  1.0f, (float)type,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[10], uv_coord[11],  0.0f,  0.0f,  1.0f, (float)type,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[12], uv_coord[13],  0.0f,  0.0f,  1.0f, (float)type,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[14], uv_coord[15],  0.0f,  0.0f,  1.0f, (float)type,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }

    if (!left_neighbor)
    {
        unsigned offset = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[16], uv_coord[17],  -1.0f,  0.0f,  0.0f, (float)type,
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[18], uv_coord[19],  -1.0f,  0.0f,  0.0f, (float)type,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[20], uv_coord[21],  -1.0f,  0.0f,  0.0f, (float)type,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[22], uv_coord[23],  -1.0f,  0.0f,  0.0f, (float)type,
            });
        indices.insert(indices.end(), {
            3 + offset, 0 + offset, 1 + offset,
            1 + offset, 2 + offset, 3 + offset
            });
    }
    if (!right_neighbor)
    {
        unsigned offset = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[24], uv_coord[25],  1.0f,  0.0f,  0.0f, (float)type,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[26], uv_coord[27],  1.0f,  0.0f,  0.0f, (float)type,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[28], uv_coord[29],  1.0f,  0.0f,  0.0f, (float)type,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[30], uv_coord[31],  1.0f,  0.0f,  0.0f, (float)type,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }

    if (!bottom_neighbor)
    {
        unsigned offset = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[32], uv_coord[33],  1.0f, -1.0f,  0.0f, (float)type,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  uv_coord[34], uv_coord[35],  1.0f, -1.0f,  0.0f, (float)type,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[36], uv_coord[37],  1.0f, -1.0f,  0.0f, (float)type,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  uv_coord[38], uv_coord[39],  1.0f, -1.0f,  0.0f, (float)type,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }
    if (!top_neighbor)
    {
        unsigned offset = vertices.size() / 9;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[40], uv_coord[41],  0.0f,  1.0f,  0.0f, (float)type,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  uv_coord[42], uv_coord[43],  0.0f,  1.0f,  0.0f, (float)type,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[44], uv_coord[45],  0.0f,  1.0f,  0.0f, (float)type,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  uv_coord[46], uv_coord[47],  0.0f,  1.0f,  0.0f, (float)type,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }

    BlockMesh b;
    b.vertices = vertices;
    b.indices = indices;

    return b;
}