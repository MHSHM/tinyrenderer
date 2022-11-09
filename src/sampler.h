#pragma once

#include "tgaimage.h"

namespace tiny 
{
    // accepts u and v in range 0 - 1
    TGAColor sample(TGAImage* texture, float u, float v);
};