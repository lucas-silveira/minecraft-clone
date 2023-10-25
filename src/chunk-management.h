#ifndef CHUNKM_H
#define CHUNKM_H

#include <glm/glm.hpp>

#include <vector>

#include "chunk.h"

extern std::vector<Chunk*> render_list;

void InitUnloadList();
void UpdateChunks(glm::vec3 cam_pos);
void UpdateLoadList(glm::vec3 cam_pos);
void UpdateSetupList();
void UpdateRenderList();
void UpdateUnloadList(glm::vec3 pos);
bool IsNear(Chunk* chunk, glm::vec3 pos);

#endif