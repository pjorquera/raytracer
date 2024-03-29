#pragma once

#include "vector.h"

class Perlin {
  
private:

    Vector* _randomData;
    int* _permuteX;
    int* _permuteY;
    int* _permuteZ;
    
public:
    
    static const int POINT_COUNT = 256;
    
    Perlin();
    ~Perlin();
    
    double noise(const Point& point) const;
    double turbulence(const Point& p, int depth = 7) const;
};
