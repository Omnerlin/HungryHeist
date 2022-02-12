#pragma once
#include <Typedefs.h>
#include <string>
#include <vector>

struct Animation {
    std::string name;
    std::string texture;
    int32 width{0};
    int32 height{0};
    int32 startFrame{0};
    int32 endFrame{0};
    float framesPerSecond{8};
};