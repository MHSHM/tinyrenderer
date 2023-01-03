#include "image.h"

namespace tiny
{
    inline static void
    _image_load(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Image* image = (Image*)resource;
        image->data->read_tga_file(path);
    }

    inline static void
    _image_free(Resource* resource, Resource_Manager* resource_manager, const std::string& path)
    {
        Image* image = (Image*)resource;
        delete image->data;
        image->data   = nullptr;
        delete image;
        resource_manager_remove_resource(resource_manager, resource, path);
    }

    Image*
    image_new(Resource_Manager* resource_manager, const std::string& tag, int width, int height, TGAImage::Format format)
    {
        Image* image  = new Image;
        image->load = &_image_load;
        image->free = &_image_free;
        image->data   = new TGAImage(width, height, format);
        resource_manager_add_resource(resource_manager, image, tag);
        return image;
    }

    Image*
    image_new(Resource_Manager* resource_manager, const std::string& tag)
    {
        Image* image  = new Image;
        image->load = &_image_load;
        image->free = &_image_free;
        image->data   = new TGAImage;
        resource_manager_add_resource(resource_manager, image, tag);
        return image;
    }
};