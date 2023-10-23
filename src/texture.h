#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "shader.h"

typedef struct {
	unsigned ID;
	const char* name;
	const char* path;
} Texture;

void LoadTextures(Shader &shader);
Texture* GetTextures();

#endif;