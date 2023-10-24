#include "chunk-management.h"

#include <vector>

#include "chunk.h"

std::vector<Chunk*> load_list;
std::vector<Chunk*> setup_list;
std::vector<Chunk*> visibility_list;
std::vector<Chunk*> render_list;

void UpdateLoadList()
{
    for (int x = -kTerrainSize/2; x < kTerrainSize/2; x++)
        for (int y = -kTerrainSize/2; y < kTerrainSize/2; y++)
            for (int z = -kTerrainSize/2; z < kTerrainSize/2; z++)
            {
                Chunk* chunk = MakeChunk();
                chunk->position = glm::vec3(x * kChunkSize, y * kChunkSize, z * kChunkSize);
                
                load_list.push_back(chunk);
            }
}

void UpdateSetupList()
{
    for (Chunk* chunk : load_list)
    {
        ApplyNoise(chunk);
        if (chunk->is_empty) continue;
        chunk->mesh = MakeChunkMesh(chunk);

        setup_list.push_back(chunk);
    }
    load_list.clear();
}

void UpdateVisibilityList()
{
    for (Chunk* chunk : setup_list)
    {
        PrepareChunkToRender(chunk);
        visibility_list.push_back(chunk);
    }
    setup_list.clear();
}

void UpdateRenderList()
{
    for (Chunk* chunk : visibility_list)
    {
        render_list.push_back(chunk);
    }
    visibility_list.clear();
}