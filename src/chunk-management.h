#ifndef CHUNKM_H
#define CHUNKM_H

#include <vector>

#include "chunk.h"

extern std::vector<Chunk*> render_list;

void InitUnloadList();
void UpdateLoadList();
void UpdateSetupList();
void UpdateRenderList();
void UpdateUnloadList();

#endif