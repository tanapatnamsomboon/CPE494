#pragma once

#include <vector>
#include "item_id.h"

struct ItemStack
{
    ItemId Id{ ItemId::None };
    int Count{ 0 };
};

class Inventory
{
public:
    explicit Inventory(int capacity = 16);
    bool Add(ItemId id, int amount);
    bool Remove(ItemId id, int amount);
    [[nodiscard]] const std::vector<ItemStack>& GetSlots() const { return m_Slots; }

private:
    int m_Capacity;
    std::vector<ItemStack> m_Slots;
};