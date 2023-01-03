#pragma once

#include "tgaimage.h"
#include "resource.h"

struct TGAImage;

namespace tiny
{
    struct Image : Resource
    {
        TGAImage* data;
    };

    Image*
    image_new(Resource_Manager* resource_manager, const std::string& tag, int width, int height, TGAImage::Format format);

    Image*
    image_new(Resource_Manager* resource_manager, const std::string& tag);
};