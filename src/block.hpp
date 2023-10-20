#ifndef BLOCK_H
#define BLOCK_H

extern const float BLOCK_SIZE;
typedef struct {
    bool active;
    float vertices[3*6*6];
    float texCoords[2*6*6];
} block;

block makeBlock(float x, float y, float z);

#endif