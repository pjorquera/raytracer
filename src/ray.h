#pragma once

#include "vector.h"

class Ray {
    
private:
    
    Point _orig;
    Vector _dir;
    
public:
    
    Ray() {}
    Ray(const Point& orig, const Vector& dir) : _orig(orig), _dir(dir) {}
    
    const Point& orig() const { return _orig; }
    const Vector& dir() const { return _dir; }
    
    Point at(double t) const {
        return _orig + t * _dir;
    }
    
};
