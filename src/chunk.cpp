#include "chunk.hpp"

const unsigned int CHUNK_SIZE = 16;

block*** makeChunk(void)
{
    block*** blocks = new block * *[CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        blocks[x] = new block * [CHUNK_SIZE];
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            blocks[x][y] = new block[CHUNK_SIZE];
            for (int z = 0; z < CHUNK_SIZE; z++)
                blocks[x][y][z] = makeBlock(x, y, z);
        }
    }
    return blocks;
}

void deleteChunk(block*** chunk)
{
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            delete[] chunk[i][j];
        }
        delete[] chunk[i];
    }
    delete[] chunk;
}