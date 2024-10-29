#pragma once
#include <vector>
#include <ostream>

/* SBuffer data structures. SBuffer stores spans for each scanline that primitive rasterization routines emit.
Each span contains all information neccessaŕy to render the pixels corresponding to the span. It's an
interesting question how to store the spans in memory. Each scanline consits of spans, forming a partition.
When new spans are inserted exising spans may be broken into pieces, overwritten or removed. The data structure
shall support these operations efficiently.

If spans are stored in an array then insert/remove at the beginning or the end is costly. Initial size and
growing the array raises questions as well.

A linked list looks a good choice with constant insertion and removal time. Each span (node) in the list has a next pointer which needs ~2 bytes. Spans have variable length depending on the information stored. This needs a smart memory alllocator to serve and reuse span memory.
Finding an insertion point also requires list traversal.
Another option would be to use a tree. 2 pointers per node, same memory allocator, and the added complexity of tree rotations. Finding the insertion point would take log2 time.

Combination of list and array. Spans for a scanline are stored in 64 byte bucket. Each bucket works as an array of tigthly packed spans. If one bucket is not enough to hold all spans, then the last 2 bytes contain the memory offset to the next bucket. The bucket has a 2 byte header containing the sum of span lengths (10bits) and number of free bytes (6bits).

Finding an insertion point can happen jumpig over the buckets. Insert/remove happens as with arrays, at most 62 bytes moved. If needed a new bucket is allocated.
*/



struct Span {
    int32_t xLeft;
    int32_t xRight;
};

std::ostream& operator<<(std::ostream& os, const Span& span);


class SBuffer {
public:
    using BufferType = std::vector<std::vector<Span>>;

    const int32_t width;
    const int32_t height;

    SBuffer(int32_t width, int32_t height) :
        width(width),
        height(height)
    {
        _buffer.resize(height);
    }

    void add(int32_t yPos, const Span& span) {
        _buffer[yPos].emplace_back(span);
    }

    BufferType const& buffer() const {
        return _buffer;
    }
private:
     BufferType _buffer;


    // a better add() should take the minimal set of parameters neede for insertion (xstart, xend, zstart, zdelta) and return the place to store color parameters if the span was not rejected
};



std::ostream& operator<<(std::ostream& os, const SBuffer& sBuffer);


// Monochrome (1bpp, black/white) framebuffer to render sbuffer contents into
// dimensions are same as sbuffer
// used for testing pixel coverage of primitive rasterization
// pixel values are flipped(xor) when written to detect overwrites
class FrameBuffer1BPP{
public:
    const SBuffer& sBuffer;
    std::vector<uint8_t> frameBuffer;

    FrameBuffer1BPP(const SBuffer& sBuffer) :
        sBuffer(sBuffer),
        frameBuffer(sBuffer.width*sBuffer.height)
    {}

    void rasterize() {
        //memset();

        // iterate scanlines
        for (size_t y=0; y<sBuffer.buffer().size(); y++) {
            // spans
            for(size_t spanIdx=0; spanIdx<sBuffer.buffer()[y].size(); spanIdx++) {
                auto& span = sBuffer.buffer()[y][spanIdx];
                // span pixels
                for (int32_t x=span.xLeft; x<span.xRight; x++) {
                    auto& pixel = frameBuffer[y*sBuffer.width+x];
                    // invert color each time the pixel is written
                    pixel^=1;
                }
            }
        }
    }

};


std::ostream& operator<<(std::ostream& os, const FrameBuffer1BPP& fb);
