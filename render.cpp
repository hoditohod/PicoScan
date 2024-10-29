#include "render.h"

#include <utility>
#include "log.h"

Raster::Raster(int32_t width, int32_t height) :
    sBuffer(width, height)
{}

// IO: going from bottom-up, with reverse fill-convention
// would allow to replace ceil with floor, sparing one
// addition per operation
/* Clipping is supported. If vertex coordinates are out
of the screen, then the triangle will render correctly, skipping the non-visible parts. The only source of error is the overflowing of fixpoint calculations. The safe range is generally -1024 to 1024 for both x&y.
*/
void Raster::triangle(TriVertex* a, TriVertex* b, TriVertex* c) {
    // sort vertices top to bottom (may change winding order)
    if (a->y > b->y) std::swap(a, b);
    if (a->y > c->y) std::swap(a, c);
    if (b->y > c->y) std::swap(b, c);

    // delta Y values top to bottom
    Fix10 dyab = b->y - a->y;
    Fix10 dyac = c->y - a->y;
    Fix10 dybc = c->y - b->y;

    // slopes
    Fix10 slope_ab=(b->x - a->x)/dyab;
    Fix10 slope_ac=(c->x - a->x)/dyac;
    Fix10 slope_bc=(c->x - b->x)/dybc;

    // check triangle "sideness"
    //       a          a
    //      / \        / \
    //     b -_\      /_- b
    //          c    c
    //      left      right

    // b2: point on ac edge across of b (with same y)
    //       a
    //      / \
    //     b -_b2  <-- left sided if b.x<b2x
    //          c
    Fix10 b2x = slope_ac*dyab + a->x;

    // distance of a.y and next scanline (example vals)
    //         a          a.y = 1.7
    //        / \      ^
    //       /   \     |  yTopResidue = 0.3
    //      /     \    v
    // - - - - - - - - -  yTop = 2.0
    // ^next integer y coordinate
    Fix10 yTop = ceil(a->y);
    Fix10 yMid = ceil(b->y);
    Fix10 yBottom = ceil(c->y);

    // clamp y values for clipping
    {
        Fix10 clampTop{0};
        Fix10 clampBottom{sBuffer.height};
        yTop = std::max(yTop, clampTop);
        yMid = std::clamp(yMid, clampTop, clampBottom);
        yBottom = std::min(yBottom, clampBottom);
    }

    //        a
    //       / \
    //      /   \    top half triangle
    // - - b - - \ - - - - - - - - - - - -
    //      ---   \  bottom half triangle
    //         --- c

    if (b->x < b2x) {
        // left sided
        TrapezoidParams tp;
        tp.slopeRight = slope_ac;

        // top half
        Fix10 yTopResidue = yTop - a->y;
        tp.yTop = yTop.toInt();
        tp.yBottom = yMid.toInt();
        tp.slopeLeft = slope_ab;
        tp.xLeft = a->x + yTopResidue * tp.slopeLeft;
        tp.xRight = a->x + yTopResidue * tp.slopeRight;
        trapezoid(tp);

        // bottom half
        Fix10 yMidLeftDist = yMid - b->y;
        Fix10 yMidRightDist = yMid - a->y;
        tp.yTop = yMid.toInt();
        tp.yBottom = yBottom.toInt();
        tp.slopeLeft = slope_bc;
        tp.xLeft = b->x + yMidLeftDist * tp.slopeLeft;
        tp.xRight = a->x + yMidRightDist * tp.slopeRight;
        trapezoid(tp);
    } else {
        // right sided
        TrapezoidParams tp;
        tp.slopeLeft = slope_ac;

        // top half
        Fix10 yTopResidue = yTop - a->y;
        tp.yTop = yTop.toInt();
        tp.yBottom = yMid.toInt();
        tp.slopeRight = slope_ab;
        tp.xLeft = a->x + yTopResidue * tp.slopeLeft;
        tp.xRight = a->x + yTopResidue * tp.slopeRight;
        trapezoid(tp);

        // bottom half
        Fix10 yMidLeftDist = yMid - a->y;
        Fix10 yMidRightDist = yMid - b->y;
        tp.yTop = yMid.toInt();
        tp.yBottom = yBottom.toInt();
        tp.slopeRight = slope_bc;
        tp.xLeft = a->x + yMidLeftDist * tp.slopeLeft;
        tp.xRight = b->x + yMidRightDist * tp.slopeRight;
        trapezoid(tp);
    }
}

#if 1
int render(uint32_t width, uint32_t height, uint32_t time, unsigned char* buffer) {
    TriVertex a{ Fix10{ 50}, Fix10{ 50} };
    TriVertex b{ Fix10{200}, Fix10{ 80} };
    TriVertex c{ Fix10{170}, Fix10{200} };
    Raster rast(320, 240);
    rast.triangle(&a, &b, &c);
    return 8;
}
#endif


void Raster::trapezoid(const TrapezoidParams& tp) {
    auto xLeft = tp.xLeft;
    auto xRight = tp.xRight;

    for(int32_t y=tp.yTop; y<tp.yBottom; y++) {
        int32_t xLeftInt = std::max(xLeft.toInt(), 0);
        int32_t xRightInt = std::min(xRight.toInt(), sBuffer.width);
        sBuffer.add(y, {xLeftInt, xRightInt});

        xLeft.val += tp.slopeLeft.val;
        xRight.val += tp.slopeRight.val;
    }
}


std::ostream& operator<<(std::ostream& os, const TrapezoidParams& tp) {
    os << "yTop: " << tp.yTop << ", yBottom: " << tp.yBottom;
    os << ", xLeft: " << tp.xLeft << " xRight: " << tp.xRight.toFloat();
    os << ", slopeLeft: " << tp.slopeLeft << ", slopeRight: " << tp.slopeRight;
    return os;
}
