#ifndef CHUNKM_H
#define CHUNKM_H

#include <vector>

#include "chunk.h"

extern std::vector<Chunk*> render_list;

void UpdateLoadList();
void UpdateSetupList();
void UpdateVisibilityList();
void UpdateRenderList();

#endif