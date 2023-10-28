#include "chunk-management.h"

#include <glm/glm.hpp>

#include <math.h>

#include <vector>
#include <future>
#include <thread>
#include <chrono>

#include "chunk.h"

using namespace std::chrono_literals;

const int kTerrainSize = 10;
const int kThreshold = kChunkSize*4;

std::vector<Chunk*> visibility_list;
std::vector<Chunk*> visibility_temp_list;
std::vector<Chunk*> unload_list;
std::vector<Chunk*> load_list;
std::vector<Chunk*> setup_list;
std::vector<Chunk*> render_list;

int left_edge, right_edge;
int bottom_edge, top_edge;
int back_edge, front_edge;

bool horizontal_unload_update = false;
bool vertical_unload_update = false;
bool depth_unload_update = false;

std::future<void> cm_thread;
bool exec_thread = true;

bool isNear(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center - pos);

    return dist <= kThreshold;
}

void InitVisibilityList(glm::vec3 cam_pos)
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
    UpdateLoadList(cam_pos);
    UpdateSetupList();
    UpdateRenderList();
}

void UpdateChunks(glm::vec3 cam_pos)
{
    
    if (exec_thread)
    {
        cm_thread = std::async(std::launch::async, [&] {
            UpdateLoadList(cam_pos);
            UpdateSetupList();
        });
        exec_thread = false;
    }

    auto thread_status = cm_thread.wait_for(0ms);
    if (thread_status != std::future_status::ready) return;

    UpdateRenderList();
    UpdateVisibilityList(cam_pos);
    UpdateUnloadList();

    exec_thread = true;
}

void UpdateLoadList(glm::vec3 cam_pos)
{
    for (Chunk* chunk : visibility_list)
    {
        if (isNear(chunk, cam_pos))
        {
            load_list.push_back(chunk);
            continue;
        }
        visibility_temp_list.push_back(chunk);
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

    for (Chunk* chunk : unload_list)
    {
        auto chunk_index = std::find(render_list.begin(), render_list.end(), chunk);
        bool is_rendered = chunk_index != render_list.end();

        if (is_rendered) render_list.erase(chunk_index);
        DeleteChunk(chunk);
    }
    unload_list.clear();
}

void updateLeftEdgeVisibility(glm::vec3 pos)
{
    float dist_left_edge = glm::length(left_edge * kChunkSize - pos.x);
    if (dist_left_edge >= kThreshold) return;

    left_edge--;
    right_edge--;
    horizontal_unload_update = true;

    for (int y = bottom_edge; y <= top_edge; y++)
        for (int z = back_edge; z <= front_edge; z++)
        {
            glm::vec3 pos(left_edge * kChunkSize, y * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateRightEdgeVisibility(glm::vec3 pos)
{
    float dist_right_edge = glm::length(right_edge * kChunkSize - pos.x);
    if (dist_right_edge >= kThreshold) return;

    right_edge++;
    left_edge++;
    horizontal_unload_update = true;

    for (int y = bottom_edge; y <= top_edge; y++)
        for (int z = back_edge; z <= front_edge; z++)
        {
            glm::vec3 pos(right_edge * kChunkSize, y * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateBottomEdgeVisibility(glm::vec3 pos)
{
    float dist_bottom_edge = glm::length(bottom_edge * kChunkSize - pos.y);
    if (dist_bottom_edge >= kThreshold) return;

    bottom_edge--;
    top_edge--;
    vertical_unload_update = true;

    for (int x = left_edge; x <= right_edge; x++)
        for (int z = back_edge; z <= front_edge; z++)
        {
            glm::vec3 pos(x * kChunkSize, bottom_edge * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateTopEdgeVisibility(glm::vec3 pos)
{
    float dist_top_edge = glm::length(top_edge * kChunkSize - pos.y);
    if (dist_top_edge >= kThreshold) return;

    top_edge++;
    bottom_edge++;
    vertical_unload_update = true;

    for (int x = left_edge; x <= right_edge; x++)
        for (int z = back_edge; z <= front_edge; z++)
        {
            glm::vec3 pos(x * kChunkSize, top_edge * kChunkSize, z * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateBackEdgeVisibility(glm::vec3 pos)
{
    float dist_back_edge = glm::length(back_edge * kChunkSize - pos.z);
    if (dist_back_edge >= kThreshold) return;

    back_edge--;
    front_edge--;
    depth_unload_update = true;

    for (int x = left_edge; x <= right_edge; x++)
        for (int y = bottom_edge; y <= top_edge; y++)
        {
            glm::vec3 pos(x * kChunkSize, y * kChunkSize, back_edge * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void updateFrontEdgeVisibility(glm::vec3 pos)
{
    float dist_front_edge = glm::length(front_edge * kChunkSize - pos.z);
    if (dist_front_edge >= kThreshold) return;

    front_edge++;
    back_edge++;
    depth_unload_update = true;

    for (int x = left_edge; x <= right_edge; x++)
        for (int y = bottom_edge; y <= top_edge; y++)
        {
            glm::vec3 pos(x * kChunkSize, y * kChunkSize, front_edge * kChunkSize);
            Chunk* c = MakeChunk(pos);
            visibility_list.push_back(c);
        }
}

void UpdateVisibilityList(glm::vec3 pos)
{
    for (Chunk* chunk : visibility_temp_list)
    {
        visibility_list.push_back(chunk);
    }
    visibility_temp_list.clear();

    updateLeftEdgeVisibility(pos);
    updateRightEdgeVisibility(pos);
    //updateBottomEdgeVisibility(pos);
    //updateTopEdgeVisibility(pos);
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
    if (horizontal_unload_update)
    {
        updateRightEdgeUnload();
        updateLeftEdgeUnload();
        horizontal_unload_update = false;
    }

    //if (vertical_unload_update)
    //{

    //    updateTopEdgeUnload();
    //    updateBottomEdgeUnload();
    //    vertical_unload_update = false;
    //}

    if (depth_unload_update)
    {
        updateFrontEdgeUnload();
        updateBackEdgeUnload();
        depth_unload_update = false;
    }
}