#pragma once

#include <limits>

class Interval {
    
private:
    
    double _min;
    double _max;
  
public:
        
    Interval(double min = 0,
             double max = std::numeric_limits<double>::infinity()):
    _min(min), _max(max) {}
    
    double min() const {
        return _min;
    }
    
    double max() const {
        return _max;
    }
        
    bool contains(double x) const {
        return _min <= x && x <= _max;
    }
    
    double clamp(double x) const {
        if (x < _min) return _min;
        if (x > _max) return _max;
        return x;
    }

};
