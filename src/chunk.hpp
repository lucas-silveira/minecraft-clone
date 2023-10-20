#ifndef CHUNK_H
#define CHUNK_H

#include "block.hpp"

extern const unsigned int CHUNK_SIZE;

block*** makeChunk(void);
void deleteChunk(block*** chunk);

#endif