#include "constantmedium.h"

#include <cmath>

using namespace std;

bool ConstantMedium::intersects(const Ray &ray, const Interval &interval, Hit &hit) const {
    Hit hit1, hit2;

    if (!_boundary->intersects(ray, Interval::UNIVERSE, hit1)) return false;
    if (!_boundary->intersects(ray, Interval(hit1.t() + 0.0001, numeric_limits<double>::infinity()), hit2)) return false;

    if (hit1.t() < interval.min()) hit1.setT(interval.min());
    if (hit2.t() > interval.max()) hit2.setT(interval.max());

    if (hit1.t() >= hit2.t()) return false;

    if (hit1.t() < 0.0) hit1.setT(0.0);

    auto ray_length = ray.dir().length();
    auto distance_inside_boundary = (hit2.t() - hit1.t()) * ray_length;
    auto hit_distance = _negInvDensity * log(randomDouble());

    if (hit_distance > distance_inside_boundary) return false;

    hit.setT(hit1.t() + hit_distance / ray_length);
    hit.setPoint(ray.at(hit.t()));

    hit.setNormal(Vector(1.0, 0.0, 0.0));  // arbitrary
    hit.setFrontFace(true);     // also arbitrary
    hit.setMaterial(_phaseFunction);

    return true;
}

