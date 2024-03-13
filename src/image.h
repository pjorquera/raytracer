#pragma once

#include <string>

class Image {
    
private:
    
    const int _bytesPerPixel = 3;
    unsigned char* _data;
    int _width;
    int _height;
    int _bytesPerScanline;
        
public:
    
    Image():_data(nullptr) {}
    ~Image();
    Image(const std::string& filename);
    bool load(const std::string& filename);
  
    inline auto width() const { return _width; }
    inline auto height() const { return _height; }
    
    const unsigned char* pixelData(int x, int y) const;
    
};
