#include "sphere.h"

using namespace std;

bool Sphere::intersects(const Ray& ray, const Interval& interval, Hit& hit) const {
    const auto oc = ray.orig() - _center;
    const auto a = ray.dir().lengthSquared();
    const auto halfB = Vector::dot(oc, ray.dir());
    const auto c = oc.lengthSquared() - _radius * _radius;
    const auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0.0) return false;
    
    const auto sqrtd = sqrt(discriminant);
    auto root = (-halfB - sqrtd) / a;
    
    if (!interval.contains(root)) {
        root = (-halfB + sqrtd) / a;
        if (!interval.contains(root)) return false;
    }
    
    hit.setT(root);
    hit.setPoint(ray.at(hit.t()));
    Vector outwardNormal = (hit.point() - _center) / _radius;
    hit.setFaceNormal(ray, outwardNormal);
    hit.setMaterial(_material);
    
    return true;
}
