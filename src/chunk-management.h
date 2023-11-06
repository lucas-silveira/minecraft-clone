#ifndef CHUNKM_H
#define CHUNKM_H

#include <vector>

#include <glm/glm.hpp>

#include "chunk.h"

extern std::vector<Chunk*> render_list;

void InitVisibilityList(glm::vec3 cam_pos);
void UpdateChunks(glm::vec3 cam_pos);
void UpdateLoadList(glm::vec3 cam_pos);
void UpdateSetupList();
void UpdateRenderList();
void UpdateVisibilityList(glm::vec3 pos);
void UpdateUnloadList();

#endif