#pragma once

#include "item_id.h"
#include <string>
#include <unordered_map>

struct ItemDef
{
    ItemId Id{ ItemId::None };
    const char* Name{ "" };
    const char* IconPath{ "" };
    bool Stackable{ true };
    int MaxStack{ 20 };
    std::vector<std::string> Variants;
};

class ItemDatabase
{
public:
    static void Init();
    static const ItemDef& Get(ItemId id);

private:
    static std::unordered_map<ItemId, ItemDef> s_Map;
    static ItemDef s_Null;
};