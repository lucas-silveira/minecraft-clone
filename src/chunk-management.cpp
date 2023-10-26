#include "chunk-management.h"

#include <glm/glm.hpp>

#include <vector>
#include <math.h>

#include "chunk.h"

const int kTerrainSize = 10;
const int kHThreshold = kChunkSize*4;
const int kVThreshold = kChunkSize;
const int kDThreshold = kChunkSize*4;

std::vector<Chunk*> visibility_list;
std::vector<Chunk*> visibility_temp_list;
std::vector<Chunk*> unload_list;
std::vector<Chunk*> unload_temp_list;
std::vector<Chunk*> remove_list;
std::vector<Chunk*> load_list;
std::vector<Chunk*> setup_list;
std::vector<Chunk*> render_list;

glm::vec3 last_position;
int left_edge, right_edge;
int bottom_edge, top_edge;
int back_edge, front_edge;

bool horizontal_unload_update = false;
bool vertical_unload_update = false;
bool depth_unload_update = false;

bool isNear(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center - pos);

    return dist <= kHThreshold;
}

bool isFar(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center - pos);

    return dist >= kHThreshold;
}

void InitVisibilityList()
{
    left_edge = bottom_edge = back_edge = -kTerrainSize / 2;
    right_edge = top_edge = front_edge = kTerrainSize / 2;

    for (int x = left_edge; x <= right_edge; x++)
        for (int y = bottom_edge; y <= top_edge; y++)
            for (int z = back_edge; z <= front_edge; z++)
            {
                glm::vec3 pos(x * kChunkSize, y * kChunkSize, z * kChunkSize);
                Chunk* chunk = MakeChunk(pos);

                visibility_list.push_back(chunk);
            }
}

void UpdateChunks(glm::vec3 cam_pos)
{
    if (last_position == cam_pos) return;

    UpdateRemoveList(cam_pos);
    UpdateLoadList(cam_pos);
    UpdateSetupList();
    UpdateRenderList();
    UpdateVisibilityList(cam_pos);
    UpdateUnloadList();

    last_position = cam_pos;
}

void UpdateRemoveList(glm::vec3 cam_pos)
{
    for (Chunk* chunk : unload_list)
    {
        if (!isFar(chunk, cam_pos))
        {
            unload_temp_list.push_back(chunk);
            continue;
        }
        remove_list.push_back(chunk);
    }
    unload_list.clear();
}

void UpdateLoadList(glm::vec3 cam_pos)
{
    for (Chunk* chunk : visibility_list)
    {
        if (!isNear(chunk, cam_pos))
        {
            visibility_temp_list.push_back(chunk);
            continue;
        }

        load_list.push_back(chunk);
    }
    visibility_list.clear();
}

void UpdateSetupList()
{
    for (Chunk* chunk : load_list)
    {
        ApplyNoise(chunk);
        if (chunk->is_empty)
        {
            DeleteChunk(chunk);
            continue;
        };
        chunk->mesh = MakeChunkMesh(chunk);

        setup_list.push_back(chunk);
    }
    load_list.clear();
}

void UpdateRenderList()
{
    for (Chunk* chunk : setup_list)
    {
        PrepareChunkToRender(chunk);
        render_list.push_back(chunk);
    }
    setup_list.clear();

    for (Chunk* chunk : remove_list)
    {
        auto chunk_index = std::find(render_list.begin(), render_list.end(), chunk);
        bool is_rendered = chunk_index != render_list.end();

        if (is_rendered) render_list.erase(chunk_index);
        DeleteChunk(chunk);
    }
    remove_list.clear();
}

