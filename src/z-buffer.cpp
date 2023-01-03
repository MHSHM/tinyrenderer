#include "z-buffer.h"

namespace tiny
{
    inline static void
    _load_zbuffer(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
    }

    inline static void
    _free_zbuffer(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Zbuffer* zbuffer = (Zbuffer*)resource;
        delete zbuffer;
        resource_manager_remove_resource(resource_manager, resource, path);
    }


    Zbuffer*
    zbuffer_new(Resource_Manager* resource_manager, const std::string& tag, int width, int height, float clear_value)
    {
        Zbuffer* zbuffer = new Zbuffer;
        zbuffer->depths.resize(width * height, clear_value);
        zbuffer->load = &_load_zbuffer;
        zbuffer->free = &_free_zbuffer;
        zbuffer->width = width;
        zbuffer->height = height;
        resource_manager_add_resource(resource_manager, zbuffer, tag);
        return zbuffer;
    }

    void
    zbuffer_clear(Zbuffer* zbuffer, float clear_value)
    {
        memset((void*)&zbuffer->depths[0], (int)clear_value, sizeof(float) * zbuffer->width * zbuffer->height);
    }
};