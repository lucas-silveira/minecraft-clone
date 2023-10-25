#include "chunk-management.h"

#include <glm/glm.hpp>

#include <vector>
#include <math.h>

#include "chunk.h"

std::vector<Chunk*> unload_list;
std::vector<Chunk*> load_list;
std::vector<Chunk*> setup_list;
std::vector<Chunk*> render_list;
std::vector<Chunk*> visibility_list;

glm::vec3 last_position;

void InitUnloadList()
{
    for (int x = -kTerrainSize/2; x < kTerrainSize/2; x++)
        for (int y = 0; y < kTerrainSize; y++)
            for (int z = -kTerrainSize/2; z < kTerrainSize/2; z++)
            {
                Chunk* chunk = MakeChunk();
                chunk->position = glm::vec3(x * kChunkSize, y * kChunkSize, z * kChunkSize);
                
                unload_list.push_back(chunk);
            }
}

void UpdateChunks(glm::vec3 cam_pos)
{
    if (last_position != cam_pos)
    {
        UpdateLoadList(cam_pos);
        UpdateSetupList();
        UpdateRenderList();
        UpdateUnloadList(cam_pos);
    }
    last_position = cam_pos;
}

void UpdateLoadList(glm::vec3 cam_pos)
{
    for (Chunk* chunk : unload_list)
    {
        if (!IsNear(chunk, cam_pos))
        {
            visibility_list.push_back(chunk);
            continue;
        }

        load_list.push_back(chunk);
    }
    unload_list.clear();
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
}

void UpdateUnloadList(glm::vec3 pos) // remover chunks distantes e adicionar novos chunks em pontecial
{
    for (Chunk* chunk : visibility_list)
    {
        unload_list.push_back(chunk);
    }
    visibility_list.clear();

    //unsigned max_slots = 20;
    //if (unload_list.size() >= max_slots) return;

    //float slots = max_slots - unload_list.size();
    //std::vector<Chunk*> cache;

    //for (int i = 0; i < slots; i++)
    //{
    //    Chunk* near_chunk = NULL;
    //    float less_dist = INFINITY;
    //    for (Chunk* chunk : render_list)
    //    {
    //        glm::vec3 chunk_center = ChunkCenter(chunk);

    //        float dist = glm::length(chunk_center - pos);
    //        bool is_cached = std::find(cache.begin(), cache.end(), chunk) != cache.end();

    //        if (dist < less_dist && !is_cached)
    //        {
    //            near_chunk = chunk;
    //            less_dist = dist;
    //        }
    //    }

    //    if (!near_chunk) continue;

    //    Chunk* chunk = MakeChunk();
    //    chunk->position = glm::vec3(
    //        near_chunk->position.x + kChunkSize,
    //        near_chunk->position.y + kChunkSize,
    //        near_chunk->position.z + kChunkSize
    //    );
    //    cache.push_back(chunk);
    //}
    //unload_list.insert(unload_list.begin(), cache.begin(), cache.end());
}

bool IsNear(Chunk* chunk, glm::vec3 pos)
{
    glm::vec3 chunk_center = ChunkCenter(chunk);
    float dist = glm::length(chunk_center-pos);

    return dist <= kChunkSize*2;
}
