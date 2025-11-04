#include "inventory.h"

#include "item_database.h"

Inventory::Inventory(int capacity)
    : m_Capacity(capacity)
{
    m_Slots.resize(m_Capacity);
}

bool Inventory::Add(ItemId id, int amount)
{
    if (id == ItemId::None || amount <= 0) return false;

    const auto& def = ItemDatabase::Get(id);
    int remain = amount;

    if (def.Stackable)
    {
        for (auto& s : m_Slots)
        {
            if (s.Id == id && s.Count < def.MaxStack)
            {
                int can = def.MaxStack - s.Count;
                int add = std::min(can, remain);
                s.Count += add;
                remain -= add;
                if (remain <= 0) return true;
            }
        }
    }

    for (auto& [sId, sCount] : m_Slots)
    {
        if (sId == ItemId::None)
        {
            int put = def.Stackable ? std::min(def.MaxStack, remain) : 1;
            sId = id;
            sCount = put;
            remain -= put;
            if (remain <= 0) return true;
        }
    }
    return remain <= 0;
}

bool Inventory::Remove(ItemId id, int amount)
{
    if (id == ItemId::None || amount <= 0) return false;

    int need = amount;
    for (auto& [sId, sCount] : m_Slots)
    {
        if (sId == id)
        {
            const int take = std::min(sCount, need);
            sCount -= take;
            need -= take;
            if (sCount == 0) sId = ItemId::None;
            if (need <= 0) return true;
        }
    }
    return false;
}
