#pragma once

#include "interval.h"
#include "ray.h"

#include <cmath>

class Aabb {
  
private:
    
    Interval _x, _y, _z;
    
public:
    
    Aabb() {}
    Aabb(const Interval& ix, const Interval& iy, const Interval& iz):
    _x(ix), _y(iy), _z(iz) {}
    
    Aabb(const Point& a, const Point& b) {
        _x = Interval(fmin(a.x(), b.x()), fmax(a.x(), b.x()));
        _y = Interval(fmin(a.y(), b.y()), fmax(a.y(), b.y()));
        _z = Interval(fmin(a.z(), b.z()), fmax(a.z(), b.z()));
    }
    
    Aabb(const Aabb& box0, const Aabb& box1) {
        _x = Interval(box0.x(), box1.x());
        _y = Interval(box0.y(), box1.y());
        _z = Interval(box0.z(), box1.z());
    }
    
    inline Interval x() const { return _x; }
    inline Interval y() const { return _y; }
    inline Interval z() const { return _z; }
    
    const Interval& axis(int n) const {
        if (n == 1) return _y;
        if (n == 2) return _z;
        return _x;
    }
    
    bool intersects(const Ray& ray, Interval interval) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1 / ray.dir()[a];
            auto orig = ray.orig()[a];

            auto t0 = (axis(a).min() - orig) * invD;
            auto t1 = (axis(a).max() - orig) * invD;

            if (invD < 0) std::swap(t0, t1);

            if (t0 > interval.min()) interval.setMin(t0);
            if (t1 < interval.max()) interval.setMax(t1);

            if (interval.max() <= interval.min()) return false;
        }
        return true;
    }
    
};
