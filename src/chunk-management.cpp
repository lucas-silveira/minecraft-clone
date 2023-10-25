#include "chunk-management.h"

#include <glm/glm.hpp>

#include <vector>
#include <math.h>

#include "chunk.h"

const int kTerrainSize = 6;
const int kDistThreshold = kChunkSize*2;

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

bool isNear(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center - pos);

    return dist <= kDistThreshold;
}

bool isFar(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center - pos);

    return dist >= kDistThreshold;
}

void InitVisibilityList()
{
    left_edge = back_edge = -kTerrainSize/2;
    bottom_edge = 0, top_edge = kTerrainSize;
    right_edge = front_edge = kTerrainSize/2;

    for (int x = left_edge; x < right_edge; x++)
        for (int y = bottom_edge; y < top_edge; y++)
            for (int z = back_edge; z < front_edge; z++)
            {
                glm::vec3 pos(x*kChunkSize, y*kChunkSize, z*kChunkSize);
                Chunk* chunk = MakeChunk(pos);

                visibility_list.push_back(chunk);
            }
}

void UpdateChunks(glm::vec3 cam_pos)
{
    if (last_position != cam_pos)
    {
        UpdateRemoveList(cam_pos);
        UpdateLoadList(cam_pos);
        UpdateSetupList();
        UpdateRenderList();
        UpdateVisibilityList(cam_pos);
        UpdateUnloadList();
    }
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
    if (dist_left_edge >= kDistThreshold) return;

    left_edge--;
    right_edge--;

    for (int y = bottom_edge; y < top_edge; y++)
        for (int z = back_edge; z < front_edge; z++)
        {
            glm::vec3 pos(left_edge * kChunkSize, y * kChunkSize, z * kChunkSize);
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
}

void updateRightEdgeUnload()
{
    for (Chunk* chunk : render_list)
    {
        if (chunk->position.x > right_edge * kChunkSize) unload_list.push_back(chunk);
    }
}

void UpdateUnloadList()
{
    for (Chunk* chunk : unload_temp_list)
    {
        unload_list.push_back(chunk);
    }
    unload_temp_list.clear();

    updateRightEdgeUnload();
}