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
        Zbuffer* zbuffer = (Zbuffer*)zbuffer;
        delete zbuffer->vtable;
        delete zbuffer;
        resource_manager_remove_resource(resource_manager, resource, path);
    }


    Zbuffer*
    zbuffer_new(int width, int height, float clear_value)
    {
        Zbuffer* zbuffer = new Zbuffer;
        zbuffer->depths.resize(width * height, clear_value);
        zbuffer->vtable = new VTable;
        zbuffer->vtable->load = &_load_zbuffer;
        zbuffer->vtable->free = &_free_zbuffer;
        zbuffer->width = width;
        zbuffer->height = height;

        return zbuffer;
    }

    void
    zbuffer_clear(Zbuffer* zbuffer, float clear_value)
    {
        memset((void*)&zbuffer->depths[0], (int)clear_value, sizeof(float) * zbuffer->width * zbuffer->height);
    }
};