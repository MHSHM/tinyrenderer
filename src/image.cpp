#include "image.h"


namespace tiny 
{
    TGAImage*
    image_new(int width, int height, TGAImage::Format format)
    {
        static TGAImage image(width, height, format);
    
        return &image;
    }

    TGAImage*
    image_load(const char* path)
    {
        static TGAImage image;
        image.read_tga_file(path);
        return &image;
    }
};