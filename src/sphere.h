#pragma once

#include "intersectable.h"
#include "material.h"

#include <memory>

class Sphere : public Intersectable {
    
private:
    
    Point _center;
    double _radius;
    std::shared_ptr<Material> _material;
    bool _isMoving;
    Vector _centerVec;
    
public:
    
    Sphere(const Point& center, double radius, const std::shared_ptr<Material>& material):
        _center(center),_radius(radius),_material(material),_isMoving(false) {}
    Sphere(const Point& centerFrom, const Point& centerTo, double radius, const std::shared_ptr<Material>& material):
        _center(centerFrom),_radius(radius),_material(material),_isMoving(true) {
            _centerVec = centerTo - centerFrom;
        }
    bool intersects(const Ray& ray, const Interval& interval, Hit& hit) const override;
    
    Point center(double time) const {
        return _center + time * _centerVec;
    }
    
};
