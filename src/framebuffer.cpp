#include "framebuffer.h"
#include "interval.h"
#include "image.h"

using namespace std;

FrameBuffer::FrameBuffer(int width, int height):_width(width),_height(height) {
    _buffer = new unsigned char[_height * _width * 3];
}

FrameBuffer::~FrameBuffer() {
    delete [] _buffer;
}

void FrameBuffer::draw(int x, int y, Color color) {
    static const Interval intensity(0.000, 0.999);
    auto pixelOffset = 3 * (x + y * _width);
    _buffer[pixelOffset + 0] = 256 * intensity.clamp(color.r());
    _buffer[pixelOffset + 1] = 256 * intensity.clamp(color.g());
    _buffer[pixelOffset + 2] = 256 * intensity.clamp(color.b());
}

void FrameBuffer::save(const std::string& filename) const {
    Image::save(_width, _height, _buffer, filename);
}
