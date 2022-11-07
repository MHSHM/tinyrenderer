#pragma once

#include "tgaimage.h"

namespace tiny 
{
    TGAImage*
    image_new(int width, int height, TGAImage::Format format);

    TGAImage*
    image_load(const char* path);
};