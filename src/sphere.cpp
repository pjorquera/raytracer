#include "sphere.h"

#include <cmath>

using namespace std;

bool Sphere::intersects(const Ray& ray, const Interval& interval, Hit& hit) const {
    auto currentCenter = _isMoving ? center(ray.time()) : _center;
    auto oc = ray.orig() - currentCenter;
    auto a = ray.dir().lengthSquared();
    auto halfB = Vector::dot(oc, ray.dir());
    auto c = oc.lengthSquared() - _radius * _radius;
    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0.0) return false;
    
    auto sqrtd = sqrt(discriminant);
    auto root = (-halfB - sqrtd) / a;
    
    if (!interval.contains(root)) {
        root = (-halfB + sqrtd) / a;
        if (!interval.contains(root)) return false;
    }
    
    hit.setT(root);
    hit.setPoint(ray.at(hit.t()));
    Vector outwardNormal = (hit.point() - currentCenter) / _radius;
    hit.setFaceNormal(ray, outwardNormal);
    Sphere::computeUV(outwardNormal, hit);
    hit.setMaterial(_material);
    
    return true;
}

void Sphere::computeUV(const Point& point, Hit& hit) {
    auto theta = acos(-point.y());
    auto phi = atan2(-point.z(), point.x()) + M_PI;

    hit.setU(phi / (2.0 * M_PI));
    hit.setV(theta / M_PI);
}
