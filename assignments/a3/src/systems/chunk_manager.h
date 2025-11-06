#pragma once

#include <entity.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

struct ChunkCoord
{
    int x, z;
    bool operator==(const ChunkCoord& o) const noexcept
    {
        return x == o.x && z == o.z;
    }
};

struct ChunkCoordHash
{
    size_t operator()(const ChunkCoord& c) const noexcept
    {
        const uint64_t ux = (uint32_t)c.x;
        const uint64_t uz = (uint32_t)c.z;
        uint64_t h = ux * 0x9E3779B185EBCA87ull;
        h ^= uz + 0x9E3779B185EBCA87ull + (h << 6) + (h >> 2);
        return h;
    }
};

class ChunkManager
{
public:
    ChunkManager(float size, int radius);

    void Update(const glm::vec3& playerPos);

    [[nodiscard]] const std::vector<std::shared_ptr<Entity>>& GetVisibleEntities() const { return m_Entities; }

    [[nodiscard]] float GetChunkSize() const noexcept { return m_ChunkSize; }
    [[nodiscard]] int   GetActiveRadius() const noexcept { return m_ActiveRadius; }

private:
    void LoadChunk(const ChunkCoord& coord);
    void UnloadFarChunks(const glm::vec3& playerPos);

private:
    float m_ChunkSize;
    int   m_ActiveRadius;

    int m_LastCenterX{ INT32_MIN };
    int m_LastCenterZ{ INT32_MIN };

    std::unordered_map<ChunkCoord, std::vector<std::shared_ptr<Entity>>, ChunkCoordHash> m_Loaded;
    std::vector<std::shared_ptr<Entity>> m_Entities;
};