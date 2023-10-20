#include "block.hpp"

const float BLOCK_SIZE = 0.5f;

block makeBlock(float x, float y, float z)
{
    block b = {
        false,
        {
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,

            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,

            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,

            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,

            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y-BLOCK_SIZE,  z-BLOCK_SIZE,

            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x+BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z+BLOCK_SIZE,
            x-BLOCK_SIZE,  y+BLOCK_SIZE,  z-BLOCK_SIZE
        },
        {
            0.f,  0.f,
            1.0f, 0.f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.f,  1.0f,
            0.f,  0.f,

            0.f,  0.f,
            1.0f, 0.f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.f,  1.0f,
            0.f,  0.f,

            1.0f, 0.f,
            1.0f, 1.0f,
            0.f,  1.0f,
            0.f,  1.0f,
            0.f,  0.f,
            1.0f, 0.f,

            1.0f, 0.f,
            1.0f, 1.0f,
            0.f,  1.0f,
            0.f,  1.0f,
            0.f,  0.f,
            1.0f, 0.f,

            0.f,  1.0f,
            1.0f, 1.0f,
            1.0f, 0.f,
            1.0f, 0.f,
            0.f,  0.f,
            0.f,  1.0f,

            0.f,  1.0f,
            1.0f, 1.0f,
            1.0f, 0.f,
            1.0f, 0.f,
            0.f,  0.f,
            0.f,  1.0f
        }
    };
    return b;
}