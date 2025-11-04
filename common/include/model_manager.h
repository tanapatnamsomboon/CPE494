#pragma once

#include "model.h"
#include <string>
#include <memory>
#include <unordered_map>

class ModelManager
{
public:
    static std::shared_ptr<Model> GetModel(const std::string& path);

    static void Clear();

private:
    static std::unordered_map<std::string, std::shared_ptr<Model>> s_ModelCache;
};
