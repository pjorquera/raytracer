#pragma once

#include "ray.h"
#include "interval.h"

#include <memory>

class Material;

class Hit {
  
private:
    
    Point _point;
    Vector _normal;
    std::shared_ptr<Material> _material;
    double _t;
    bool _fronFace;
    
public:
    
    void setPoint(const Point& point) { _point = point; }
    void setNormal(const Vector& normal) { _normal = normal; }
    void setMaterial(const std::shared_ptr<Material>& material) { _material = material; }
    void setT(double t) { _t = t; }
    void setFrontFace(bool frontFace) { _fronFace = frontFace; }
    
    const Point& point() const { return _point; }
    const Vector& normal() const { return _normal; }
    const std::shared_ptr<Material>& material() const { return _material; }
    double t() const { return _t; }
    bool frontFace() const { return _fronFace; }
    
    void setFaceNormal(const Ray& ray, const Vector& outwardNormal) {
        _fronFace = Vector::dot(ray.dir(), outwardNormal) < 0.0;
        _normal = _fronFace ? outwardNormal : -outwardNormal;
    }
    
};

class Intersectable {
    
public:
    
    virtual ~Intersectable() {}
    virtual bool intersects(const Ray& ray,
                            const Interval& interval,
                            Hit& hit
                            ) const = 0;
    
};
