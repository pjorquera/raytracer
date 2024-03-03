#pragma once

#include "vector.h"

class Ray {
    
private:
    
    Point _orig;
    Vector _dir;
    double _tm;
    
public:
    
    Ray() {}
    Ray(const Point& orig, const Vector& dir, double time = 0.0) : _orig(orig), _dir(dir), _tm(time) {}
    
    const Point& orig() const { return _orig; }
    const Vector& dir() const { return _dir; }
    
    Point at(double t) const {
        return _orig + t * _dir;
    }
    
    double time() const { return _tm; }
    
};
