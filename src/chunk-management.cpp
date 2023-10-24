#include "chunk-management.h"

#include <vector>

#include "chunk.h"

std::vector<Chunk*> unload_list;
std::vector<Chunk*> load_list;
std::vector<Chunk*> setup_list;
std::vector<Chunk*> render_list;

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

void UpdateLoadList() // adicionar somente os chunks próximos
{
    for (Chunk* chunk : unload_list)
    {
        ApplyNoise(chunk);
        if (chunk->is_empty) continue;
        chunk->mesh = MakeChunkMesh(chunk);

        load_list.push_back(chunk);
    }
    unload_list.clear();
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

void UpdateRenderList()
{
    for (Chunk* chunk : setup_list)
    {
        PrepareChunkToRender(chunk);
        render_list.push_back(chunk);
    }
    setup_list.clear();
}

void UpdateUnloadList() // remover chunks distantes e adicionar novos chunks em pontecial
{

}