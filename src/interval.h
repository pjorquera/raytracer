#pragma once

#include <limits>
#include <cmath>

class Interval {
    
private:
    
    double _min;
    double _max;
  
public:
    
    static const Interval EMPTY;
    static const Interval UNIVERSE;
    
    Interval(double min = 0,
             double max = std::numeric_limits<double>::infinity()):
    _min(min), _max(max) {}
    
    Interval(const Interval& a, const Interval& b):
    _min(std::fmin(a.min(), b.min())), _max(std::fmax(a.max(), b.max())) {}
    
    double min() const {
        return _min;
    }
    
    double max() const {
        return _max;
    }
    
    void setMin(double min) {
        _min = min;
    }
    
    void setMax(double max) {
        _max = max;
    }
        
    bool contains(double x) const {
        return _min <= x && x <= _max;
    }
    
    double clamp(double x) const {
        if (x < _min) return _min;
        if (x > _max) return _max;
        return x;
    }
    
    Interval expand(double delta) const {
        auto padding = delta / 2.0;
        return Interval(_min - padding, _max + padding);
    }
    
    double size() const { return _max - _min; }

};

inline Interval operator+(const Interval& ival, double displacement) {
    return Interval(ival.min() + displacement, ival.max() + displacement);
}

inline Interval operator+(double displacement, const Interval& ival) {
    return ival + displacement;
}

inline const Interval Interval::EMPTY = Interval(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
inline const Interval Interval::UNIVERSE = Interval(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());
