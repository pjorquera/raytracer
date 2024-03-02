#pragma once

#include "intersectable.h"
#include "material.h"

#include <memory>

class Sphere : public Intersectable {
    
private:
    
    Point _center;
    double _radius;
    std::shared_ptr<Material> _material;
    
public:
    
    Sphere(const Point& center, double radius, const std::shared_ptr<Material>& material):
        _center(center),_radius(radius),_material(material) {}
    bool intersects(const Ray& ray, const Interval& interval, Hit& hit) const override;
    
};
