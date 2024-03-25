#pragma once

#include "intersectable.h"
#include "material.h"
#include "aabb.h"
#include "scene.h"

#include <cmath>

class Quad : public Intersectable {

private:
    
    Point _q;
    Vector _u, _v;
    std::shared_ptr<Material> _mat;
    Aabb _bbox;
    Vector _normal;
    double _d;
    Vector _w;

public:
    
    Quad(const Point& q, const Vector& u, const Vector& v, const std::shared_ptr<Material>& mat):_q(q),_u(u),_v(v),_mat(mat) {

        auto n = Vector::cross(_u, _v);
        _normal = n.unit();
        _d = Vector::dot(_normal, _q);
        _w = n / Vector::dot(n, n);
        
        computeBoundingBox();
    }
    
    virtual void computeBoundingBox() {
        _bbox = Aabb(_q, _q + _u + _v).pad();
    }
    
    virtual Aabb boundingBox() const override { return _bbox; }
    virtual bool intersects(const Ray& ray, const Interval& interval, Hit& hit) const override {
        auto denom = Vector::dot(_normal, ray.dir());
        if (std::fabs(denom) < 1e-8) return false;
        auto t = (_d - Vector::dot(_normal, ray.orig())) / denom;
        if (!interval.contains(t)) return false;
        
        auto intersection = ray.at(t);
        Vector planar_hitpt_vector = intersection - _q;
        auto alpha = Vector::dot(_w, Vector::cross(planar_hitpt_vector, _v));
        auto beta = Vector::dot(_w, Vector::cross(_u, planar_hitpt_vector));

        if (!isInterior(alpha, beta, hit)) return false;

        hit.setT(t);
        hit.setPoint(intersection);
        hit.setMaterial(_mat);
        hit.setFaceNormal(ray, _normal);

        return true;
    }
    virtual bool isInterior(double a, double b, Hit& hit) const {
        if ((a < 0.0) || (1.0 < a) || (b < 0.0) || (1.0 < b)) return false;

        hit.setU(a);
        hit.setV(b);
        return true;
    }
    
};

inline std::shared_ptr<Intersectable> createBox(const Point& a, const Point& b, std::shared_ptr<Material> mat)
{
    auto sides = std::make_shared<Scene>();

    auto min = Point(std::fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = Point(std::fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = Vector(max.x() - min.x(), 0, 0);
    auto dy = Vector(0, max.y() - min.y(), 0);
    auto dz = Vector(0, 0, max.z() - min.z());

    sides->add(std::make_shared<Quad>(Point(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->add(std::make_shared<Quad>(Point(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->add(std::make_shared<Quad>(Point(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->add(std::make_shared<Quad>(Point(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->add(std::make_shared<Quad>(Point(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->add(std::make_shared<Quad>(Point(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}
