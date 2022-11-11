#pragma once

#include "tgaimage.h"
#include "resource.h"

struct TGAImage;

struct Image : Resource 
{
    TGAImage* data;
};

namespace tiny
{
    Image*
    image_new(int width, int height, TGAImage::Format format);

    Image*
    image_new();
};