#include "tgaimage.h"
#include "line.h"

static const TGAColor white   = TGAColor(255, 255, 255, 255);
static const TGAColor red     = TGAColor(255, 0,   0,   255);
static const TGAColor green   = TGAColor(0, 255,   0,   255);
static const TGAColor blue    = TGAColor(0 , 0,   255,   255);

int main()
{
    TGAImage image(100, 100, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    tiny::Line line1 = tiny::line_new(13, 20, 80, 40, red);
    tiny::Line line2 = tiny::line_new(20, 13, 40, 80, green);
    tiny::line_draw(line1, image);
    tiny::line_draw(line2, image);
    image.write_tga_file("output.tga");
    return 0;
}