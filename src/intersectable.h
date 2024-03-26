#pragma once

#include "ray.h"
#include "interval.h"
#include "aabb.h"

#include <memory>
#include <limits>
#include <cmath>

class Material;

class Hit {
  
private:
    
    Point _point;
    Vector _normal;
    std::shared_ptr<Material> _material;
    double _t;
    double _u;
    double _v;
    bool _frontFace;
    
public:
    
    void setPoint(const Point& point) { _point = point; }
    void setNormal(const Vector& normal) { _normal = normal; }
    void setMaterial(const std::shared_ptr<Material>& material) { _material = material; }
    void setT(double t) { _t = t; }
    void setU(double u) { _u = u; }
    void setV(double v) { _v = v; }
    void setFrontFace(bool frontFace) { _frontFace = frontFace; }
    
    const Point& point() const { return _point; }
    const Vector& normal() const { return _normal; }
    const std::shared_ptr<Material>& material() const { return _material; }
    double t() const { return _t; }
    double u() const { return _u; }
    double v() const { return _v; }
    bool frontFace() const { return _frontFace; }
    
    void setFaceNormal(const Ray& ray, const Vector& outwardNormal) {
        _frontFace = Vector::dot(ray.dir(), outwardNormal) < 0.0;
        _normal = _frontFace ? outwardNormal : -outwardNormal;
    }
    
};

class Intersectable {
    
public:
    
    virtual ~Intersectable() {}
    virtual bool intersects(const Ray& ray,
                            const Interval& interval,
                            Hit& hit
                            ) const = 0;
    virtual Aabb boundingBox() const = 0;
    
};

class Translate : public Intersectable {
  
private:
    
    Vector _offset;
    std::shared_ptr<Intersectable> _intersectable;
    Aabb _bbox;
    
public:
    
    Translate(const std::shared_ptr<Intersectable>& intersectable, const Vector& offset):
    _intersectable(intersectable),_offset(offset) {
        _bbox = _intersectable->boundingBox() + _offset;
    }
    
    bool intersects(const Ray &ray, const Interval &interval, Hit &hit) const override {
        Ray offset_r(ray.orig() - _offset, ray.dir(), ray.time());
        if (! _intersectable->intersects(offset_r, interval, hit)) return false;
        hit.setPoint(hit.point() + _offset);
        return true;
    }
    
    Aabb boundingBox() const override {
        return _bbox;
    }
    
};

class RotateY : public Intersectable {

private:
    
    std::shared_ptr<Intersectable> _intersectable;
    double _sinTheta;
    double _cosTheta;
    Aabb _bbox;
    
public:
    
    RotateY(std::shared_ptr<Intersectable> intersectable, double angle): _intersectable(intersectable) {
        auto radians = angle * (M_PI / 180.0);
        _sinTheta = std::sin(radians);
        _cosTheta = std::cos(radians);
        _bbox = intersectable->boundingBox();
        
        auto infinity = std::numeric_limits<double>::infinity();

        Point min( infinity,  infinity,  infinity);
        Point max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i*_bbox.x().max() + (1-i)*_bbox.x().min();
                    auto y = j*_bbox.y().max() + (1-j)*_bbox.y().min();
                    auto z = k*_bbox.z().max() + (1-k)*_bbox.z().min();

                    auto newx =  _cosTheta*x + _sinTheta*z;
                    auto newz = -_sinTheta*x + _cosTheta*z;

                    Vector tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        _bbox = Aabb(min, max);
    }
    
    bool intersects(const Ray &ray, const Interval &interval, Hit &hit) const override {
        // Change the ray from world space to object space
        auto origin = ray.orig();
        auto direction = ray.dir();
        
        origin[0] = _cosTheta*ray.orig()[0] - _sinTheta*ray.orig()[2];
        origin[2] = _sinTheta*ray.orig()[0] + _cosTheta*ray.orig()[2];
        
        direction[0] = _cosTheta*ray.dir()[0] - _sinTheta*ray.dir()[2];
        direction[2] = _sinTheta*ray.dir()[0] + _cosTheta*ray.dir()[2];
        
        Ray rotated_r(origin, direction, ray.time());
        
        // Determine where (if any) an intersection occurs in object space
        if (!_intersectable->intersects(rotated_r, interval, hit)) return false;
        
        // Change the intersection point from object space to world space
        auto p = hit.point();
        p[0] =  _cosTheta*hit.point()[0] + _sinTheta*hit.point()[2];
        p[2] = -_sinTheta*hit.point()[0] + _cosTheta*hit.point()[2];
        
        // Change the normal from object space to world space
        auto normal = hit.normal();
        normal[0] =  _cosTheta*hit.normal()[0] + _sinTheta*hit.normal()[2];
        normal[2] = -_sinTheta*hit.normal()[0] + _cosTheta*hit.normal()[2];
        
        hit.setPoint(p);
        hit.setNormal(normal);
        
        return true;
    }
    
    Aabb boundingBox() const override { return _bbox; }
    
};
