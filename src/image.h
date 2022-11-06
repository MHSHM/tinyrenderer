#pragma once

#include "tgaimage.h"


static const TGAColor white   = TGAColor(255, 255, 255, 255);
static const TGAColor red     = TGAColor(255, 0,   0,   255);
static const TGAColor green   = TGAColor(0, 255,   0,   255);
static const TGAColor blue    = TGAColor(0 , 0,   255,   255);

inline static TGAImage*
image_new(int width, int height, TGAImage::Format format)
{
    static TGAImage image(width, height, format);
    
    return &image;
}

inline static TGAImage*
image_load(const char* path)
{
    static TGAImage image;
    image.read_tga_file(path);
    return &image;
}