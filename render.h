#pragma once

#include <cstdint>
#include <vector>
#include "fix.h"


// Parameters associated to a vertex of a triangle:
// coordinates, light intensity, texture coordinates
struct TriVertex { //PerVertexParams, CommonParams
    Fix10 x;
    Fix10 y;
};


struct TrapezoidParams {
    int32_t yTop, yBottom;
    Fix10 xLeft, xRight;
    Fix10 slopeLeft, slopeRight;
};

class Raster {
public:
    std::vector<uint8_t> frameBuffer;
    uint32_t stride; //constexpr on rp2040?
    int32_t width;
    int32_t height;
    Raster(int32_t width, int32_t height);
    ~Raster() = default;
    void trapezoid(const TrapezoidParams& tp);
    void triangle(TriVertex* a, TriVertex* b, TriVertex* c);
};


int render(uint32_t width, uint32_t height, uint32_t time, unsigned char* buffer);

std::ostream& operator<<(std::ostream& os, const TrapezoidParams& tp);
