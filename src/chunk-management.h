#ifndef CHUNKM_H
#define CHUNKM_H

#include <glm/glm.hpp>

#include <vector>

#include "chunk.h"

extern std::vector<Chunk*> render_list;

void InitVisibilityList();
void UpdateChunks(glm::vec3 cam_pos);
void UpdateRemoveList(glm::vec3 cam_pos);
void UpdateLoadList(glm::vec3 cam_pos);
void UpdateSetupList();
void UpdateRenderList();
void UpdateVisibilityList(glm::vec3 pos);
void UpdateUnloadList();

#endif