#include "chunk_manager.h"

#include "world_generator.h"
#include <cmath>
#include <ranges>

ChunkManager::ChunkManager(float size, int radius)
    : m_ChunkSize(size), m_ActiveRadius(radius)
{
    const int side = m_ActiveRadius * 2 + 1;
    m_Entities.reserve(side * side * 32);
}

void ChunkManager::Update(const glm::vec3& playerPos)
{
    const int cx = static_cast<int>(std::floor(playerPos.x / m_ChunkSize));
    const int cz = static_cast<int>(std::floor(playerPos.z / m_ChunkSize));

    if (cx == m_LastCenterX && cz == m_LastCenterZ)
        return;

    m_LastCenterX = cx;
    m_LastCenterZ = cz;

    m_Entities.clear();
    const int side = m_ActiveRadius * 2 + 1;
    m_Entities.reserve(side * side * 32);

    for (int dz = -m_ActiveRadius; dz <= m_ActiveRadius; ++dz)
    {
        for (int dx = -m_ActiveRadius; dx <= m_ActiveRadius; ++dx)
        {
            ChunkCoord coord{ cx + dx, cz + dz };
            auto it = m_Loaded.find(coord);
            if (it == m_Loaded.end())
            {
                LoadChunk(coord);
                it = m_Loaded.find(coord);
            }

            const auto& ents = it->second;
            m_Entities.insert(m_Entities.end(), ents.begin(), ents.end());
        }
    }

    UnloadFarChunks(playerPos);
}

void ChunkManager::LoadChunk(const ChunkCoord& coord)
{
    const glm::vec2 offset((float)coord.x * m_ChunkSize, (float)coord.z * m_ChunkSize);
    auto chunkEntities = WorldGenerator::GenerateChunk(offset, m_ChunkSize);
    m_Loaded.emplace(coord, std::move(chunkEntities));
}

void ChunkManager::UnloadFarChunks(const glm::vec3& playerPos)
{
    const int cx = static_cast<int>(std::floor(playerPos.x / m_ChunkSize));
    const int cz = static_cast<int>(std::floor(playerPos.z / m_ChunkSize));

    std::vector<ChunkCoord> toRemove;
    for (const auto& coord: m_Loaded | std::views::keys)
    {
        if (std::abs(coord.x - cx) > m_ActiveRadius + 1 ||
            std::abs(coord.z - cz) > m_ActiveRadius + 1)
            toRemove.push_back(coord);
    }

    for (auto& c : toRemove)
        m_Loaded.erase(c);
}
