#include "image.h"

namespace tiny 
{
    inline static void
    _image_load(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Image* image = (Image*)resource;
        image->data->read_tga_file(path);
        resource_manager_add_resource(resource_manager, resource, path);
    }

    inline static void
    _image_free(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Image* image = (Image*)resource;
        delete image->vtable;
        delete image->data;
        
        image->vtable = nullptr;
        image->data   = nullptr;

        delete image;

        resource_manager_remove_resource(resource_manager, resource, path);
    }

    Image*
    image_new(int width, int height, TGAImage::Format format)
    {
        Image* image  = new Image;
        image->vtable = new VTable;
        image->vtable->load = &_image_load;
        image->vtable->free = &_image_free;
        image->data   = new TGAImage(width, height, format);
        return image;
    }

    Image*
    image_new()
    {
        Image* image  = new Image;
        image->vtable = new VTable;
        image->vtable->load = &_image_load;
        image->vtable->free = &_image_free;
        image->data   = new TGAImage;
        return image;
    }
};