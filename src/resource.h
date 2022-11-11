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

struct VTable
{
    void(*load)(struct Resource* resource, Resource_Manager* resources_manager, const std::string& path);
    void(*free)(struct Resource* resource, Resource_Manager* resources_manager, const std::string& path);
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
    for(auto& [_, resource]: resource_manager->resources_map)
    {
        if(resource)
        {
             delete resource->vtable;
             delete resource;
             resource->vtable = nullptr;
             resource = nullptr;
        }
    }
}

inline static void
resource_manager_add_resource(Resource_Manager* resource_manager, Resource* resource, const std::string& tag)
{
    resource_manager->resources_map[tag] = resource;
}

inline static void
resource_manager_remove_resource(Resource_Manager* resource_manager, Resource* resource, const std::string& tag)
{
    resource_manager->resources_map.erase(tag);
}