#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

using namespace std;

inline int clamp(int x, int low, int high) {
    return (x < low) ? low : ((x < high) ? x : high - 1);
}

Image::Image(const string& filename) {
    if(load(filename)) return;
    if(load("resources/" + filename)) return;
    if(load("../resources/" + filename)) return;
    if(load("../../resources/" + filename)) return;
    if(load("../../../resources/" + filename)) return;
    if(load("../../../../resources/" + filename)) return;
    if(load("../../../../../resources/" + filename)) return;
    if(load("../../../../../../resources/" + filename)) return;
    cerr << "ERROR: Could not load image file '" << filename << "'" << endl;
}

Image::~Image() {
    STBI_FREE(_data);
}

bool Image::load(const string& filename) {
    auto n = _bytesPerPixel;
    _data = stbi_load(filename.c_str(), &_width, &_height, &n, _bytesPerPixel);
    _bytesPerScanline = _width * _bytesPerPixel;
    return _data != nullptr;
}

bool Image::save(int width, int height, const unsigned char* data, const std::string& filename) {
    return stbi_write_png(filename.c_str(), width, height, 3, data, width * 3);
}

const unsigned char* Image::pixelData(int x, int y) const {
    // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
    static unsigned char magenta[] = { 255, 0, 255 };
    if (_data == nullptr) return magenta;

    x = clamp(x, 0, _width);
    y = clamp(y, 0, _height);

    return _data + y * _bytesPerScanline + x * _bytesPerPixel;
}
