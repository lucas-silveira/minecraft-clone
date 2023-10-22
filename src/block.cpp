#include "block.h"

const float kBlockSize = 0.5f;

BlockMesh MakeBlockMesh(
    float x, float y, float z,
    bool left_neighbor, bool right_neighbor,
    bool bottom_neighbor, bool top_neighbor,
    bool back_neighbor, bool front_neighbor
)
{
    std::vector<float> vertices;
    std::vector<unsigned> indices;

    if (!back_neighbor)
    {
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            });
        indices.insert(indices.end(), {
            0, 3, 2,
            2, 1, 0
            });
    }
    if (!front_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }

    if (!left_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
            });
        indices.insert(indices.end(), {
            3 + offset, 0 + offset, 1 + offset,
            1 + offset, 2 + offset, 3 + offset
            });
    }
    if (!right_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }

    if (!bottom_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x - kBlockSize, y - kBlockSize, z - kBlockSize,  0.0f, 0.0f,  1.0f, -1.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z - kBlockSize,  1.0f, 0.0f,  1.0f, -1.0f,  0.0f,
            x + kBlockSize, y - kBlockSize, z + kBlockSize,  1.0f, 1.0f,  1.0f, -1.0f,  0.0f,
            x - kBlockSize, y - kBlockSize, z + kBlockSize,  0.0f, 1.0f,  1.0f, -1.0f,  0.0f,
            });
        indices.insert(indices.end(), {
            0 + offset, 1 + offset, 2 + offset,
            2 + offset, 3 + offset, 0 + offset
            });
    }
    if (!top_neighbor)
    {
        unsigned offset = vertices.size() / 8;
        vertices.insert(vertices.end(), {
            x + kBlockSize, y + kBlockSize, z - kBlockSize,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z - kBlockSize,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
            x - kBlockSize, y + kBlockSize, z + kBlockSize,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
            x + kBlockSize, y + kBlockSize, z + kBlockSize,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
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