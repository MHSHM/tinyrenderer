#pragma once

#include "image.h"

namespace tiny 
{
    // accepts u and v in range 0 - 1
    TGAColor sample(Image* texture, float u, float v);
};