void updateLeftEdgeVisibility(glm::vec3 pos)
{
    float dist_left_edge = glm::length(left_edge * kChunkSize - pos.x);
    if (dist_left_edge >= kHThreshold) return;

    left_edge--;
    right_edge--;
    horizontal_unload_update = true;

    for (int y = bottom_edge; y < top_edge; y++)
        for (int z = back_edge; z < front_edge; z++)
        {
            glm::vec3 pos(left_edge * kChunkSize, y * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateRightEdgeVisibility(glm::vec3 pos)
{
    float dist_right_edge = glm::length(right_edge * kChunkSize - pos.x);
    if (dist_right_edge >= kHThreshold) return;

    right_edge++;
    left_edge++;
    horizontal_unload_update = true;

    for (int y = bottom_edge; y < top_edge; y++)
        for (int z = back_edge; z < front_edge; z++)
        {
            glm::vec3 pos(right_edge * kChunkSize, y * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateBottomEdgeVisibility(glm::vec3 pos)
{
    float dist_bottom_edge = glm::length(bottom_edge * kChunkSize - pos.y);
    if (dist_bottom_edge >= kVThreshold) return;

    bottom_edge--;
    top_edge--;
    vertical_unload_update = true;

    for (int x = left_edge; x < right_edge; x++)
        for (int z = back_edge; z < front_edge; z++)
        {
            glm::vec3 pos(x * kChunkSize, bottom_edge * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateTopEdgeVisibility(glm::vec3 pos)
{
    float dist_top_edge = glm::length(top_edge * kChunkSize - pos.y);
    if (dist_top_edge >= kVThreshold) return;

    top_edge++;
    bottom_edge++;
    vertical_unload_update = true;

    for (int x = left_edge; x < right_edge; x++)
        for (int z = back_edge; z < front_edge; z++)
        {
            glm::vec3 pos(x * kChunkSize, top_edge * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateBackEdgeVisibility(glm::vec3 pos)
{
    float dist_back_edge = glm::length(back_edge * kChunkSize - pos.z);
    if (dist_back_edge >= kDThreshold) return;

    back_edge--;
    front_edge--;
    depth_unload_update = true;

    for (int x = left_edge; x < right_edge; x++)
        for (int y = bottom_edge; y < top_edge; y++)
        {
            glm::vec3 pos(x * kChunkSize, y * kChunkSize, back_edge * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateFrontEdgeVisibility(glm::vec3 pos)
{
    float dist_front_edge = glm::length(front_edge * kChunkSize - pos.z);
    if (dist_front_edge >= kDThreshold) return;

    front_edge++;
    back_edge++;
    depth_unload_update = true;

    for (int x = left_edge; x < right_edge; x++)
        for (int y = bottom_edge; y < top_edge; y++)
        {
            glm::vec3 pos(x * kChunkSize, y * kChunkSize, front_edge * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void UpdateVisibilityList(glm::vec3 pos) // remover chunks distantes e adicionar novos chunks em pontecial
{
    for (Chunk* chunk : visibility_temp_list)
    {
        visibility_list.push_back(chunk);
    }
    visibility_temp_list.clear();

    updateLeftEdgeVisibility(pos);
    updateRightEdgeVisibility(pos);
    updateBottomEdgeVisibility(pos);
    updateTopEdgeVisibility(pos);
    updateBackEdgeVisibility(pos);
    updateFrontEdgeVisibility(pos);
}

void updateRightEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.x > right_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void updateLeftEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.x < left_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void updateTopEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.y > top_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void updateBottomEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.y < bottom_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void updateFrontEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.z > front_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void updateBackEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.z < back_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void UpdateUnloadList()
{
    for (Chunk* chunk : unload_temp_list)
    {
        unload_list.push_back(chunk);
    }
    unload_temp_list.clear();

    if (horizontal_unload_update)
    {
        updateRightEdgeUnload();
        updateLeftEdgeUnload();
        horizontal_unload_update = false;
    }

    if (vertical_unload_update)
    {

        updateTopEdgeUnload();
        updateBottomEdgeUnload();
        vertical_unload_update = false;
    }

    if (depth_unload_update)
    {
        updateFrontEdgeUnload();
        updateBackEdgeUnload();
        depth_unload_update = false;
    }
}