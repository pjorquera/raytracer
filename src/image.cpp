#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

inline int clamp(int x, int low, int high) {
    return (x < low) ? low : ((x < high) ? x : high - 1);
}

Image::Image(const std::string& filename) {
    load(filename);
}

Image::~Image() {
    STBI_FREE(_data);
}

bool Image::load(const std::string& filename) {
    auto n = _bytesPerPixel;
    _data = stbi_load(filename.c_str(), &_width, &_height, &n, _bytesPerPixel);
    _bytesPerScanline = _width * _bytesPerPixel;
    return _data != nullptr;
}

const unsigned char* Image::pixelData(int x, int y) const {
    // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
    static unsigned char magenta[] = { 255, 0, 255 };
    if (_data == nullptr) return magenta;

    x = clamp(x, 0, _width);
    y = clamp(y, 0, _height);

    return _data + y * _bytesPerScanline + x * _bytesPerPixel;
}
