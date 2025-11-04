#include "model_manager.h"

#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Model>> ModelManager::s_ModelCache;

std::shared_ptr<Model> ModelManager::GetModel(const std::string& path)
{
    const auto it = s_ModelCache.find(path);
    if (it != s_ModelCache.end())
    {
        return it->second;
    }

    try
    {
        auto model = std::make_shared<Model>(path);
        s_ModelCache[path] = model;
        std::cout << "[ModelManager] Loaded model: " << path << std::endl;
        return model;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ModelManager] Failed to load model: " << path
        << " (" << e.what() << ")" << std::endl;
        return nullptr;
    }
}

void ModelManager::Clear()
{
    s_ModelCache.clear();
    std::cout << "[ModelManager] Cache cleared.\n";
}
