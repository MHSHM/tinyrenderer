#pragma once

#include <unordered_map>
#include <string>

struct Resource
{
    struct VTable* vtable;
};

struct Resource_Manager
{
    std::unordered_map<std::string, Resource*> resources_map;
};

inline static Resource_Manager*
resource_manager_new()
{
    static Resource_Manager resource_manager;
    return &resource_manager;
}

inline static void
resource_manager_free(Resource_Manager* resource_manager)
{
    for(auto& pair: resource_manager->resources_map)
    {
        if(pair.second)
        {
             delete pair.second;
             pair.second = nullptr;
        }
    }
}

struct VTable
{
    void(*load)(struct Resource* mesh, Resource_Manager* resources_manager, const std::string& path);
    void(*free)(struct Resource* mesh, Resource_Manager* resources_manager, const std::string& path);
};