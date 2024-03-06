#pragma once

#include "intersectable.h"
#include "material.h"
#include "aabb.h"

#include <memory>

class Sphere : public Intersectable {
    
private:
    
    Point _center;
    double _radius;
    std::shared_ptr<Material> _material;
    bool _isMoving;
    Vector _centerVec;
    Aabb _boundingBox;
    
public:
    
    Sphere(const Point& center, double radius, const std::shared_ptr<Material>& material):
        _center(center),_radius(radius),_material(material),_isMoving(false) {
            const auto rvec = Vector(_radius, _radius, _radius);
            _boundingBox = Aabb(_center - rvec, _center + rvec);
        }
    Sphere(const Point& centerFrom, const Point& centerTo, double radius, const std::shared_ptr<Material>& material):
        _center(centerFrom),_radius(radius),_material(material),_isMoving(true) {
            const auto rvec = Vector(_radius, _radius, _radius);
            const Aabb box1(centerFrom - rvec, centerFrom + rvec);
            const Aabb box2(centerTo - rvec, centerTo + rvec);
            _boundingBox = Aabb(box1, box2);
            _centerVec = centerTo - centerFrom;
        }
    bool intersects(const Ray& ray, const Interval& interval, Hit& hit) const override;
    
    Point center(double time) const {
        return _center + time * _centerVec;
    }
    
    Aabb boundingBox() const override { return _boundingBox; }
    
};
