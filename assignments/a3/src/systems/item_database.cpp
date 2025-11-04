#include "item_database.h"

std::unordered_map<ItemId, ItemDef> ItemDatabase::s_Map{};
ItemDef ItemDatabase::s_Null{ ItemId::None, "Unknown", "", true, 1 };

void ItemDatabase::Init()
{
    s_Map.clear();
    s_Map[ItemId::Apple] = {
        ItemId::Apple, "Apple", "assets/icons/apple.png", true, 10,
        { "apple_0", "apple_1" }
    };
    s_Map[ItemId::Bottle] = {
        ItemId::Bottle,  "Bottle",  "assets/icons/bottle.png", true, 5,
        { "bottle_0", "bottle_1", "bottle_2", "bottle_3", "bottle_4" }
    };
    s_Map[ItemId::Bandage] = {
        ItemId::Bandage, "Bandage", "assets/icons/bandage.png", true, 5,
        {}
    };
    s_Map[ItemId::Branch] = {
        ItemId::Branch,  "Branch",  "assets/icons/branch.png", true, 20,
        { "branch_0", "branch_1" }
    };
}

const ItemDef& ItemDatabase::Get(ItemId id)
{
    const auto it = s_Map.find(id);
    return it != s_Map.end() ? it->second : s_Null;
}
