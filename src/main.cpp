#include "tgaimage.h"
#include "line.h"
#include "objloader.h"

static const TGAColor white   = TGAColor(255, 255, 255, 255);
static const TGAColor red     = TGAColor(255, 0,   0,   255);
static const TGAColor green   = TGAColor(0, 255,   0,   255);
static const TGAColor blue    = TGAColor(0 , 0,   255,   255);

static const int width  = 800;
static const int height = 800;

int main()
{
    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    
    objl::Loader loader;
    loader.LoadFile("../obj/african_head.obj");
   
    for(int i = 0; i < loader.LoadedIndices.size(); i+=3)
    {
        const std::vector<objl::Vertex>& vertices = loader.LoadedVertices;

        objl::Vector3 v0 = vertices[loader.LoadedIndices[i]].Position;
        objl::Vector3 v1 = vertices[loader.LoadedIndices[i + 1]].Position;
        objl::Vector3 v2 = vertices[loader.LoadedIndices[i + 2]].Position;

        v0.X = v0.X * (width / 2.0) + (width / 2.0);
        v1.X = v1.X * (width / 2.0) + (width / 2.0);
        v2.X = v2.X * (width / 2.0) + (width / 2.0);

        v0.Y = v0.Y * (height / 2.0) + (height / 2.0);
        v1.Y = v1.Y * (height / 2.0) + (height / 2.0);
        v2.Y = v2.Y * (height / 2.0) + (height / 2.0);

        tiny::Line line0 = tiny::line_new((int)v0.X, (int)v0.Y, (int)v1.X, (int)v1.Y, white);
        tiny::Line line1 = tiny::line_new((int)v0.X, (int)v0.Y, (int)v2.X, (int)v2.Y, white);
        tiny::Line line2 = tiny::line_new((int)v1.X, (int)v1.Y, (int)v2.X, (int)v2.Y, white);

        tiny::line_draw(line0, image);
        tiny::line_draw(line1, image);
        tiny::line_draw(line2, image);
    }

    image.write_tga_file("output.tga");
    return 0;
}