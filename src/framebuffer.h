#pragma once

#include "vector.h"

#include <string>

class FrameBuffer {
    
private:
    
    int _width;
    int _height;
    unsigned char* _buffer;
    
public:
    
    FrameBuffer(int width, int height);
    virtual ~FrameBuffer();
    
    int width() const { return _width; }
    int height() const { return _height; }
    void draw(int x, int y, Color color);
    void save(const std::string& filename) const;

};
