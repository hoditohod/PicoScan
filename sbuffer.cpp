#include "sbuffer.h"


std::ostream& operator<<(std::ostream& os, const Span& span) {
    os << "(" << span.xLeft << "," << span.xRight << ")";
    return os;
}


std::ostream& operator<<(std::ostream& os, const SBuffer& sBuffer) {
   for (size_t y=0; y<sBuffer.buffer().size(); y++) {
        // spans
        os << y << ": ";
        for(size_t spanIdx=0; spanIdx<sBuffer.buffer()[y].size(); spanIdx++) {
            auto& span = sBuffer.buffer()[y][spanIdx];
            os << span;
        }
        os << std::endl;
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const FrameBuffer1BPP& fb) {
    for(int y=0; y<fb.sBuffer.height; y++) {
        for(int x=0; x<fb.sBuffer.width; x++) {
            char pix = fb.frameBuffer[y*fb.sBuffer.width+x] ? '#' : '.';
             os << pix << " ";
        }
        os << std::endl;
    }

    return os;
}

